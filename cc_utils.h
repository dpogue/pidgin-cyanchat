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

#ifndef CYANCHAT_UTILS_H
#define CYANCHAT_UTILS_H

#include "cyanchat.h"

typedef enum
{
	cLogIn	= 10,
	cLogOut	= 15,
	cSendPM	= 20,
	cSendMsg = 30,
	cConnect = 40,
	cIgnore	= 70,

	sNoSet	= 10,
	sSet	= 11,
	sSentPM	= 21,
	sSentMsg = 31,
	sUsers	= 35,
	sLobby	= 40,
	sIgnore	= 70
} CyanChatMessageType;

/* CyanChat Send Functions */
gint cyanchat_write(CyanChatServer* ccs, const gchar* msg);
void cyanchat_connect(CyanChatServer* ccs);
void cyanchat_join(CyanChatServer* ccs);
void cyanchat_part(CyanChatServer* ccs);
void cyanchat_public(CyanChatServer* ccs, const gchar* text);
void cyanchat_private(CyanChatServer* ccs, const gchar* text, CyanChatMember* m);
void cyanchat_ping(gpointer key, gpointer value, gpointer user_data);

/* CyanChat Parse function */
void cyanchat_parse_input(CyanChatServer* ccs, gchar* input);
void cyanchat_add_initial_users(gpointer key, gpointer value, gpointer user_data);

#endif /* CYANCHAT_UTILS_H */
