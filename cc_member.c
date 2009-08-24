/**
 * @file cc_member.c CyanChat Protocol Plugin
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

#include "cc_member.h"
#include "cc_utils.h"

/** @copydoc _CyanChatMemberPrivate */
typedef struct _CyanChatMemberPrivate CyanChatMemberPrivate;

/** The CyanChat Member class. */
struct _CyanChatMemberClass
{
	GObjectClass parent_class;	/**< The parent class. */
};

/** The CyanChat Member's data. */
struct _CyanChatMember
{
	GObject parent;	/**< The parent of this member. */
	CyanChatMemberPrivate* priv;	/**< Private data for the member. */
};

/** The CyanChat Member's private data. */
struct _CyanChatMemberPrivate
{
	CyanChatUserLevel level;	/* The chat user level */
	gchar* nickname;	/* The (friendly) nickname */
	gchar* username;	/* The full username of the user */
};

#define CYANCHAT_MEMBER_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), CYANCHAT_TYPE_MEMBER, CyanChatMemberPrivate))

static GObjectClass* parent_class = NULL;

static void
cyanchat_member_class_init(CyanChatMemberClass* klass)
{
	GObjectClass* gobject_class = (GObjectClass*)klass;
	parent_class = g_type_class_peek_parent(klass);
	
	gobject_class->finalize = cyanchat_member_finalize;

	g_type_class_add_private(klass, sizeof(CyanChatMemberPrivate));
}

static void
cyanchat_member_init(CyanChatMember* member)
{
	member->priv = CYANCHAT_MEMBER_GET_PRIVATE(member);
	member->priv->username = NULL;
	member->priv->nickname = NULL;
	member->priv->level = kUser;
}

static void
cyanchat_member_finalize(GObject* object)
{
	CyanChatMemberPrivate* priv = CYANCHAT_MEMBER_GET_PRIVATE(object);
	
	if(priv->username)
		g_free(priv->username);
	
	if(priv->nickname)
		g_free(priv->nickname);
	
	parent_class->finalize(object);
}

/**
 * Gets the member's GType.
 *
 * @return The member's GType.
 */
GType
cyanchat_member_get_type()
{
	static GType type = 0;

	if (type == 0) {
		static const GTypeInfo info = {
			sizeof(CyanChatMemberClass),
			NULL,
			NULL,
			(GClassInitFunc) cyanchat_member_class_init,
			NULL,
			NULL,
			sizeof(CyanChatMember),
			0,
			(GInstanceInitFunc) cyanchat_member_init,
			NULL
		};
		type = g_type_register_static(G_TYPE_OBJECT, "CyanChatMember", &info, 0);
	}
	return type;
}

/**
 * Create a new #CyanChatMember.
 *
 * @return A #CyanChatMember.
 */
CyanChatMember*
cyanchat_member_new(const gchar* username)
{
	CyanChatMember* b;
	CyanChatMemberPrivate* priv;
	gchar* nick;
	gchar* utf8username;
	guint i = 0;
	
	b = CYANCHAT_MEMBER(g_object_new(cyanchat_member_get_type(), NULL));
	
	utf8username = purple_utf8_try_convert(username);
	if (utf8username == NULL) {
		utf8username = g_strdup(username);
	}
	
	priv = CYANCHAT_MEMBER_GET_PRIVATE(b);
	priv->username = g_strdup(utf8username);
	priv->level = g_ascii_digit_value(*utf8username);
	
	nick = g_malloc(24);
	while ((*(utf8username + i + 1) != ',') && (*(utf8username + i + 1) != '\0')) {
		*(nick + i) = *(utf8username + i + 1);
		i++;
	}
	*(nick + i) = '\0';
	
	priv->nickname = g_strdup(nick);
	
	g_free(nick);
	g_free(utf8username);
	
	return b;
}

/**
 * Get the username of a #CyanChatMember.
 *
 * @param b The member whose username to get.
 *
 * @return The member's username, or NULL.
 */
const gchar*
cyanchat_member_get_username(CyanChatMember* b)
{
	CyanChatMemberPrivate* priv;
	g_return_val_if_fail(b != NULL, NULL);
	priv = CYANCHAT_MEMBER_GET_PRIVATE(cc);
	
	return priv->username;
}

/**
 * Set the username of a #CyanChatMember.
 *
 * @param b The member whose username to set.
 * @param name The username to set.
 */
void
cyanchat_member_set_username(CyanChatMember* b, const gchar* name)
{
	CyanChatMemberPrivate* priv;
	g_return_if_fail(b != NULL);
	priv = CYANCHAT_MEMBER_GET_PRIVATE(cc);
	
	priv->username = name;
}

/**
 * Get the nickname of a #CyanChatMember.
 *
 * @param b The member whose nickname to get.
 *
 * @return The member's nickname, or NULL.
 */
const gchar*
cyanchat_member_get_nickname(CyanChatMember* b)
{
	CyanChatMemberPrivate* priv;
	g_return_val_if_fail(b != NULL, NULL);
	priv = CYANCHAT_MEMBER_GET_PRIVATE(cc);
	
	return priv->nickname;
}

/**
 * Set the nickname of a CyanChatMember.
 *
 * @param b The member whose nickname to set.
 * @param nick The nickname to set.
 */
void
cyanchat_member_set_nickname(CyanChatMember* b, const gchar* nick)
{
	CyanChatMemberPrivate* priv;
	g_return_if_fail(b != NULL);
	priv = CYANCHAT_MEMBER_GET_PRIVATE(cc);
	
	priv->nickname = nick;
}

/**
 * Get the #CyanChatUserLevel of a #CyanChatMember.
 *
 * @param b The member whose level to get.
 *
 * @return The member's #CyanChatUserLevel, or 0.
 */
CyanChatUserLevel
cyanchat_member_get_level(CyanChatMember* b)
{
	CyanChatMemberPrivate* priv;
	g_return_val_if_fail(b != NULL, kUser);
	priv = CYANCHAT_MEMBER_GET_PRIVATE(cc);
	
	return priv->level;
}

/**
 * Get the #PurpleConvChatBuddyFlags of a #CyanChatMember.
 *
 * @param b The member whose level flags to get.
 *
 * @return The member's #PurpleConvChatBuddyFlags, or 0.
 */
PurpleConvChatBuddyFlags
cyanchat_member_get_flags(CyanChatMember* b)
{
	CyanChatMemberPrivate* priv;
	g_return_val_if_fail(b != NULL, PURPLE_CBFLAGS_NONE);
	priv = CYANCHAT_MEMBER_GET_PRIVATE(cc);
	
	if (priv->level == kAdmin)
		return PURPLE_CBFLAGS_FOUNDER;
	else if (priv->level == kGuest)
		return PURPLE_CBFLAGS_OP;
	else
		return PURPLE_CBFLAGS_NONE;
}

/**
 * Set the #CyanChatUserLevel of a #CyanChatMember.
 *
 * @param b The member whose level to set.
 * @param l The #CyanChatUserLevel to set.
 */
void
cyanchat_member_set_level(CyanChatMember* b, CyanChatUserLevel l)
{
	CyanChatMemberPrivate* priv;
	g_return_if_fail(b != NULL);
	priv = CYANCHAT_MEMBER_GET_PRIVATE(cc);
	
	priv->level = l;
}
