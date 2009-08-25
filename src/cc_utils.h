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

#ifndef _CYANCHAT_UTILS_H
#define _CYANCHAT_UTILS_H

#include "cc_session.h"

typedef enum {
	CYANCHAT_C_JOIN = 10,
	CYANCHAT_C_PART = 15,
	CYANCHAT_C_IM = 20,
	CYANCHAT_C_CHAT = 30,
	CYANCHAT_C_CONNECT = 40,
	CYANCHAT_C_AUTH = 45,
	CYANCHAT_C_IGNORE = 70,

	CYANCHAT_S_NAMENOSET = 10,
	CYANCHAT_S_NAMESET = 11,
	CYANCHAT_S_IM = 21,
	CYANCHAT_S_CHAT = 31,
	CYANCHAT_S_USERS = 35,
	CYANCHAT_S_LOBBY = 40,
	CYANCHAT_S_IGNORE = 70
} CyanChatCommands;

void cyanchat_utils_join(CyanChatSession* s, const gchar* nickname);
void cyanchat_utils_part(CyanChatSession* s);
void cyanchat_utils_im(CyanChatSession* s, CyanChatBuddy* b, const gchar* msg);
void cyanchat_utils_chat(CyanChatSession* s, const gchar* msg);
void cyanchat_utils_connect(CyanChatSession* s);
void cyanchat_utils_auth(CyanChatSession* s, const gchar* passwd);
void cyanchat_utils_ignore(CyanChatSession* s, CyanChatBuddy* b);

#endif /* _CYANCHAT_UTILS_H */
