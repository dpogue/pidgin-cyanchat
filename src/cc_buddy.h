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


#ifndef _CYANCHAT_BUDDY_H
#define _CYANCHAT_BUDDY_H

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

/* Boilerplate */
#define CYANCHAT_TYPE_BUDDY              (cyanchat_buddy_get_type())
#define CYANCHAT_BUDDY(object)           (G_TYPE_CHECK_INSTANCE_CAST((object), CYANCHAT_TYPE_BUDDY, CyanChatBuddy))
#define CYANCHAT_IS_BUDDY(object)        (G_TYPE_CHECK_INSTANCE_TYPE((object), CYANCHAT_TYPE_BUDDY))
#define CYANCHAT_BUDDY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), CYANCHAT_TYPE_BUDDY, CyanChatBuddyClass))
#define CYANCHAT_IS_BUDDY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), CYANCHAT_TYPE_BUDDY))
#define CYANCHAT_BUDDY_GET_CLASS(object) (G_TYPE_INSTANCE_GET_CLASS((object), CYANCHAT_TYPE_BUDDY, CyanChatBuddyClass))
#define CYANCHAT_TYPE_BUDDY_LEVEL        (cyanchat_buddy_level_get_type())

typedef enum {
	CYANCHAT_LEVEL_USER = 0,
	CYANCHAT_LEVEL_ADMIN,
	CYANCHAT_LEVEL_ERROR,
	CYANCHAT_LEVEL_SERVER,
	CYANCHAT_LEVEL_GUEST
} CyanChatBuddyLevel;

/**
  CyanChatBuddy:

  todo docs
*/
typedef struct _CyanChatBuddy CyanChatBuddy;
struct _CyanChatBuddy {
	GObject parent;
};


/**
  CyanChatBuddyClass

  @parent: parent class
*/
typedef struct _CyanChatBuddyClass CyanChatBuddyClass;
struct _CyanChatBuddyClass {
	GObjectClass parent;
};

GType               cyanchat_buddy_level_get_type(void);
GType               cyanchat_buddy_get_type(void);
CyanChatBuddy*      cyanchat_buddy_new(const gchar* nickname, CyanChatBuddyLevel level, gboolean ignored);
CyanChatBuddy*      cyanchat_buddy_new_from_username(const gchar* username);
gchar*              cyanchat_buddy_get_nickname(CyanChatBuddy* b);
CyanChatBuddyLevel  cyanchat_buddy_get_level(CyanChatBuddy* b);
gboolean            cyanchat_buddy_get_ignored(CyanChatBuddy* b);

G_END_DECLS

#endif /* _CYANCHAT_BUDDY_H */
