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

#include "cyanchat.h"

/* libpurple CyanChat PRPL */
static PurplePluginProtocolInfo prpl_info = {
	OPT_PROTO_PASSWORD_OPTIONAL | OPT_PROTO_SLASH_COMMANDS_NATIVE,  /* options */
	NULL,                                                           /* user splits */
	NULL,                                                           /* protocol options */
	NO_BUDDY_ICONS,                                                 /* icon spec */
	prplcyanchat_list_icon,                                         /* list icon */
	NULL,                                                           /* list emblems */
	NULL,                                                           /* status text */
	NULL,                                                           /* tooltip text */
	NULL,                                                           /* status types */
	NULL,                                                           /* blist node menu */
	NULL,                                                           /* chat info */
	NULL,                                                           /* chat info defaults */
	NULL,                                                           /* login */
