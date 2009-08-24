/**
 * @file cc_utils.c CyanChat Protocol Plugin
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

#include "cc_utils.h"

/* CyanChat Send Functions */
gint
cyanchat_send(guint fd, const gchar* msg)
{
	purple_debug_misc(CYANCHAT_PLUGIN_NAME, "< %s", msg);
	return write(fd, msg, strlen(msg));
}

void
cyanchat_send_ping(CyanChatAccount* cc)
{
	gchar* buf;
	buf = g_strdup_printf("%d|^\r\n", cLogIn);
	cyanchat_send(cc->fd, buf);
	g_free(buf);
}

void
cyanchat_send_join(CyanChatAccount* cc)
{
	gchar* buf;
	buf = g_strdup_printf("%d|%s\r\n", cLogIn, cyanchat_account_get_req_nick(cc));
	cyanchat_send(cc->fd, buf);
	g_free(buf);
}

void
cyanchat_send_part(CyanChatAccount* cc)
{
	gchar* buf;
	buf = g_strdup_printf("%d\r\n", cLogOut);
	cyanchat_send(cc->fd, buf);
	g_free(buf);
}

void
cyanchat_send_private(CyanChatAccount* cc, const gchar* text, CyanChatMember* b)
{
	gchar* buf;
	buf = g_strdup_printf("%d|%d%s|^0%s\r\n", cSendPM, cyanchat_member_get_level(b), cyanchat_member_get_nickname(b), text);
	cyanchat_send(cc->fd, buf);
	g_free(buf);
}

void
cyanchat_send_public(CyanChatAccount* cc, const gchar* text)
{
	gchar* buf;
	buf = g_strdup_printf("%d|^1%s\r\n", cSendMsg, text);
	cyanchat_send(cc->fd, buf);
	g_free(buf);
}

void
cyanchat_send_connect(CyanChatAccount* cc)
{
	gchar* buf;
	buf = g_strdup_printf("%d|%d\r\n", cConnect, CYANCHAT_PLUGIN_PROTOCOL);
	cyanchat_send(cc->fd, buf);
	g_free(buf);
}

void
cyanchat_send_auth(CyanChatAccount* cc, const gchar* passwd)
{
	gchar* buf;
	buf = g_strdup_printf("%d|%s\r\n", cAuth, passwd);
	cyanchat_send(cc->fd, buf);
	g_free(buf);
}

void
cyanchat_send_ignore(CyanChatAccount* cc, CyanChatMember* b)
{
	gchar* buf;
	buf = g_strdup_printf("%d|%d%s\r\n", cIgnore, cyanchat_member_get_level(b), cyanchat_member_get_nickname(b));
	cyanchat_send(cc->fd, buf);
	g_free(buf);
}
