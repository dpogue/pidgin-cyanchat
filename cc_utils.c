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

#include "cc_utils.h"
#include "cc_member.h"
#include "cc_server.h"

/* CyanChat Send Functions */
gint
cyanchat_write(CyanChatServer* ccs, const gchar* msg)
{
	purple_debug_misc("cyanchat", "%s:%d < %s\n", ccs->host, ccs->port, msg);
	return write(ccs->fd, msg, strlen(msg));
}

void
cyanchat_connect(CyanChatServer* ccs)
{
	gchar* buf;
	buf = g_strdup_printf("%d|1\r\n", cConnect);
	cyanchat_write(ccs, buf);
	g_free(buf);
}

void
cyanchat_join(CyanChatServer* ccs)
{
	gchar* buf;
	buf = g_strdup_printf("%d|%s\r\n", cLogIn, ccs->reqnick);
	cyanchat_write(ccs, buf);
	g_free(buf);
}

void
cyanchat_part(CyanChatServer* ccs)
{
	gchar* buf;
	buf = g_strdup_printf("%d\r\n", cLogOut);
	cyanchat_write(ccs, buf);
	g_free(buf);
}

void
cyanchat_public(CyanChatServer* ccs, const gchar* text)
{
	gchar* buf;
	buf = g_strdup_printf("%d|^1%s\r\n", cSendMsg, text);
	cyanchat_write(ccs, buf);
	g_free(buf);
}

void
cyanchat_private(CyanChatServer* ccs, const gchar* text, CyanChatMember* m)
{
	gchar* buf;
	buf = g_strdup_printf("%d|%d%s|^1%s\r\n", cSendPM, m->level, m->nickname, text);
	cyanchat_write(ccs, buf);
	g_free(buf);
}

void
cyanchat_ping(gpointer key, gpointer value, gpointer user_data)
{
	CyanChatServer* ccs;
	ccs = value;
	
	if (ccs->id != -1) {
		gchar* buf;
		buf = g_strdup_printf("%d||^1ping\r\n", cSendPM);
		cyanchat_write(ccs, buf);
		g_free(buf);
	}
}


