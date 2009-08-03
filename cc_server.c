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
 * along with this program.	If not, see <http://www.gnu.org/licenses/>.
 */

#include "cc_server.h"
#include "cc_utils.h"
#include "cc_member.h"

/* Callback methods */
static void
cyanchat_server_input_cb(gpointer data, gint source, PurpleInputCondition cond)
{
	CyanChatServer* ccs;
	ccs = data;
	
	guint len;
	
	if (ccs->rxsize < ccs->rxoff + 1024) {
		ccs->rxsize += 1024;
		ccs->rxbuf = g_realloc(ccs->rxbuf, ccs->rxsize);
	}
	
	len = read(ccs->fd, ccs->rxbuf + ccs->rxoff, 1024 - 1);
	
	if (len < 0 && errno == EAGAIN) {
		return;
	} else if (len < 0) {
		/* Read Error */
		purple_debug_error("cyanchat", "Error! %s:%d -- Read Error\n", ccs->host, ccs->port);
		return;
	} else if (len == 0) {
		/* Server Disconnected */
		purple_debug_error("cyanchat", "Error! %s:%d -- Server Disconnected\n", ccs->host, ccs->port);
		return;
	}
	
	gchar* cur;
	gchar* end;
	
	ccs->account->pc->last_received = time(NULL);
	ccs->rxoff += len;
	ccs->rxbuf[ccs->rxoff] = '\0';
	
	cur = ccs->rxbuf;
	
	while ((cur < (ccs->rxbuf + ccs->rxoff)) && !*cur)
		cur++;
	
	while (cur < ccs->rxbuf + ccs->rxoff && ((end = strstr(cur, "\r\n")) || (end = strstr(cur, "\n")))) {
		guint step = (*end == '\r' ? 2 : 1);
		*end = '\0';
		purple_debug_misc("cyanchat", "%s:%d > %s\n", ccs->host, ccs->port, cur);
		cyanchat_parse_input(ccs, cur);
		cur = end + step;
	}
	
	/* Handle the left-over */
	if (cur != ccs->rxbuf + ccs->rxoff) {
		ccs->rxoff -= (cur - ccs->rxbuf);
		memmove(ccs->rxbuf, cur, ccs->rxoff);
	} else {
		ccs->rxoff = 0;
	}
}

static void
cyanchat_server_connect_cb(gpointer data, gint source, const gchar *error_message)
{
	CyanChatServer* ccs;
	ccs = data;
	
	if (error_message) {
		purple_debug_error("cyanchat", "Error connecting to %s:%d\n", ccs->host, ccs->port);
		purple_debug_error("cyanchat", "Error: %s\n", error_message);
		purple_serv_got_join_chat_failed(ccs->account->pc, ccs->data);
		cyanchat_server_destroy(ccs);
		return;
	}
	
	if (source < 0) {
		purple_debug_error("cyanchat", "Error connecting to %s:%d\n", ccs->host, ccs->port);
		purple_serv_got_join_chat_failed(ccs->account->pc, ccs->data);
		cyanchat_server_destroy(ccs);
		return;
	}
	
	ccs->fd = source;
	/* Handler for socket input */
	ccs->inputhandler = purple_input_add(ccs->fd, PURPLE_INPUT_READ, cyanchat_server_input_cb, ccs);
	
	/* cyanchat_connect(ccs); */
}

static void
insert_in_hash_table(gpointer key, gpointer value, gpointer user_data)
{
	GHashTable *hash_table = (GHashTable *)user_data;
	g_hash_table_insert(hash_table, g_strdup(key), g_strdup(value));
}

/* CyanChat Server functions */
void
cyanchat_server_connect(CyanChatAccount* cc, GHashTable* data)
{
	CyanChatServer* ccs;
	gchar* host;
	guint port;
	gchar* nick;
	gchar* serverid;
	
	host = g_hash_table_lookup(data, "server");
	port = atoi(g_hash_table_lookup(data, "port"));
	nick = g_hash_table_lookup(data, "nickname");
	
	serverid = g_strdup_printf("%s:%d", host, port);
	
	ccs = g_new0(CyanChatServer, 1);
	ccs->account = cc;
	ccs->fd = -1;
	ccs->id = -1;
	ccs->host = g_strdup(host);
	ccs->port = port;
	ccs->users = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, (GDestroyNotify)cyanchat_chat_member_free);
	ccs->data = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
	g_hash_table_foreach(data, insert_in_hash_table, ccs->data);
	
	if(nick) {
		ccs->reqnick = g_strdup(nick);
	} else {
		ccs->reqnick = g_strdup(purple_account_get_username(cc->pc->account));
	}
	
	g_hash_table_insert(cc->servers, serverid, ccs);
	
	if (purple_proxy_connect(cc->pc, cc->account, ccs->host, ccs->port, cyanchat_server_connect_cb, ccs) == NULL) {
		purple_debug_error("cyanchat", "Error! %s:%d -- Connection Problem\n", ccs->host, ccs->port);
		purple_serv_got_join_chat_failed(cc->pc, data);
		return;
	}
}

void
cyanchat_server_destroy(CyanChatServer* ccs)
{
	CyanChatAccount* cc;
	gchar* serverid;
	
	serverid = g_strdup_printf("%s:%d", ccs->host, ccs->port);
	
	cc = ccs->account;
	g_hash_table_remove(cc->servers, serverid);
	
	g_free(serverid);
	
	ccs = NULL;
}

void
cyanchat_server_free(CyanChatServer* ccs)
{
	if (ccs->id != -1) {
		cyanchat_chat_leave(ccs->account->pc, ccs->id);
	}
	
	if (ccs->inputhandler > 0) {
		purple_input_remove(ccs->inputhandler);
	}
	
	g_free(ccs->rxbuf);
	
	if (ccs->fd >= 0) {
		close(ccs->fd);
	}
	
	if (ccs->reqnick != NULL) {
		g_free(ccs->reqnick);
	}
	
	if (ccs->lobbymsg != NULL) {
		g_free(ccs->lobbymsg);
	}
	
	g_free(ccs->nickname);
	g_free(ccs->host);
	
	g_hash_table_destroy(ccs->data);
	g_hash_table_destroy(ccs->users);
	
	g_free(ccs);
}

CyanChatServer*
cyanchat_server_find_by_data(CyanChatAccount* cc, GHashTable* data)
{
	gchar* id;
	gchar* host;
	guint port;
	CyanChatServer* ccs;
	
	host = g_hash_table_lookup(data, "server");
	port = atoi(g_hash_table_lookup(data, "port"));
	
	id = g_strdup_printf("%s:%d", host, port);
	
	ccs = g_hash_table_lookup(cc->servers, id);
	
	g_free(id);
	
	return ccs;
}

gboolean
cyanchat_server_find_by_chat_id(gpointer key, gpointer value, gpointer user_data)
{
	CyanChatServer* ccs;
	gint id = GPOINTER_TO_INT(user_data);
	
	if (id == -1)
		return FALSE;
	
	ccs = (CyanChatServer*)value;
	
	if (ccs->id == -1)
		return FALSE;
	
	if (ccs->id == id)
		return TRUE;
	
	return FALSE;
	
}
