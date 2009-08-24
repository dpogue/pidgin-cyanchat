/**
 * @file cc_account.c CyanChat Protocol Plugin
 *
 *
 * libcyanchat
 *
 * libcyanchat is the property of its developers. See the COPYRIGHT file
 * for more details.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.	If not, see <http://www.gnu.org/licenses/>.
 */

#include "cc_account.h"
#include "cc_parser.h"

/** @copydoc _CyanChatAccountPrivate */
typedef struct _CyanChatAccountPrivate CyanChatAccountPrivate;

extern guint cyanchat_blocking;
extern GStaticMutex cyanchat_mutex;

/** The CyanChat Account class. */
struct _CyanChatAccountClass
{
	GObjectClass parent_class;	/**< The parent class. */
};

/** The CyanChat Account's data. */
struct _CyanChatAccount
{
	GObject parent;	/**< The parent of this account. */
	CyanChatAccountPrivate* priv;	/**< Private data for the account. */
	gint fd;	/**< The socket file descriptor. */
};

/** The CyanChat Account's private data. */
struct _CyanChatAccountPrivate
{
	PurpleConnection* pc;	/* libpurple's Connection */
	gchar* host;	/* Server Name */
	guint port;	/* Server Port */
	gchar* reqnick;	/* Our "desired" nickname while waiting for approval */
	gint id;	/* Purple Conversation ID */
	PurpleConversation* conv;	/* The room's PurpleConversation*/
	gchar* rxbuf;	/* The read buffer */
	guint rxoff;	/* The offset in the read buffer */
	guint rxsize;	/* The size of the read buffer */
	GQueue* messagequeue;	/* Queue for incoming messages */
	guint inputhandler;	/* Input Handling callback method */
	guint ping_timeout;	/* Timer for ping timeouts */
};

#define CYANCHAT_ACCOUNT_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), CYANCHAT_TYPE_ACCOUNT, CyanChatAccountPrivate))

static GObjectClass* parent_class = NULL;

static void
cyanchat_account_class_init(CyanChatAccountClass* klass)
{
	GObjectClass* gobject_class = (GObjectClass*)klass;
	parent_class = g_type_class_peek_parent(klass);
	
	gobject_class->finalize = cyanchat_account_finalize;

	g_type_class_add_private(klass, sizeof(CyanChatAccountPrivate));
}

static void
cyanchat_account_init(CyanChatAccount* account)
{
	account->priv = CYANCHAT_ACCOUNT_GET_PRIVATE(account);
	account->priv->pc = NULL;
	account->priv->host = NULL;
	account->priv->reqnick = NULL;
	account->priv->id = -1;
	account->priv->conv = NULL;
	account->priv->messagequeue = g_queue_new();
	
	account->fd = -1;
}

static void
cyanchat_account_finalize(GObject* object)
{
	CyanChatAccountPrivate* priv = CYANCHAT_ACCOUNT_GET_PRIVATE(object);
	
	g_queue_free(priv->messagequeue);
	
	parent_class->finalize(object);
}

/**
 * Gets the account's GType.
 *
 * @return The account's GType.
 */
GType
cyanchat_account_get_type()
{
	static GType type = 0;

	if (type == 0) {
		static const GTypeInfo info = {
			sizeof(CyanChatAccountClass),
			NULL,
			NULL,
			(GClassInitFunc) cyanchat_account_class_init,
			NULL,
			NULL,
			sizeof(CyanChatAccount),
			0,
			(GInstanceInitFunc) cyanchat_account_init,
			NULL
		};
		type = g_type_register_static(G_TYPE_OBJECT, "CyanChatAccount", &info, 0);
	}
	return type;
}

/**
 * Create a new CyanChatAccount.
 *
 * @return A CyanChatAccount.
 */
CyanChatAccount*
cyanchat_account_new()
{
	CyanChatAccount* cc;
	
	cc = CYANCHAT_ACCOUNT(g_object_new(cyanchat_account_get_type(), NULL));
	
	return cc;
}

/**
 * Read data from the socket into a buffer and then add the buffer
 * to the GQueue for parsing.
 *
 * @param cc The account to which is read.
 */
void
cyanchat_account_read(CyanChatAccount* cc)
{
	CyanChatAccountPrivate* priv;
	g_return_if_fail(cc != NULL);
	priv = CYANCHAT_ACCOUNT_GET_PRIVATE(cc);
	
	guint len;
	
	if (priv->rxsize < priv->rxoff + 1024) {
		priv->rxsize += 1024;
		priv->rxbuf = g_realloc(priv->rxbuf, priv->rxsize);
	}
	
	len = read(cc->fd, priv->rxbuf + priv->rxoff, 1024 - 1);
	
	if (len < 0 && errno == EAGAIN) {
		return;
	} else if (len < 0) {
		/* Read Error */
		purple_connection_error_reason(priv->pc, PURPLE_CONNECTION_ERROR_NETWORK_ERROR, _("Read error"));
		return;
	} else if (len == 0) {
		/* Server Disconnected */
		purple_connection_error_reason(priv->pc, PURPLE_CONNECTION_ERROR_NETWORK_ERROR, _("Server has disconnected"));
		return;
	}
	
	gchar* cur;
	gchar* end;
	
	priv->pc->last_received = time(NULL);
	priv->inbufused += len;
	priv->inbuf[priv->inbufused] = '\0';
	
	cur = priv->inbuf;

	while ((cur < (priv->inbuf + priv->inbufused)) && !*cur)
		cur++;
	
	while (cur < priv->inbuf + priv->inbufused && ((end = strstr(cur, "\r\n")) || (end = strstr(cur, "\n")))) {
		int step = (*end == '\r' ? 2 : 1);
		*end = '\0';
		purple_debug_misc(CYANCHAT_PLUGIN_NAME, "> %s\n", cur);
		CyanChatMessage* m;
		CyanChatTransport* t;

		m = cyanchat_message_new(cur, time(NULL));

		t = cyanchat_transport_new(cc, NULL, NULL, m);
		
		if(cyanchat_parser_quick(t)) {
			g_queue_push_head(priv->messagequeue, m);
		}

		gboolean block = FALSE;
		g_static_mutex_lock(cyanchat_mutex);
		block = (cyanchat_block > 0);
		g_static_mutex_unlock(cyanchat_mutex);
		
		if(!block) {
			/* Do parsing in a separate thread here */
		}
		
		cur = end + step;
	}
	if (cur != priv->inbuf + priv->inbufused) { /* leftover */
		priv->inbufused -= (cur - priv->inbuf);
		memmove(priv->inbuf, cur, priv->inbufused);
	
	} else {
		priv->inbufused = 0;
	}
}

