/**
 * @file cyanchat.c CyanChat Protocol Plugin
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

#include "cyanchat.h"
#include "cc_account.h"
#include "cc_utils.h"
#include "cc_server.h"
#include "cc_callback.h"

/* libpurple functions */
const gchar*
cyanchat_list_icon(PurpleAccount* a, PurpleBuddy* b)
{
	return CYANCHAT_PLUGIN_NAME;
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
	return NULL;
}

void
cyanchat_login(PurpleAccount* account)
{
	CyanChatAccount* cc;
	PurpleConnection* pc;
	pc = purple_account_get_connection(account);
	
	purple_connection_update_progress(pc, _("Connecting"), 0, 3);
	
	pc->flags =	PURPLE_CONNECTION_NO_BGCOLOR |
		PURPLE_CONNECTION_NO_URLDESC |
		PURPLE_CONNECTION_NO_FONTSIZE |
		PURPLE_CONNECTION_NO_IMAGES |
		PURPLE_CONNECTION_NO_NEWLINES;
	
	cc = cyanchat_account_new();
	cyanchat_account_set_connection(cc, pc);
	cyanchat_account_set_host(cc, purple_account_get_string(account, "server", "cho.cyan.com"));
	cyanchat_account_set_port(cc, purple_account_get_int(account, "port", 1812));
	cyanchat_account_set_req_nick(cc, purple_account_get_username(account));
	
	pc->proto_data = cc;
	
	if (purple_proxy_connect(pc, account, cyanchat_account_get_host(cc), cyanchat_account_get_port(cc), cyanchat_callback_login, cc) == NULL) {
		purple_connection_error_reason(pc, PURPLE_CONNECTION_ERROR_NETWORK_ERROR, _("Connection problem"));
		return;
	}
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
cyanchat_ping(PurpleConnection* pc)
{
	static gboolean ping = FALSE;
	
	ping = !ping;
	
	if(ping) {
		CyanChatAccount* cc;
	
		cc = CYANCHAT_ACCOUNT(pc->proto_data);
	
		if ((cyanchat_account_get_timeout(cc) == 0) && (cyanchat_account_get_req_nick(cc) == NULL)) {
			cyanchat_send_ping(cc);
			cyanchat_account_set_timeout(cc, 181, (GSourceFunc)cyanchat_callback_ping);
		}
	}
}
