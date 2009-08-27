/* 
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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "cc_session.h"

/* Private Data */
struct _CyanChatSessionPrivate {
	gchar* server;
	guint port;
	GIOChannel* socket;
	GHashTable* buddies;
	gchar* nickname;
	gchar* reqnick;
};

#define CYANCHAT_SESSION_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), CYANCHAT_TYPE_SESSION, CyanChatSessionPrivate))

G_DEFINE_TYPE(CyanChatSession, cyanchat_session, G_TYPE_OBJECT);

static void
cyanchat_session_init(CyanChatSession* self)
{
	self->priv = CYANCHAT_SESSION_GET_PRIVATE(self);

	self->priv->server = NULL;
	self->priv->nickname = NULL;
	self->priv->reqnick = NULL;
	self->priv->buddies = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify)cyanchat_buddy_free);
}

static void
cyanchat_session_finalize(GObject* object)
{
	CyanChatSession* s;

	g_return_if_fail(object != NULL);
	g_return_if_fail(CYANCHAT_IS_SESSION(object));

	s = CYANCHAT_SESSION(object);

	g_hash_table_destroy(s->priv->buddies);
	g_free(s->priv->server);
	g_free(s->priv->nickname);
	g_free(s->priv->reqnick);

	G_OBJECT_CLASS(cyanchat_session_parent_class)->finalize(object);
}

static void
cyanchat_session_class_init(CyanChatSessionClass* klass)
{
	GObjectClass* gobject_class = (GObjectClass*)klass;

	gobject_class->finalize = cyanchat_session_finalize;

	g_type_class_add_private(klass, sizeof(CyanChatSessionPrivate));
}





void cyanchat_session_send(CyanChatSession* s, const gchar* msg)
{
	g_return_if_fail(CYANCHAT_IS_SESSION(s));

	GIOStatus status;
	gsize written;
	gchar* buffer;
	GError* error = NULL;

	// FIXME - check if we are connected first

	buffer = g_strdup("%s\r\n", msg);

	status = g_io_channel_write_chars(s->priv->socket, buffer, -1, &written, &error);
}
