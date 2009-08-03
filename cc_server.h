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

#ifndef CYANCHAT_SERVER_H
#define CYANCHAT_SERVER_H

#include "cyanchat.h"

/* CyanChat Server functions */
void cyanchat_server_connect(CyanChatAccount* cc, GHashTable* data);
void cyanchat_server_destroy(CyanChatServer* ccs);
void cyanchat_server_free(CyanChatServer* ccs);

CyanChatServer* cyanchat_server_find_by_data(CyanChatAccount* cc, GHashTable* data);
gboolean cyanchat_server_find_by_chat_id(gpointer key, gpointer value, gpointer user_data);

#endif /* CYANCHAT_SERVER_H */
