/*
 * libcyanchat
 *
 * libcyanchat is the property of its developers.  See the COPYRIGHT file
 * for more details.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
#	endif /* __GNUC__ >= 4 */
#endif /* G_GNUC_NULL_TERMINATED */

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

typedef struct _CyanChatAccount CyanChatAccount;
typedef struct _CyanChatServer CyanChatServer;
typedef struct _CyanChatMember CyanChatMember;

typedef enum
{
    kUser   = 0,
    kAdmin  = 1,
    kServer = 2,
    kError  = 3,
    kGuest  = 4
} CyanChatUserLevel;

/* The CyanChat Account */
struct _CyanChatAccount {
    /* The PurpleAccount */
    PurpleAccount* account;
    
    /* The PurpleConnection */
    PurpleConnection* pc;
    
    /* A list of all the CyanChatServers we are connected to */
    GSList* servers;

    /*gboolean connected;
    gboolean firstTime;
    gboolean login2used;
    gchar* desiredName;*/
};

/* A Connection to a CyanChat server */
struct _CyanChatServer {
    /* Server Name */
    gchar* host;
    
    /* Server Port */
    guint port;
    
    /* Connection File Descriptor */
    int fd;
    
    /* Our Nickname in the chat */
    gchar* nickname;
    
    /* Our "desired" nickname while waiting for approval */
    gchar* reqnick;
    
    /* List of chat users */
    GHashTable* users;
    
    /* Purple Conversation ID */
    int id;
    
    /* The room's PurpleConversation*/
    PurpleConversation* conv;
    
    /* The read buffer */
    gchar* rxbuf;
    
    /* The offset in the read buffer */
    guint rxoff;
    
    /* The size of the read buffer */
    guint rxsize;
};

/* A member of a CyanChat room */
struct _CyanChatMember {
    /* The chat user level */
    CyanChatUserLevel l;
    
    /* The (friendly) nickname */
    gchar* nickname;
    
    /* The full username (level + nickname + mangled IP) */
    gchar* username;
};

/* libpurple Functions */
const char* cyanchat_list_icon(PurpleAccount* a, PurpleBuddy* b);
GSList* cyanchat_status_types(PurpleAccount* account);
void cyanchat_login(PurpleAccount* account);
void cyanchat_close(PurpleConnection* pc);

#endif /*CYANCHAT_H*/
