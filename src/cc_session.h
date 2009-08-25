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

#ifndef _CYANCHAT_SESSION_H
#define _CYANCHAT_SESSION_H

#include <glib.h>
#include <glib-object.h>

#include "cc_buddy.h"

G_BEGIN_DECLS

/* Boilerplate */
#define CYANCHAT_TYPE_SESSION (cyanchat_session_get_type())
#define CYANCHAT_SESSION(object) (G_TYPE_CHECK_INSTANCE_CAST((object), CYANCHAT_TYPE_SESSION, CyanChatSession))
#define CYANCHAT_IS_SESSION(object) (G_TYPE_CHECK_INSTANCE_TYPE((object), CYANCHAT_TYPE_SESSION))
#define CYANCHAT_SESSION_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), CYANCHAT_TYPE_SESSION, CyanChatSessionClass))
#define CYANCHAT_IS_SESSION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), CYANCHAT_TYPE_SESSION))
#define CYANCHAT_SESSION_GET_CLASS(object) (G_TYPE_INSTANCE_GET_CLASS((object), CYANCHAT_TYPE_SESSION, CyanChatSessionClass))

typedef struct _CyanChatSessionPrivate CyanChatSessionPrivate;

/**
  CyanChatSession:

  todo docs
*/
typedef struct _CyanChatSession CyanChatSession;
struct _CyanChatSession {
	GObject parent;
	CyanChatSessionPrivate* priv;
};


/**
  CyanChatSessionClass:

  @parent: parent class

  @name_accepted: React to an "11" message
  @name_denied: React to a "10" message
  @received_im: React to an incoming private message
  @received_chat: React to an incoming public chat message
  @add_chat_buddy: Add a new user to the chat buddy list
  @remove_chat_buddy: Remove a user from the chat buddy list
  @change_chat_buddy: Change a chat buddy's nickname or level
  @lobby_message: React to a "40" message
  @ignore_chat_buddy: Ignore a chat buddy
  @unhandled_cmd: React to an unexpected message type
  @get_version: The !version response string
*/
typedef struct _CyanChatSessionClass CyanChatSessionClass;
struct _CyanChatSessionClass {
	GObjectClass parent;

	/* Virtual Functions */
	void (*name_accepted)(CyanChatSession* s);
	void (*name_denied)(CyanChatSession* s);
	void (*received_im)(CyanChatSession* s, CyanChatBuddy* b, gchar* msg);
	void (*recevied_chat)(CyanChatSession* s, CyanChatBuddy* b, gchar* msg);
	void (*add_chat_buddy)(CyanChatSession* s, CyanChatBuddy* b);
	void (*remove_chat_buddy)(CyanChatSession* s, CyanChatBuddy* b);
	void (*change_chat_buddy)(CyanChatSession* s, CyanChatBuddy* a, CyanChatBuddy* b);
	void (*lobby_message)(CyanChatSession* s, gchar* msg);
	void (*ignore_chat_buddy)(CyanChatSession* s, CyanChatBuddy* b);
	void (*unhandled_cmd)(CyanChatSession* s, gchar* rawcmd);

	G_GNUC_CONST gchar* (*get_version)();
};

GType cyanchat_session_get_type(void);
CyanChatSession* cyanchat_session_new(const gchar* server, const guint port);

G_GNUC_CONST gchar* cyanchat_session_get_name(CyanChatSession* s);
G_GNUC_CONST gchar* cyanchat_session_get_nickname(CyanChatSession* s);

void cyanchat_session_connect(CyanChatSession* s);
void cyanchat_session_disconnect(CyanChatSession* s);

/* msg should *NOT* be terminated with \r\n\0 as that is added here */
void cyanchat_session_send(CyanChatSession* s, const gchar* msg);

G_END_DECLS

#endif /* _CYANCHAT_SESSION_H */
