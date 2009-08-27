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

#include "cc_purple.h"

#include <core.h>

/* Private Data */
struct _CyanChatSessionPurplePrivate {
	PurpleAccount* account;
	PurpleConnection* pconn;
	PurpleConversation* conv;
	guint chat_id;
	guint ping;
};

#define CYANCHAT_SESSION_PURPLE_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), CYANCHAT_TYPE_SESSION_PURPLE, CyanChatSessionPurplePrivate))

G_DEFINE_TYPE(CyanChatSessionPurple, cyanchat_session_purple, CYANCHAT_TYPE_SESSION);


G_GNUC_CONST gchar*
cyanchat_session_purple_get_version()
{
	return g_strdup("libCyanChat version 2.0 running on %s.", purple_core_get_version());
}
