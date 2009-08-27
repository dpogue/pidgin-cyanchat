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

#ifndef _CYANCHAT_PURPLE_CYANCHAT_H
#define _CYANCHAT_PURPLE_CYANCHAT_H

#include <glib.h>

#ifndef G_GNUC_NULL_TERMINATED
#	if __GNUC__ >= 4
#		define G_GNUC_NULL_TERMINATED __attribute__((__sentinel__))
#	else
#		define G_GNUC_NULL_TERMINATED
#	endif /* __GNUC__ >= 4 */
#endif /* G_GNUC_NULL_TERMINATED */

#include <accountopt.h>
#include <blist.h>
#include <cmds.h>
#include <core.h>
#include <notify.h>
#include <plugin.h>
#include <request.h>
#include <status.h>
#include <version.h>

#include "cc_purple.h"

typedef enum {
	kCmdNick = 0,
	kCmdAuth,
	kCmdJoin,
	kCmdPart,
	kCmdQuit,
	kCmdIgnore,
	kCmdMax
} prplCyanChatCmds;

G_GNUC_CONST gchar* prplcyanchat_list_icon(PurpleAccount* a, PurpleBuddy* b);

#endif /* _CYANCHAT_PURPLE_CYANCHAT_H */
