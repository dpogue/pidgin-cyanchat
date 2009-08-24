/**
 * @file cc_transport.c CyanChat Protocol Plugin
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

#include "cc_transport.h"

/* CyanChat Transport functions */
CyanChatTransport*
cyanchat_transport_new(CyanChatAccount* cc, CyanChatMember* b, CyanChatMember* b2, CyanChatMessage* m)
{
	CyanChatTransport* t;

	t = g_new0(CyanChatTransport, 1);

	t->cc = cc;
	t->b = b;

	if(b2 != NULL)
		t->u.b2 = b2;
	if(m != NULL)
		t->u.m = m;

	return t;
}

/**
 * Get the #CyanChatAccount of a callback transport.
 *
 * @param t The callback Transport.
 *
 * @return The transport's #CyanChatAccount.
 */
CyanChatAccount*
cyanchat_transport_get_account(CyanChatTransport* t)
{
	return t->cc;
}

/**
 * Get the #CyanChatMessage of a callback transport.
 *
 * @param t The callback Transport.
 *
 * @return The transport's #CyanChatMessage.
 */
CyanChatMessage*
cyanchat_transport_get_message(CyanChatTransport* t)
{
	return t->u.m;
}
