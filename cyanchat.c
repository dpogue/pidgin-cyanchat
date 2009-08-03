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

#include "cyanchat.h"
#include "cc_utils.h"
#include "cc_server.h"

/* libpurple functions */
const gchar*
cyanchat_list_icon(PurpleAccount* a, PurpleBuddy* b)
{
	return "cyanchat";
}

GList*
cyanchat_status_types(PurpleAccount* account)
{
	PurpleStatusType* type;
	GList* types = NULL;

	type = purple_status_type_new(PURPLE_STATUS_AVAILABLE, NULL, NULL, TRUE);
	types = g_list_append(types, type);

	type = purple_status_type_new(PURPLE_STATUS_UNAVAILABLE, NULL, NULL, TRUE);
	types = g_list_append(types, type);

	type = purple_status_type_new(PURPLE_STATUS_AWAY, NULL, NULL, TRUE);
	types = g_list_append(types, type);

	type = purple_status_type_new(PURPLE_STATUS_OFFLINE, NULL, NULL, TRUE);
	types = g_list_append(types, type);

	return types;
}

GList* 
cyanchat_chat_info(PurpleConnection* pc)
{
	GList *m = NULL;
	struct proto_chat_entry *pce;

	pce = g_new0(struct proto_chat_entry, 1);
	pce->label = _("_Server:");
	pce->identifier = "server";
	pce->required = TRUE;
	m = g_list_append(m, pce);
	
	pce = g_new0(struct proto_chat_entry, 1);
	pce->label = _("_Port:");
	pce->identifier = "port";
	pce->required = TRUE;
	m = g_list_append(m, pce);

	pce = g_new0(struct proto_chat_entry, 1);
	pce->label = _("_Nickname:");
	pce->identifier = "nickname";
	pce->required = TRUE;
	m = g_list_append(m, pce);

	return m;
}

GHashTable*
cyanchat_chat_info_defaults(PurpleConnection* pc, const gchar* chat_name)
{
	GHashTable* defaults;
	gchar* chat;

	defaults = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, g_free);

	g_hash_table_insert(defaults, "nickname", g_strdup(purple_account_get_username(pc->account)));
	
	if (chat_name != NULL) {
		chat = g_strdup(chat_name);
		gchar* tok = strtok(chat, ":");
		
		if (tok != NULL) {
			g_hash_table_insert(defaults, "server", g_strdup(tok));
			
			tok = strtok(NULL, "\n");
			g_hash_table_insert(defaults, "port", g_strdup(tok));
		}
		
		g_free(chat);
	}

	return defaults;
}

void
cyanchat_login(PurpleAccount* account)
{
	CyanChatAccount* cc;
	PurpleConnection* pc;
	pc = purple_account_get_connection(account);
	
	pc->flags =	PURPLE_CONNECTION_NO_BGCOLOR |
		PURPLE_CONNECTION_NO_URLDESC |
		PURPLE_CONNECTION_NO_FONTSIZE |
		PURPLE_CONNECTION_NO_IMAGES |
		PURPLE_CONNECTION_NO_NEWLINES;
	
	/* Create account and initialize state */
	cc = g_new0(CyanChatAccount, 1);
	cc->account = account;
	cc->pc = pc;
	cc->servers = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify)cyanchat_server_free);
	
	account->gc->proto_data = cc;
	
	purple_connection_set_state(pc, PURPLE_CONNECTED);
}

void
cyanchat_close(PurpleConnection* pc)
{
	CyanChatAccount* cc;
	
	cc = pc->proto_data;
	
	g_hash_table_destroy(cc->servers);
	
	g_free(cc);
}