/**
 * Get the #PurpleConnection of a #CyanChatAccount.
 *
 * @param cc The account whose connection to get.
 *
 * @return The account's #PurpleConnection.
 */
PurpleConnection*
cyanchat_account_get_connection(CyanChatAccount* cc)
{
	CyanChatAccountPrivate* priv;
	g_return_val_if_fail(cc != NULL, NULL);
	priv = CYANCHAT_ACCOUNT_GET_PRIVATE(cc);
	
	return priv->pc;
}

/**
 * Set the #PurpleConnection of a #CyanChatAccount.
 *
 * @param cc The account whose connection to set.
 * @param pc The connection to assign.
 */
void
cyanchat_account_set_connection(CyanChatAccount* cc, PurpleConnection* pc)
{
	CyanChatAccountPrivate* priv;
	g_return_if_fail(cc != NULL);
	priv = CYANCHAT_ACCOUNT_GET_PRIVATE(cc);
	
	priv->pc = pc;
}

const gchar*
cyanchat_account_get_host(CyanChatAccount* cc)
{
	CyanChatAccountPrivate* priv;
	g_return_val_if_fail(cc != NULL, NULL);
	priv = CYANCHAT_ACCOUNT_GET_PRIVATE(cc);
	
	return priv->host;
}

void
cyanchat_account_set_host(CyanChatAccount* cc, const gchar* host)
{
	CyanChatAccountPrivate* priv;
	g_return_if_fail(cc != NULL);
	priv = CYANCHAT_ACCOUNT_GET_PRIVATE(cc);
	
	priv->host = host;
}

guint
cyanchat_account_get_port(CyanChatAccount* cc)
{
	CyanChatAccountPrivate* priv;
	g_return_val_if_fail(cc != NULL, 0);
	priv = CYANCHAT_ACCOUNT_GET_PRIVATE(cc);
	
	return priv->port;
}

void
cyanchat_account_set_port(CyanChatAccount* cc, guint port)
{
	CyanChatAccountPrivate* priv;
	g_return_if_fail(cc != NULL);
	priv = CYANCHAT_ACCOUNT_GET_PRIVATE(cc);
	
	priv->port = port;
}

const gchar*
cyanchat_account_get_req_nick(CyanChatAccount* cc)
{
	CyanChatAccountPrivate* priv;
	g_return_val_if_fail(cc != NULL, NULL);
	priv = CYANCHAT_ACCOUNT_GET_PRIVATE(cc);
	
	return priv->reqnick;
}

void
cyanchat_account_set_req_nick(CyanChatAccount* cc, const gchar* nick)
{
	CyanChatAccountPrivate* priv;
	g_return_if_fail(cc != NULL);
	priv = CYANCHAT_ACCOUNT_GET_PRIVATE(cc);
	
	priv->reqnick = nick;
}


void
cyanchat_account_set_input(CyanChatAccount* cc, guint in)
{
	CyanChatAccountPrivate* priv;
	g_return_if_fail(cc != NULL);
	priv = CYANCHAT_ACCOUNT_GET_PRIVATE(cc);
	
	priv->inputhandler = in;
}


void
cyanchat_account_remove_input(CyanChatAccount* cc)
{
	CyanChatAccountPrivate* priv;
	g_return_if_fail(cc != NULL);
	priv = CYANCHAT_ACCOUNT_GET_PRIVATE(cc);
	g_return_if_fail(priv->inputhandler > 0);
	
	purple_input_remove(priv->inputhandler);
	priv->inputhandler = 0;
}

guint
cyanchat_account_get_timeout(CyanChatAccount* cc)
{
	CyanChatAccountPrivate* priv;
	g_return_val_if_fail(cc != NULL, 0);
	priv = CYANCHAT_ACCOUNT_GET_PRIVATE(cc);
	
	return priv->ping_timeout;
}

void
cyanchat_account_set_timeout(CyanChatAccount* cc, guint interval, GSourceFunc function)
{
	CyanChatAccountPrivate* priv;
	g_return_if_fail(cc != NULL);
	priv = CYANCHAT_ACCOUNT_GET_PRIVATE(cc);
	
	priv->ping_timeout = purple_timeout_add_seconds(interval, function, cc);
}

void
cyanchat_account_clear_timeout(CyanChatAccount* cc)
{
	CyanChatAccountPrivate* priv;
	g_return_if_fail(cc != NULL);
	priv = CYANCHAT_ACCOUNT_GET_PRIVATE(cc);
	
	priv->ping_timeout = 0;
}

gboolean
cyanchat_account_remove_timeout(CyanChatAccount* cc)
{
	CyanChatAccountPrivate* priv;
	g_return_val_if_fail(cc != NULL, FALSE);
	priv = CYANCHAT_ACCOUNT_GET_PRIVATE(cc);
	
	return purple_timeout_remove(priv->ping_timeout);
}