/* CyanChat Parse function */
void
cyanchat_parse_input(CyanChatServer* ccs, gchar* input)
{
	gchar* token;
	static guint chatid = 1;
	
	token = strtok(input, "|");
	if (token) {
		guint cmd;
		cmd = atoi(token);
		
		CyanChatMember* b;
		
		switch(cmd) {
			case sNoSet:
				if (ccs->id == -1) {
					purple_serv_got_join_chat_failed(ccs->account->pc, ccs->data);
					
					gchar* roomname = g_strdup_printf("%s:%d", ccs->host, ccs->port);
					purple_notify_error(ccs->account->pc, roomname, _("Invalid Username"), _("Your username is not valid, please try a different name."));
					g_free(roomname);

					cyanchat_server_destroy(ccs);
					return;
					
				} else {
					serv_got_chat_in(ccs->account->pc, ccs->id, "ChatServer",
						PURPLE_MESSAGE_ERROR | PURPLE_MESSAGE_NO_LOG,
						_("Your username is not valid, please try a different name."), time(NULL));
				}
				
				g_free(ccs->reqnick);
				ccs->reqnick = NULL;
				
				break;
			case sSet:
				ccs->nickname = g_strdup(ccs->reqnick);
				g_free(ccs->reqnick);
				ccs->reqnick = NULL;
				
				/* Set up the chatroom if it isn't already */
				if (ccs->id == -1) {
					gchar* roomname = g_strdup_printf("%s:%d", ccs->host, ccs->port);
					
					ccs->id = chatid++;
					ccs->conv = serv_got_joined_chat(ccs->account->pc, ccs->id, roomname);
					
					g_free(roomname);
					
					ccs->lobbymsg = purple_markup_escape_text(ccs->lobbymsg, -1);
					
					serv_got_chat_in(ccs->account->pc, ccs->id, "ChatServer",
						PURPLE_MESSAGE_SYSTEM | PURPLE_MESSAGE_NO_LOG | PURPLE_MESSAGE_DELAYED,
						ccs->lobbymsg, time(NULL));
					
					g_free(ccs->lobbymsg);
					ccs->lobbymsg = NULL;
					
					/* add the chat users */
					GList* chatusers;
					PurpleConversationUiOps* ops;
					
					chatusers = NULL;
					
					g_hash_table_foreach(ccs->users,cyanchat_add_initial_users, &chatusers);
					purple_conv_chat_set_users(PURPLE_CONV_CHAT(ccs->conv), chatusers);
					
					ops = purple_conversation_get_ui_ops(ccs->conv);
					if (ops != NULL && ops->chat_add_users != NULL)
						ops->chat_add_users(ccs->conv, chatusers, FALSE);
					
				}
				
				purple_conv_chat_set_nick(PURPLE_CONV_CHAT(ccs->conv), ccs->nickname);
				
				break;
			case sSentPM:
				if (ccs->id) {
					token = strtok(NULL, "|"); /* Get the username */
					b = cyanchat_chat_member_new(token, ccs);
				
					token = strtok(NULL, "\n"); /* Get the message */
					gchar* msg;
				
					msg = purple_utf8_try_convert(token);
					if (msg == NULL) {
						msg = g_strdup(token);
					}
					
					msg = purple_markup_escape_text(msg, -1);
					
					if(strcmp(msg + 2, "!version") == 0) {
						gchar* buf;
						buf = g_strdup_printf("libpurple %d.%d.%d - prpl-cyanchat %s", PURPLE_MAJOR_VERSION, PURPLE_MINOR_VERSION, PURPLE_MICRO_VERSION, CYANCHAT_PLUGIN_VERSION);
						cyanchat_private(ccs, buf, b);
						g_free(buf);
					} else if(strcmp(msg + 2, "!ping") == 0) {
						gchar* buf;
						buf = g_strdup("plNetMsgPong :D");
						cyanchat_private(ccs, buf, b);
						g_free(buf);
					} else {
					    PurpleConversation *conv;
					    conv = purple_find_conversation_with_account(PURPLE_CONV_TYPE_IM, b->id, ccs->account->account);
					    if (conv == NULL)
					        conv = purple_conversation_new(PURPLE_CONV_TYPE_IM, ccs->account->account, b->id);
					    
						/*serv_got_im(ccs->account->pc, b->id, msg + 2, 0, time(NULL));*/
						purple_conv_im_write(PURPLE_CONV_IM(conv), b->nickname, msg + 2, 0, time(NULL));
					}
					
					g_free(msg);
					cyanchat_chat_member_free(b);
				}
				
				break;
			case sSentMsg:
				if (ccs->id) {
					token = strtok(NULL, "|"); /* Get the username */
					b = cyanchat_chat_member_new(token, ccs);
				
					token = strtok(NULL, "\n"); /* Get the message */
					gchar* msg;
				
					msg = purple_utf8_try_convert(token);
					if (msg == NULL) {
						msg = g_strdup(token);
					}
					
					msg = purple_markup_escape_text(msg, -1);
				
					if ((msg[0] == '^') && (msg[1] == '1'))
					{
						if (purple_account_get_bool(ccs->account->account, "bot_cleaner", TRUE) && (msg[2] == '*') && (msg[3] == '*') && (msg[4] == '*')) {
							serv_got_chat_in(ccs->account->pc, ccs->id, b->nickname,
								PURPLE_MESSAGE_DELAYED, msg + 2, time(NULL));
						} else {
							serv_got_chat_in(ccs->account->pc, ccs->id, b->nickname,
								0, msg + 2, time(NULL));
						}
						cyanchat_chat_member_free(b);
					}
					else if ((msg[0] == '^') && (msg[1] == '2') && !purple_account_get_bool(ccs->account->account, "userlist_magic", TRUE))
					{
						purple_conv_chat_add_user(PURPLE_CONV_CHAT(ccs->conv), b->nickname, b->username, cyanchat_chat_member_flags(b), TRUE);
						
						g_hash_table_insert(ccs->users, b->nickname, b);
					}
					else if ((msg[0] == '^') && (msg[1] == '3') && !purple_account_get_bool(ccs->account->account, "userlist_magic", TRUE))
					{
						purple_conv_chat_remove_user(PURPLE_CONV_CHAT(ccs->conv), b->nickname, msg + 2);
						
						g_hash_table_remove(ccs->users, b->nickname);
						cyanchat_chat_member_free(b);
					}
					
					g_free(msg);
				}
				
				break;
			case sUsers:
				token = strtok(NULL, "|"); /* Get the username */
				GList* userlist;
				time_t cur = time(NULL);
				guint curCount;
				guint newCount;
				
				userlist = NULL;
				
				while (token != NULL) {
					CyanChatMember* m;
					m = cyanchat_chat_member_new(token, ccs);
					m->lastupdate = cur;
					
					userlist = g_list_prepend(userlist, m);
					
					token = strtok(NULL, "|");
				}
				
				curCount = g_hash_table_size(ccs->users);
				newCount = g_list_length(userlist);
				
				GList* user;
				user = g_list_first(userlist);
				while (user != NULL) {
					CyanChatMember* m;
					m = (CyanChatMember*)user->data;
					
					CyanChatMember* ccm = g_hash_table_lookup(ccs->users, m->nickname);
					if (ccm != NULL) {
						/* If the user is already in the userlist, check for changed flags */
						if (ccm->level != m->level) {
							ccm->level = m->level;
							
							if (ccs->id != -1) {
								purple_conv_chat_user_set_flags(PURPLE_CONV_CHAT(ccs->conv), ccm->nickname, cyanchat_chat_member_flags(b));
							}
						}
						ccm->lastupdate = cur;
						
						user = user->next;
						
						userlist = g_list_remove(userlist, m);
					} else {
						/* If the user is not in the userlist and count is higher, they must have just joined */
						if ((ccs->id != -1) && (curCount < newCount)) {
							purple_conv_chat_add_user(PURPLE_CONV_CHAT(ccs->conv), m->nickname, m->username, cyanchat_chat_member_flags(m), TRUE);
							
							PurpleConversation* conv = purple_find_conversation_with_account(PURPLE_CONV_TYPE_IM, m->id, ccs->account->account);
							if (conv) {
								gchar* tmp;
								tmp = g_strdup_printf("%s is online.", m->nickname);
								purple_conversation_write(conv, m->nickname, tmp, PURPLE_MESSAGE_SYSTEM, time(NULL));
								g_free(tmp);
							}
						}
						
						if (curCount < newCount)
							g_hash_table_insert(ccs->users, m->nickname, m);
						
						user = user->next;
						
						if (curCount < newCount) {
							userlist = g_list_remove(userlist, m);
						}
					}
				}
				
				/* For each user in ccs->users, we need to check if the lastupdate time is equal to cur.
				   If it is, skip over the user (they were still on the ist in the latest update)
				   If it is not, either remove or rename the user. */
				GHashTableIter iter;
				gpointer key, value;
				
				g_hash_table_iter_init (&iter, ccs->users);
				while (g_hash_table_iter_next (&iter, &key, &value))
				{
					CyanChatMember* m;
					m = value;
					if (m->lastupdate == cur) {
						continue;
					}
					
					if (curCount > newCount) {
						if (ccs->id != -1) {
							if (purple_account_get_bool(ccs->account->account, "userlist_magic", TRUE))
							{
								purple_conv_chat_remove_user(PURPLE_CONV_CHAT(ccs->conv), m->nickname, NULL);
								
								PurpleConversation* conv = purple_find_conversation_with_account(PURPLE_CONV_TYPE_IM, m->id, ccs->account->account);
								if (conv) {
									gchar* tmp;
									tmp = g_strdup_printf("%s is offline.", m->nickname);
									purple_conversation_write(conv, m->nickname, tmp, PURPLE_MESSAGE_SYSTEM, time(NULL));
									g_free(tmp);
								}
							}
							else
							{
								PurpleConversationUiOps* ops;
								GList* newuser;
								
								ops = purple_conversation_get_ui_ops(ccs->conv);
								
								newuser = g_list_prepend(NULL, m->nickname);
						
								if (ops != NULL && ops->chat_remove_users != NULL)
									ops->chat_remove_users(ccs->conv, newuser);
						
								g_list_free(newuser);
							}
						}
					}
					else if (curCount == newCount) {
						if (userlist) {
							CyanChatMember* ccm;
							ccm = (CyanChatMember*)userlist->data;
							
							b = cyanchat_chat_member_new(ccm->username, ccs);
							
							cyanchat_chat_member_free(ccm);
							
							purple_conv_chat_rename_user(PURPLE_CONV_CHAT(ccs->conv), m->nickname, b->nickname);
							
							PurpleConversation* conv = purple_find_conversation_with_account(PURPLE_CONV_TYPE_IM, m->id, ccs->account->account);
							if (conv) {
								purple_conversation_set_name(conv, b->id);
								gchar* tmp;
								tmp = g_strdup_printf("%s is now known as %s.", m->nickname, b->nickname);
								purple_conversation_write(conv, m->nickname, tmp, PURPLE_MESSAGE_SYSTEM, time(NULL));
								g_free(tmp);
							}
							
							g_hash_table_replace(ccs->users, b->nickname, b);
						}
					}
					
					g_hash_table_iter_remove(&iter);
				}
				
				g_list_free(userlist);
				
				/* Send the "connect" command only once we hve a userlist */
				if ((!ccs->lobbymsg) && (ccs->id == -1)) {
					cyanchat_connect(ccs);
				}
				
				break;
			case sLobby:
				if (!ccs->lobbymsg) {
					ccs->lobbymsg = g_malloc(4096);
					*ccs->lobbymsg = '\0';
					
					cyanchat_join(ccs);
				}
				
				token = strtok(NULL, "\n"); /* Get the rest of the message */
				
				/* Protocol 0 - 40|message
				   Protocol 1 - 40|1message */
				if(g_ascii_isdigit(token[0])) {
					token++; /* Protocol 1 - strip the first character */
				}
				
				gchar* buffer;
				buffer = g_strdup_printf("%s\r\n%s", token, ccs->lobbymsg);
				strcpy(ccs->lobbymsg, buffer);
				g_free(buffer);
				
				break;
		}
	}
}

void
cyanchat_add_initial_users(gpointer key, gpointer value, gpointer user_data)
{
	CyanChatMember* b;
	PurpleConvChatBuddy* pcb;
	b = value;
	
	pcb = purple_conv_chat_cb_new(b->nickname, b->nickname, cyanchat_chat_member_flags(b));
	
	*(GList **)user_data = g_list_prepend(*(GList **)user_data, pcb);
}
