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

#include <glib.g>
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

#define CYANCHAT_SESSION_NULL (0)

/**
  CyanChatSession:

  todo docs
*/
typedef struct _CyanChatSession CyanChatSession;
struct _CyanChatSession {
	GObject parent;
};


/**
  CyanChatSessionClass:

  @parent: parent class

  @name_accepted: React to an "11" message
  @name_denied: React to a "10" message
  @received_im: React to an incoming private message
  @received_chat: React to an incoming public chat message
*/
typedef struct _CyanChatSessionClass CyanChatSessionClass;
struct _CyanChatSessionClass {
	GObjectClass parent;

	/* Virtual Functions */
	void (*name_accepted)(CyanChatSession* ccs);
	void (*name_denied)(CyanChatSession* ccs);
	void (*received_im)(CyanChatSession* css, CyanChatBuddy* b, gchar* msg);
};

G_END_DECLS

#endif /* _CYANCHAT_SESSION_H */
