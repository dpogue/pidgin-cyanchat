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

#ifndef CYANCHAT_MEMBER_H
#define CYANCHAT_MEMBER_H

#include "cyanchat.h"

/* CyanChatMember Functions */
CyanChatMember* cyanchat_chat_member_new(const gchar* username, CyanChatServer* ccs);
void cyanchat_chat_member_free(CyanChatMember* b);

PurpleConvChatBuddyFlags cyanchat_chat_member_flags(CyanChatMember* b);

#endif /* CYANCHAT_MEMBER_H */
