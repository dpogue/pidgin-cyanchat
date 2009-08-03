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
 * along with this program.	If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CYANCHAT_H
#define CYANCHAT_H

#define CYANCHAT_PLUGIN_VERSION "2.0"

#include <glib.h>

#include <errno.h>
#include <string.h>
#include <glib/gi18n.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef G_GNUC_NULL_TERMINATED
#	if __GNUC__ >= 4
#		define G_GNUC_NULL_TERMINATED __attribute__((__sentinel__))
#	else
#		define G_GNUC_NULL_TERMINATED
#	endif	/* __GNUC__ >= 4 */
#endif		/* G_GNUC_NULL_TERMINATED */

#ifdef _WIN32
#	include "win32dep.h"
#else
#	include <arpa/inet.h>
#endif

#ifndef PURPLE_PLUGINS
#	define PURPLE_PLUGINS
#endif

#include "accountopt.h"
#include "blist.h"
#include "cmds.h"
#include "connection.h"
#include "debug.h"
#include "dnsquery.h"
#include "proxy.h"
#include "prpl.h"
#include "request.h"
#include "util.h"
#include "version.h"

typedef struct _CyanChatAccount	CyanChatAccount;
typedef struct _CyanChatServer	CyanChatServer;
typedef struct _CyanChatMember	CyanChatMember;

typedef enum {
	kUser   = 0,
	kAdmin  = 1,
	kServer = 2,
	kError  = 3,
	kGuest  = 4
} CyanChatUserLevel;

/* The CyanChat Account */
struct _CyanChatAccount {
	PurpleAccount* account;	/* The PurpleAccount */
	PurpleConnection* pc;	/* The PurpleConnection */
	GHashTable* servers;	/* A table of all the CyanChatServers we are connected to */
};

/* A Connection to a CyanChat server */
struct _CyanChatServer {
	CyanChatAccount* account;	/* The CyanChatAccount to which this server belongs */
	gchar* host;	/* Server Name */
	guint port;	/* Server Port */
	GHashTable* data;	/* Server & Port information */
	gint fd;	/* Connection File Descriptor */
	gchar* nickname;	/* Our Nickname in the chat */
	gchar* reqnick;	/* Our "desired" nickname while waiting for approval */
	GHashTable* users;	/* List of chat users */
	gint id;	/* Purple Conversation ID */
	PurpleConversation* conv;	/* The room's PurpleConversation*/
	gchar* rxbuf;	/* The read buffer */
	guint rxoff;	/* The offset in the read buffer */
	guint rxsize;	/* The size of the read buffer */
	guint inputhandler;	/* Input Handling callback method */
	gchar* lobbymsg;	/* The server's lobby message */
};

/* A member of a CyanChat room */
struct _CyanChatMember {
	gchar* id;	/* The chat user's ID (nickname@server:port) */
	CyanChatUserLevel level;	/* The chat user level */
	gchar* nickname;	/* The (friendly) nickname */
	gchar* username;	/* The full username of the user */
	CyanChatServer* server;	/* The CyanChatServer where this user exists */
	time_t lastupdate;	/* Time of thelast userlist update for this user */
};

/* The libpurple commands supported in CyanChat */
typedef enum {
	kNick	= 0,
	kMaxCmd	= 1
} CyanChatPurpleCmds;

/* libpurple Functions */
const gchar* cyanchat_list_icon(PurpleAccount* a, PurpleBuddy* b);
GList* cyanchat_status_types(PurpleAccount* account);
GList* cyanchat_chat_info(PurpleConnection* pc);
GHashTable* cyanchat_chat_info_defaults(PurpleConnection* pc, const gchar* chat_name);
void cyanchat_login(PurpleAccount* account);
void cyanchat_close(PurpleConnection* pc);
gint cyanchat_send_im(PurpleConnection* pc, const gchar* who, const gchar* message, PurpleMessageFlags flags);
void cyanchat_chat_join(PurpleConnection* pc, GHashTable* data);
gchar* cyanchat_chat_name(GHashTable* data);
void cyanchat_chat_leave(PurpleConnection* pc, gint id);
gint cyanchat_chat_send(PurpleConnection* pc, gint id, const gchar* what, PurpleMessageFlags flags);
void cyanchat_keepalive(PurpleConnection* pc);
gchar* cyanchat_cb_real_id(PurpleConnection* pc, gint id, const gchar* who);

/* libpurple plugin functions */
/*static void cc_register_commands(void);
static void cc_unregister_commands(void);
static void plugin_init(PurplePlugin* plugin);
static gboolean plugin_load(PurplePlugin* plugin);
static gboolean plugin_unload(PurplePlugin* plugin);*/

#endif /*CYANCHAT_H*/
