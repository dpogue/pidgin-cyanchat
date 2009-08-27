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

#include "cc_buddy.h"

#include <glib.h>
#include <glib/gprintf.h>
#include <glib-object.h>

/* Private Data */
typedef struct _CyanChatBuddyPrivate CyanChatBuddyPrivate;
struct _CyanChatBuddyPrivate {
	gchar* nickname;
	CyanChatBuddyLevel level;
	gboolean ignored;
};

/* Properties */
enum {
	PROP_BUDDY_0,
	PROP_NICKNAME,
	PROP_LEVEL,
	PROP_IGNORED
};

#define CYANCHAT_BUDDY_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), CYANCHAT_TYPE_BUDDY, CyanChatBuddyPrivate))

G_DEFINE_TYPE(CyanChatBuddy, cyanchat_buddy, G_TYPE_OBJECT);

static void
cyanchat_buddy_init(CyanChatBuddy* self)
{
	CyanChatBuddyPrivate* priv = CYANCHAT_BUDDY_GET_PRIVATE(self);

	priv->nickname = NULL;
	priv->level = 0;
	priv->ignored = FALSE;
}

static void
cyanchat_buddy_finalize(GObject* object)
{
	CyanChatBuddy* b;

	g_return_if_fail(object != NULL);
	g_return_if_fail(CYANCHAT_IS_BUDDY(object));

	b = CYANCHAT_BUDDY(object);
	CyanChatBuddyPrivate* priv = CYANCHAT_BUDDY_GET_PRIVATE(b);

	g_free(priv->nickname);

	G_OBJECT_CLASS(cyanchat_buddy_parent_class)->finalize(object);
}

static void
cyanchat_buddy_set_property(GObject* object, guint prop_id, const GValue* value, GParamSpec* pspec)
{
	CyanChatBuddyPrivate* priv;
	g_return_if_fail(CYANCHAT_IS_BUDDY(object));

	priv = CYANCHAT_BUDDY_GET_PRIVATE(object);

	switch(prop_id) {
		case PROP_NICKNAME:
			g_free(priv->nickname);
			priv->nickname = g_value_dup_string(value);
			break;
		case PROP_LEVEL:
			priv->level = g_value_get_enum(value);
			break;
		case PROP_IGNORED:
			priv->ignored = g_value_get_boolean(value);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
			break;
	}
}

static void
cyanchat_buddy_get_property(GObject* object, guint prop_id, GValue* value, GParamSpec* pspec)
{
	CyanChatBuddyPrivate* priv;
	g_return_if_fail(CYANCHAT_IS_BUDDY(object));

	priv = CYANCHAT_BUDDY_GET_PRIVATE(object);

	switch(prop_id) {
		case PROP_NICKNAME:
			g_value_set_string(value, priv->nickname);
			break;
		case PROP_LEVEL:
			g_value_set_enum(value, priv->level);
			break;
		case PROP_IGNORED:
			g_value_set_boolean(value, priv->ignored);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
			break;
	}
}

static void
cyanchat_buddy_class_init(CyanChatBuddyClass* klass)
{
	GObjectClass* gobject_class = (GObjectClass*)klass;

	gobject_class->finalize = cyanchat_buddy_finalize;
	gobject_class->set_property = cyanchat_buddy_set_property;
	gobject_class->get_property = cyanchat_buddy_get_property;

	g_object_class_install_property(gobject_class, PROP_NICKNAME,
			g_param_spec_string("nickname",
			"Nickname",
			"The nickname of the chat buddy.",
			NULL, G_PARAM_READWRITE));
	g_object_class_install_property(gobject_class, PROP_LEVEL,
			g_param_spec_enum("level",
			"User Level",
			"The user level of the chat buddy.",
			CYANCHAT_TYPE_BUDDY_LEVEL,
			CYANCHAT_LEVEL_USER,
			G_PARAM_READWRITE));
	g_object_class_install_property(gobject_class, PROP_IGNORED,
			g_param_spec_boolean("ignored",
			"Ignored",
			"The ignored flag value of the chat buddy.",
			FALSE, G_PARAM_READWRITE));

	g_type_class_add_private(klass, sizeof(CyanChatBuddyPrivate));
}

GType
cyanchat_buddy_level_get_type()
{
	static GType type = 0;
	if(type == 0) {
		static const GEnumValue values[] = {
			{CYANCHAT_LEVEL_USER, "CYANCHAT_LEVEL_USER", "user"},
			{CYANCHAT_LEVEL_ADMIN, "CYANCHAT_LEVEL_ADMIN","admin"},
			{CYANCHAT_LEVEL_ERROR, "CYANCHAT_LEVEL_ERROR", "error"},
			{CYANCHAT_LEVEL_SERVER, "CYANCHAT_LEVEL_SERVER", "server"},
			{CYANCHAT_LEVEL_GUEST, "CYANCHAT_LEVEL_GUEST", "guest"},
			{0, NULL, NULL}
		};
		type = g_enum_register_static("CyanChatBuddyLevel", values);
	}
	return type;
}

CyanChatBuddy*
cyanchat_buddy_new(const gchar* nickname, const CyanChatBuddyLevel level, const gboolean ignored)
{
	return CYANCHAT_BUDDY(g_object_new(CYANCHAT_TYPE_BUDDY, "nickname", nickname, "level", level, "ignored", ignored, NULL));
}

CyanChatBuddy*
cyanchat_buddy_new_from_username(const gchar* username)
{
	const gchar* c;
	const gchar* br;
	gchar* nickname;
	CyanChatBuddyLevel level;
	
	level = g_ascii_digit_value(*username);

	for(c = username; *c != '\0'; c++) {
		if(*c == ',') {
			br = c - 1;
		}
	}

	nickname = g_strndup(username + 1, br - username);
	/*nickname = purple_utf8_try_convert(nickname);
	if(nickname == NULL)
		nickname = g_strndup(username + 1, br - username);*/

	return CYANCHAT_BUDDY(g_object_new(CYANCHAT_TYPE_BUDDY, "nickname", nickname, "level", level, "ignored", FALSE, NULL));
}

gchar*
cyanchat_buddy_get_nickname(CyanChatBuddy* b)
{
	gchar* nickname;
	g_return_val_if_fail(CYANCHAT_IS_BUDDY(b), NULL);
	g_object_get(b, "nickname", &nickname, NULL);
	return nickname;
}

CyanChatBuddyLevel
cyanchat_buddy_get_level(CyanChatBuddy* b)
{
	CyanChatBuddyLevel level;
	g_return_val_if_fail(CYANCHAT_IS_BUDDY(b), 0);
	g_object_get(b, "level", &level, NULL);
	return level;
}

gboolean
cyanchat_buddy_get_ignored(CyanChatBuddy*b)
{
	gboolean ignored;
	g_return_val_if_fail(CYANCHAT_IS_BUDDY(b), FALSE);
	g_object_get(b, "ignored", &ignored, NULL);
	return ignored;
}

void
cyanchat_buddy_free(CyanChatBuddy* b)
{
	g_return_if_fail(b != NULL);
	
	cyanchat_buddy_finalize((GObject*)b);
}
