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

#include "cc_utils.h"

void
cyanchat_utils_join(CyanChatSession* s, const gchar* nickname)
{
	gchar* msg;

	msg = g_strdup_printf("%d|%s", CYANCHAT_C_JOIN, nickname);
	cyanchat_session_send(s, msg);

	g_free(msg);
}

void
cyanchat_utils_part(CyanChatSession* s)
{
	gchar* msg;

	msg = g_strdup_printf("%d", CYANCHAT_C_PART);
	cyanchat_session_send(s, msg);

	g_free(msg);
}

void
cyanchat_utils_im(CyanChatSession* s, CyanChatBuddy* b, const gchar* text)
{
	gchar* msg;

	msg = g_strdup_printf("%d|%d%s|^0%s", CYANCHAT_C_IM, cyanchat_buddy_get_level(b), cynchat_buddy_get_nickname(b), text);
	cyanchat_session_send(s, msg);
	
	g_free(msg);
}
