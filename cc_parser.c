/**
 * @file cc_parser.c CyanChat Protocol Plugin
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

#include "cc_parser.h"

GStaticMutex cyanchat_mutex = G_STATIC_MUTEX_INIT;
guint cyanchat_blocking = 0;
CyanChatUserlist* cyanchat_userlist = NULL;

CyanChatMessage*
cyanchat_message_new(const gchar* msg, time_t time)
{
	CyanChatMessage* m;
	
	m = g_new0(CyanChatMessage, 1);
	m->message = g_strdup(msg);
	m->time = time;
	
	return m;
}

void
cyanchat_message_free(CyanChatMessage* m)
{
	if(m->message)
		g_free(m->message);
	
	g_free(m);
}

void
cyanchat_userlist_parse(CyanChatTransport* t)
{
	if(cyanchat_userlist == NULL) {
		cyanchat_userlist = g_new0(CyanChatUserlist, 1);
		cyanchat_userlist->size = 0;
		cyanchat_userlist->strdata = g_strdup_printf("%s\r\n", sUsers);
		cyanchat_userlist->members = NULL;
	}

	gchar* list;

	list = cyanchat_transport_get_message(t);
	
	if(strcmp(cyanchat_userlist->strdata, list) == 0)
		return;
	
	CyanChatUserlist* newlist;
	gchar* token;
	GList* added = NULL;
	
	newlist = g_new0(CyanChatUserlist, 1);
	newlist->strdata = list;
	
	token = strtok(list, "|");
	if(atoi(token) == sUsers) {
		token = strtok(NULL, "|");
		while(token) {
			CyanChatMember* b;
			
			b = cyanchat_member_new(token);
			newlist->members = g_list_prepend(newlist->members, b);
			
			if(g_list_find(cyanchat_userlist->members, b)) {
				cyanchat_userlist->members = g_list_remove(cyanchat_userlist->members, b);
			} else {
				added = g_list_prepend(added, b);
			}
			
			newlist->size++;
		}
	}
	
	if(newlist->size > cyanchat_userlist->size) {
		/* Users joined... they are stored in Glist* added */
		guint i = 0;
		for(; i < g_list_length(added); i++) {
			CyanChatMember* b;
			CyanChatTransport* t2;
			b = g_list_nth_data(added, i);
			t2 = cyanchat_transport_new(cyanchat_transport_get_account(t), b, NULL, NULL);

			purple_timeout_add(1, (GSourceFunc)cyanchat_account_add_members, t2);
		}
	} else if(newlist->size < cyanchat_userlist->size) {
		/* Users parted... they are stored in GList* cyanchat_userlist->members */
	} else if(newlist->size == cyanchat_userlist->size) {
		/* Users renamed or leveled.
		   Things get really crazy right about now... */
	}
	
	g_list_free(added);
	
	g_free(cyanchat_userlist->strdata);
	g_list_free(cyanchat_userlist->members);
	g_free(cyanchat_userlist);
	
	cyanchat_userlist = newlist;
	
	g_static_mutex_lock(cyanchat_mutex);
	if (cyanchat_block > 0)
		cyanchat_block--;
	g_static_mutex_unlock(cyanchat_mutex);

	g_free(t);
}

gboolean
cyanchat_parser_quick(CyanChatTransport* t)
{
	CyanChatAccount* cc;
	CyanChatMessage* m;

	cc = cyanchat_transport_get_account(t);
	m = cyanchat_transport_get_message(t);

	g_return_val_if_fail(cyanchat_account_get_req_nick(cc) != NULL, TRUE);

	if(g_str_has_prefix(m->message, "10\0")) {
		purple_timeout_add(1, (GSourceFunc)cyanchat_callback_pong, cc);
		g_free(t);
		return FALSE;
	}
	
	if(g_str_has_prefix(m->message, "35\0")) {
		g_thread_create((GThreadFunc)cyanchat_userlist_parse, (gpointer)t, FALSE, NULL);
		return FALSE;
	}
	
	g_free(t);
	return TRUE;
}
