/**
 * @file cyanchat.h CyanChat Protocol Plugin
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

#ifndef CYANCHAT_H
#define CYANCHAT_H

#define CYANCHAT_PLUGIN_NAME "cyanchat"
#define CYANCHAT_PLUGIN_ID "prpl-cyanchat"
#define CYANCHAT_PLUGIN_PROTOCOL 1
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

/* The libpurple commands supported in CyanChat */
typedef enum {
	kNick	= 0,
	kMaxCmd	= 1
} CyanChatPurpleCmds; /* kAuth, kPart, kQuit, kWhois */

/* libpurple Functions */
const gchar* cyanchat_list_icon(PurpleAccount* a, PurpleBuddy* b);
GList* cyanchat_status_types(PurpleAccount* account);
GList* cyanchat_chat_info(PurpleConnection* pc);
void cyanchat_login(PurpleAccount* account);

void cyanchat_ping(PurpleConnection* pc);




void cyanchat_close(PurpleConnection* pc);
gint cyanchat_send_im(PurpleConnection* pc, const gchar* who, const gchar* message, PurpleMessageFlags flags);
void cyanchat_chat_join(PurpleConnection* pc, GHashTable* data);
gchar* cyanchat_chat_name(GHashTable* data);
void cyanchat_chat_leave(PurpleConnection* pc, gint id);
gint cyanchat_chat_send(PurpleConnection* pc, gint id, const gchar* what, PurpleMessageFlags flags);


#endif /*CYANCHAT_H*/
