/**
 * @file cc_transport.h CyanChat Protocol Plugin
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

#ifndef CYANCHAT_TRANSPORT_H
#define CYANCHAT_TRANSPORT_H

#include "cyanchat.h"
#include "cc_account.h"
#include "cc_member.h"

/** @copydoc _CyanChatTransport */
typedef struct _CyanChatTransport CyanChatTransport;

/** Structure for passing data back and forth between callbacks. */
struct _CyanChatTransport {
	CyanChatAccount* cc;
	CyanChatMember* b;
	union {
		CyanChatMember* b2;
		CyanChatMessage* m;
	} u;
};

/**************************************************************************/
/** @name CyanChat Transport                                              */
/**************************************************************************/
/*@{*/

CyanChatTransport* cyanchat_transport_new(CyanChatAccount* cc, CyanChatMember* b, CyanChatMember* b2, CyanChatMessage* m);

/**
 * Get the #CyanChatAccount of a callback transport.
 *
 * @param t The callback Transport.
 *
 * @return The transport's #CyanChatAccount.
 */
CyanChatAccount* cyanchat_transport_get_account(CyanChatTransport* t);

/**
 * Get the #CyanChatMessage of a callback transport.
 *
 * @param t The callback Transport.
 *
 * @return The transport's #CyanChatMessage.
 */
CyanChatMessage* cyanchat_transport_get_message(CyanChatTransport* t);

/*}@*/

#endif /* CYANCHAT_TRANSPORT_H */
