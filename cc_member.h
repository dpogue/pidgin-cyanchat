/**
 * @file cc_member.h CyanChat Protocol Plugin
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

#ifndef CYANCHAT_MEMBER_H
#define CYANCHAT_MEMBER_H

#include <glib.h>
#include <glib-object.h>

/** @copydoc _CyanChatMemberClass */
typedef struct _CyanChatMemberClass CyanChatMemberClass;
/** @copydoc _CyanChatMember */
typedef struct _CyanChatMember CyanChatMember;

#include "cyanchat.h"

G_BEGIN_DECLS

#define CYANCHAT_TYPE_MEMBER            (cyanchat_member_get_type())
#define CYANCHAT_MEMBER(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), CYANCHAT_TYPE_MEMBER, CyanChatMember))
#define CYANCHAT_MEMBER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), CYANCHAT_TYPE_MEMBER, CyanChatMemberClass))
#define CYANCHAT_IS_MEMBER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), CYANCHAT_TYPE_MEMBER))
#define CYANCHAT_IS_MEMBER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), CYANCHAT_TYPE_MEMBER))
#define CYANCHAT_MEMBER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), CYANCHAT_TYPE_MEMBER, CyanChatMemberClass))

/**************************************************************************/
/** @name CyanChat Member                                                 */
/**************************************************************************/
/*@{*/

/**
 * Gets the member's GType.
 *
 * @return The member's GType.
 */
GType cyanchat_member_get_type(void);

/**
 * Create a new #CyanChatMember.
 *
 * @return A #CyanChatMember.
 */
CyanChatMember* cyanchat_member_new(const gchar* username);

/**
 * Get the username of a #CyanChatMember.
 *
 * @param b The member whose username to get.
 *
 * @return The member's username, or NULL.
 */
const gchar* cyanchat_member_get_username(CyanChatMember* b);

/**
 * Set the username of a #CyanChatMember.
 *
 * @param b The member whose username to set.
 * @param name The username to set.
 */
void cyanchat_member_set_username(CyanChatMember* b, const gchar* name);

/**
 * Get the nickname of a #CyanChatMember.
 *
 * @param b The member whose nickname to get.
 *
 * @return The member's nickname, or NULL.
 */
const gchar* cyanchat_member_get_nickname(CyanChatMember* b);

/**
 * Set the nickname of a #CyanChatMember.
 *
 * @param b The member whose nickname to set.
 * @param nick The nickname to set.
 */
void cyanchat_member_set_nickname(CyanChatMember* b, const gchar* nick);

/**
 * Get the #CyanChatUserLevel of a #CyanChatMember.
 *
 * @param b The member whose level to get.
 *
 * @return The member's CyanChatUserLevel, or 0.
 */
CyanChatUserLevel cyanchat_member_get_level(CyanChatMember* b);

/**
 * Get the #PurpleConvChatBuddyFlags of a #CyanChatMember.
 *
 * @param b The member whose level flags to get.
 *
 * @return The member's #PurpleConvChatBuddyFlags, or 0.
 */
PurpleConvChatBuddyFlags cyanchat_member_get_flags(CyanChatMember* b);

/**
 * Set the #CyanChatUserLevel of a #CyanChatMember.
 *
 * @param b The member whose level to set.
 * @param l The #CyanChatUserLevel to set.
 */
void cyanchat_member_set_level(CyanChatMember* b, CyanChatUserLevel l);

/*}@*/

G_END_DECLS

#endif /* CYANCHAT_MEMBER_H */
