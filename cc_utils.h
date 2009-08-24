/**
 * @file cc_utils.h CyanChat Protocol Plugin
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

#ifndef CYANCHAT_UTILS_H
#define CYANCHAT_UTILS_H

#include "cyanchat.h"
#include "cc_account.h"
#include "cc_parser.h"
#include "cc_member.h"

typedef enum
{
	cLogIn	= 10,
	cLogOut	= 15,
	cSendPM	= 20,
	cSendMsg = 30,
	cConnect = 40,
	cAuth	= 45,
	cIgnore	= 70,

	sNoSet	= 10,
	sSet	= 11,
	sSentPM	= 21,
	sSentMsg = 31,
	sUsers	= 35,
	sLobby	= 40,
	sIgnore	= 70
} CyanChatMessageType;

typedef enum {
	kUser   = 0,
	kAdmin  = 1,
	kServer = 2,
	kError  = 3,
	kGuest  = 4
} CyanChatUserLevel;

/* CyanChat Send Functions */
gint cyanchat_send(guint fd, const gchar* msg);
void cyanchat_send_ping(CyanChatAccount* cc);
void cyanchat_send_join(CyanChatAccount* cc);
void cyanchat_send_part(CyanChatAccount* cc);
void cyanchat_send_private(CyanChatAccount* cc, const gchar* text, CyanChatMember* b);
void cyanchat_send_public(CyanChatAccount* cc, const gchar* text);
void cyanchat_send_connect(CyanChatAccount* cc);
void cyanchat_send_auth(CyanChatAccount* cc, const gchar* passwd);
void cyanchat_send_ignore(CyanChatAccount* cc, CyanChatMember* b);


#endif /* CYANCHAT_UTILS_H */
