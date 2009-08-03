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

#include "cc_member.h"

/* CyanChatMember Functions */
CyanChatMember*
cyanchat_chat_member_new(const gchar* username, CyanChatServer* ccs)
{
	CyanChatMember* b;
	gchar* nick;
	gchar* utf8username;
	guint i = 0;
	
	utf8username = purple_utf8_try_convert(username);
	if (utf8username == NULL) {
		utf8username = g_strdup(username);
	}
	
	b = g_new0(CyanChatMember, 1);
	b->server = ccs;
	b->username = g_strdup(utf8username);
	b->level = g_ascii_digit_value(*utf8username);
	b->id = g_strdup_printf("%s@%s:%d", utf8username + 1, ccs->host, ccs->port);
	b->lastupdate = time(NULL);
	
	nick = g_malloc(24);
	while ((*(utf8username + i + 1) != ',') && (*(utf8username + i + 1) != '\0')) {
		*(nick + i) = *(utf8username + i + 1);
		i++;
	}
	*(nick + i) = '\0';
	
	b->nickname = g_strdup(nick);
	
	g_free(nick);
	g_free(utf8username);
	
	return b;
}

void
cyanchat_chat_member_free(CyanChatMember* b)
{
	if (b->nickname)
		g_free(b->nickname);
	
	if (b->id)
		g_free(b->id);
	
	if (b->username)
		g_free(b->username);
	
	g_free(b);
}

PurpleConvChatBuddyFlags
cyanchat_chat_member_flags(CyanChatMember* b)
{
	PurpleConvChatBuddyFlags flags;
	
	flags = PURPLE_CBFLAGS_NONE;
	
	if (b->level == kAdmin) {
		flags = PURPLE_CBFLAGS_FOUNDER;
	}
	
	if (b->level == kGuest) {
		flags = PURPLE_CBFLAGS_OP;
	}
	
	return flags;
}