gint
cyanchat_send_im(PurpleConnection* pc, const gchar* who, const gchar* message, PurpleMessageFlags flags)
{
	CyanChatAccount* cc;
	gchar* user;
	gchar* userid;
	gchar* serverid;
	CyanChatServer* ccs;
	CyanChatMember* b;
	gchar* input;
	gchar* text;
	
	cc = pc->proto_data;
	user = g_strdup(who);
	userid = strtok(user, ",");
	strtok(NULL, "@");
	serverid = strtok(NULL, "\0");
	
	ccs = g_hash_table_lookup(cc->servers, serverid);
	
	if (ccs == NULL) {
		purple_debug_error("cyanchat", "Error! We tried to chat with a buddy on a server that doesn't exist!");
		return -EINVAL;
	}
	
	b = g_hash_table_lookup(ccs->users, userid);
	
	input = purple_markup_strip_html(message);
	purple_str_strip_char(input, '\r');
	purple_str_strip_char(input, '\n');
	text = purple_unescape_html(input);
	
	cyanchat_private(ccs, text, b);
	
	g_free(text);
	g_free(input);
	g_free(user);
	
	return strlen(message);
}

void
cyanchat_chat_join(PurpleConnection* pc, GHashTable* data)
{
	CyanChatAccount* cc;
	
	cc = pc->proto_data;

	if (!g_hash_table_lookup(data, "server") || !g_hash_table_lookup(data, "port"))
		return;
	
	if (cyanchat_server_find_by_data(cc, data))
		return;
	
	cyanchat_server_connect(cc, data);
}

gchar*
cyanchat_chat_name(GHashTable* data)
{
	gchar* host;
	gchar* port;
	gchar* id;

	host = g_hash_table_lookup(data, "server");
	port = g_hash_table_lookup(data, "port");
	
	id = g_strdup_printf("%s:%d", host, atoi(port));
	
	return id;
}

void
cyanchat_chat_leave(PurpleConnection* pc, gint id)
{
	CyanChatAccount* cc;
	CyanChatServer* ccs;
	
	cc = pc->proto_data;
	
	ccs = g_hash_table_find(cc->servers, cyanchat_server_find_by_chat_id, GINT_TO_POINTER(id));
	
	if (ccs == NULL) {
		purple_debug_error("cyanchat", "Error! We tried to leave a non-existent chatroom!");
		return;
	}
	
	cyanchat_part(ccs);
	
	ccs->id = -1;
	
	cyanchat_server_destroy(ccs);
}

gint
cyanchat_chat_send(PurpleConnection* pc, gint id, const gchar* what, PurpleMessageFlags flags)
{
	CyanChatAccount* cc;
	CyanChatServer* ccs;
	gchar* input;
	gchar* text;
	
	cc = pc->proto_data;
	
	ccs = g_hash_table_find(cc->servers, cyanchat_server_find_by_chat_id, GINT_TO_POINTER(id));
	
	if (ccs == NULL) {
		purple_debug_error("cyanchat", "Error! We tried to send to a non-existent chatroom!");
		return -EINVAL;
	}
	
	input = purple_markup_strip_html(what);
	purple_str_strip_char(input, '\r');
	purple_str_strip_char(input, '\n');
	text = purple_unescape_html(input);
	
	cyanchat_public(ccs, text);
	
	g_free(text);
	g_free(input);
	
	return strlen(what);
}

void
cyanchat_keepalive(PurpleConnection* pc)
{
	CyanChatAccount* cc;
	
	cc = pc->proto_data;
	
	g_hash_table_foreach(cc->servers, cyanchat_ping, cc);
}

gchar*
cyanchat_cb_real_id(PurpleConnection* pc, gint id, const gchar* who)
{
	CyanChatAccount* cc;
	CyanChatServer* ccs;
	CyanChatMember* b;
	
	cc = pc->proto_data;
	
	ccs = g_hash_table_find(cc->servers, cyanchat_server_find_by_chat_id, GINT_TO_POINTER(id));
	
	if (ccs == NULL) {
		purple_debug_error("cyanchat", "Error! We tried to get info about a buddy in a non-existent chatroom!");
		return NULL;
	}
	
	b = g_hash_table_lookup(ccs->users, who);
	
	if (b == NULL) {
		return NULL;
	}
	
	return g_strdup(b->id);
}
