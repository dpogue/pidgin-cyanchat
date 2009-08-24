/**
 * @file cc_account.h CyanChat Protocol Plugin
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

#ifndef CYANCHAT_ACCOUNT_H
#define CYANCHAT_ACCOUNT_H

#include <glib.h>
#include <glib-object.h>

/** @copydoc _CyanChatAccountClass */
typedef struct _CyanChatAccountClass CyanChatAccountClass;
/** @copydoc _CyanChatAccount */
typedef struct _CyanChatAccount CyanChatAccount;

#include "cyanchat.h"

G_BEGIN_DECLS

#define CYANCHAT_TYPE_ACCOUNT            (cyanchat_account_get_type())
#define CYANCHAT_ACCOUNT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), CYANCHAT_TYPE_ACCOUNT, CyanChatAccount))
#define CYANCHAT_ACCOUNT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), CYANCHAT_TYPE_ACCOUNT, CyanChatAccountClass))
#define CYANCHAT_IS_ACCOUNT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), CYANCHAT_TYPE_ACCOUNT))
#define CYANCHAT_IS_ACCOUNT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), CYANCHAT_TYPE_ACCOUNT))
#define CYANCHAT_ACCOUNT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), CYANCHAT_TYPE_ACCOUNT, CyanChatAccountClass))

/**************************************************************************/
/** @name CyanChat Account                                                */
/**************************************************************************/
/*@{*/

/**
 * Gets the account's GType.
 *
 * @return The account's GType.
 */
GType cyanchat_account_get_type(void);

/**
 * Create a new #CyanChatAccount.
 *
 * @return A #CyanChatAccount.
 */
CyanChatAccount* cyanchat_account_new(void);

/**
 * Read data from the socket into a buffer and then add the buffer
 * to the GQueue for parsing.
 *
 * @param cc The account to which is read.
 */
void cyanchat_account_read(CyanChatAccount* cc);

/**
 * Get the #PurpleConnection of a #CyanChatAccount.
 *
 * @param cc The account whose connection to get.
 *
 * @return The account's #PurpleConnection.
 */
PurpleConnection* cyanchat_account_get_connection(CyanChatAccount* cc);

/**
 * Set the #PurpleConnection of a #CyanChatAccount.
 *
 * @param cc The account whose connection to set.
 * @param pc The connection to assign.
 */
void cyanchat_account_set_connection(CyanChatAccount* cc, PurpleConnection* pc);


const gchar* cyanchat_account_get_host(CyanChatAccount* cc);


void cyanchat_account_set_host(CyanChatAccount* cc, const gchar* host);


guint cyanchat_account_get_port(CyanChatAccount* cc);


void cyanchat_account_set_port(CyanChatAccount* cc, guint port);


const gchar* cyanchat_account_get_req_nick(CyanChatAccount* cc);


void cyanchat_account_set_req_nick(CyanChatAccount* cc, const gchar* nick);


void cyanchat_account_set_input(CyanChatAccount* cc, guint in);


void cyanchat_account_remove_input(CyanChatAccount* cc);


guint cyanchat_account_get_timeout(CyanChatAccount* cc);


void cyanchat_account_set_timeout(CyanChatAccount* cc, guint interval, GSourceFunc function);


void cyanchat_account_clear_timeout(CyanChatAccount* cc);


gboolean cyanchat_account_remove_timeout(CyanChatAccount* cc);

/*}@*/

G_END_DECLS

#endif /* CYANCHAT_ACCOUNT_H */
