/**
 * @file cc_parser.h CyanChat Protocol Plugin
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

#ifndef CYANCHAT_PARSER_H
#define CYANCHAT_PARSER_H

#include "cyanchat.h"
#include "cc_account.h"
#include "cc_utils.h"

/** @copydoc _CyanChatMessage */
typedef struct _CyanChatMessage CyanChatMessage;

/** @copydoc _CyanChatUserlist */
typedef struct _CyanChatUserlist CyanChatUserlist;

/**************************************************************************/
/** @name CyanChat Message                                                */
/**************************************************************************/
/*@{*/
struct _CyanChatMessage
{
	gchar* message;
	time_t time;
};

CyanChatMessage* cyanchat_message_new(const gchar* msg, time_t time);

void cyanchat_message_free(CyanChatMessage* m);

/*}@*/

/**************************************************************************/
/** @name CyanChat Userlist                                               */
/**************************************************************************/
/*@{*/
struct _CyanChatUserlist
{
	guint size;
	gchar* strdata;
	GList* members;
};

void cyanchat_userlist_parse(CyanChatTransport* t);

/*}@*/

/**************************************************************************/
/** @name CyanChat Parser                                                 */
/**************************************************************************/
/*@{*/


gboolean cyanchat_parser_quick(CyanChatAccount* cc, CyanCyanMessage* m);

/*}@*/

#endif /* CYANCHAT_PARSER_H */
