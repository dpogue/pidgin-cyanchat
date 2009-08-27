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

#ifndef _CYANCHAT_PURPLE_H
#define _CYANCHAT_PURPLE_H

#include <glib.h>
#include <glib-object.h>
#include "cc_session.h"

G_BEGIN_DECLS

/* Boilerplate */
#define CYANCHAT_TYPE_SESSION_PURPLE (cyanchat_session_purple_get_type())
#define CYANCHAT_SESSION_PURPLE(object) (G_TYPE_CHECK_INSTANCE_CAST((object), CYANCHAT_TYPE_SESSION_PURPLE, CyanChatSessionPurple))
#define CYANCHAT_IS_SESSION_PURPLE(object) (G_TYPE_CHECK_INSTANCE_TYPE((object), CYANCHAT_TYPE_SESSION_PURPLE))
#define CYANCHAT_SESSION_PURPLE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), CYANCHAT_TYPE_SESSION_PURPLE, CyanChatSessionPurpleClass))
#define CYANCHAT_IS_SESSION_PURPLE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), CYANCHAT_TYPE_SESSION_PURPLE))
#define CYANCHAT_SESSION_PURPLE_GET_CLASS(object) (G_TYPE_INSTANCE_GET_CLASS((object), CYANCHAT_TYPE_SESSION_PURPLE, CyanChatSessionPurpleClass))

typedef struct _CyanChatSessionPurplePrivate CyanChatSessionPurplePrivate;

/**
  CyanChatSessionPurple:
  
  CyanChatSession extension which implements libpurple code.
*/
typedef struct _CyanChatSessionPurple CyanChatSessionPurple;
struct _CyanChatSessionPurple {
	CyanChatSession parent;
	CyanChatSessionPurplePrivate* priv;
};

/**
  CyanChatSessionPurpleClass:

  @parent: parent class
*/
typedef struct _CyanChatSessionPurpleClass CyanChatSessionPurpleClass;
struct _CyanChatSessionPurpleClass {
	CyanChatSessionClass parent;
};

GType cyanchat_session_purple_get_type(void);

void cyanchat_session_purple_name_accepted(CyanChatSession* s);
void cyanchat_session_purple_name_denied(CyanChatSession* s);
void cyanchat_session_purple_received_im(CyanChatSession* s, CyanChatBuddy* b, const gchar* msg);
void cyanchat_session_purple_received_chat(CyanChatSession* s, CyanChatBuddy* b, const gchar* msg);
void cyanchat_session_purple_add_chat_buddy(CyanChatSession* s, CyanChatBuddy* b);
void cyanchat_session_purple_remove_chat_buddy(CyanChatSession* s, CyanChatBuddy* b);
void cyanchat_session_purple_change_chat_buddy(CyanChatSession* s, CyanChatBuddy* a, CyanChatBuddy* b);
void cyanchat_session_purple_lobby_message(CyanChatSession* s, const gchar* msg);
void cyanchat_session_purple_ignore_chat_buddy(CyanChatSession* s, CyanChatBuddy* b);
void cyanchat_session_purple_unhandled_cmd(CyanChatSession* s, const gchar* rawcmd);
G_GNUC_CONST gchar* cyanchat_session_purple_get_version(void);

G_END_DECLS

#endif /* _CYANCHAT_PURPLE_H */
