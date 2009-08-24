/**
 * @file cc_callback.c CyanChat Protocol Plugin
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

#include "cc_callback.h"

gboolean
cyanchat_callback_ping(CyanChatAccount* cc)
{
	purple_connection_error_reason(cyanchat_account_get_connection(cc), PURPLE_CONNECTION_ERROR_NETWORK_ERROR, _("Ping timed out"));
	cyanchat_account_clear_timeout(cc);
	return FALSE;
}

gboolean
cyanchat_callback_pong(CyanChatAccount* cc)
{
	if (cyanchat_account_get_timeout(cc) != 0) {
		cyanchat_account_remove_timeout(cc);
		cyanchat_account_clear_timeout(cc);
	}
	
	return FALSE;
}

void
cyanchat_callback_login(gpointer data, gint source, const gchar *error_message)
{
	CyanChatAccount* cc;
	PurpleConnection* pc;
	
	cc = CYANCHAT_ACCOUNT(data);
	pc = cyanchat_account_get_connection(cc);
	
	if (!PURPLE_CONNECTION_IS_VALID(pc)) {
		close(source);
		return;
	}

	if (source < 0) {
		gchar *tmp;
		tmp = g_strdup_printf(_("Could not establish a connection with the server:\n%s"), error_message);
		purple_connection_error_reason(pc, PURPLE_CONNECTION_ERROR_NETWORK_ERROR, tmp);
		g_free(tmp);
		return;
	}
	
	cc->fd = source;
	
	cyanchat_account_set_input(cc, purple_input_add(cc->fd, PURPLE_INPUT_READ, cyanchat_callback_input, cc));
	
	purple_connection_update_progress(pc, _("Connecting"), 1, 3);
}

void
cyanchat_callback_input(gpointer data, gint source, PurpleInputCondition cond)
{
	CyanChatAccount* cc;
	
	cc = CYANCHAT_ACCOUNT(data);
	
	cyanchat_account_read(cc);
}
