/**
 * @file cc_callback.h CyanChat Protocol Plugin
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

#ifndef CYANCHAT_CALLBACK_H
#define CYANCHAT_CALLBACK_H

#include "cyanchat.h"
#include "cc_account.h"

gboolean cyanchat_callback_ping(CyanChatAccount* cc);
gboolean cyanchat_callback_pong(CyanChatAccount* cc);
void cyanchat_callback_login(gpointer data, gint source, const gchar *error_message);
void cyanchat_callback_input(gpointer data, gint source, PurpleInputCondition cond);

#endif /* CYANCHAT_CALLBACK_H */
