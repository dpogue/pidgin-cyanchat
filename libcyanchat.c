/**
 * @file libcyanchat.c CyanChat Protocol Plugin
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

#include "cyanchat.h"
#include "cc_utils.h"
#include "cc_server.h"

/* CyanChat Command functions */
static PurpleCmdId cc_cmds[kMaxCmd];

static PurpleCmdRet
cyanchat_cmd_nick(PurpleConversation* conv, const gchar* cmd, gchar** args, gchar** error, gpointer data)
{
	if(!PURPLE_CONV_CHAT(conv) || !args || !args[0])
		return PURPLE_CMD_RET_FAILED;
	
/*	PurpleConnection* pc;
	CyanChatAccount* cc;
	CyanChatServer* ccs;
	
	pc = purple_conversation_get_gc(conv);
	cc = pc->proto_data;
	gint id = purple_conv_chat_get_id(PURPLE_CONV_CHAT(conv));
	ccs = g_hash_table_find(cc->servers, cyanchat_server_find_by_chat_id, GINT_TO_POINTER(id));
	
	if (ccs == NULL)
		return PURPLE_CMD_RET_FAILED;
	
	ccs->reqnick = g_strdup(args[0]);
	
	cyanchat_join(ccs);*/
	
	return PURPLE_CMD_RET_OK;
}


/* libpurple plugin functions */
static void
cc_register_commands(void)
{
	/*cc_cmds[kNick]	= purple_cmd_register("nick", "s", PURPLE_CMD_P_PRPL,
		PURPLE_CMD_FLAG_CHAT | PURPLE_CMD_FLAG_PRPL_ONLY,
		"prpl-cyanchat", cyanchat_cmd_nick,
		_("nick &lt;new nickname&gt;:	Change your nickname."), NULL);*/
}

static void
cc_unregister_commands(void)
{ 
	gint i;
	/*for (i = 0; i < kMaxCmd; i++) {
		purple_cmd_unregister(cc_cmds[i]);
	}*/
}

static void
plugin_init(PurplePlugin* plugin)
{
	purple_debug_misc(CYANCHAT_PLUGIN_NAME, "Initializing plugin\n");

	PurpleAccountOption* option;
	PurplePluginInfo* info = plugin->info;
	PurplePluginProtocolInfo* prpl_info = info->extra_info;

	/* CHAT SERVER
	The server to which the connection is to be made */
	option = purple_account_option_string_new(_("Chat Server"), "server", "cho.cyan.com");
	prpl_info->protocol_options = g_list_append(prpl_info->protocol_options, option);
	
	/* CHAT PORT
	The port to which the connection is to be made */
	option = purple_account_option_int_new(_("Chat Port"), "port", 1812);
	prpl_info->protocol_options = g_list_append(prpl_info->protocol_options, option);

	/* CLEANER RELAY BOT OUTPUT
	This will prevent joins, parts, and renames on the IRC-side
	from causing Pidgin to beep as if there was real chat. */
	option = purple_account_option_bool_new(_("Cleaner Relay Bot Output"), "bot_cleaner", TRUE);
	prpl_info->protocol_options = g_list_append(prpl_info->protocol_options, option);
}

static gboolean
plugin_load(PurplePlugin* plugin)
{
	if (!g_thread_supported())
		g_thread_init(NULL);
	
	cc_register_commands();
	return TRUE;
}

static gboolean
plugin_unload(PurplePlugin* plugin)
{
	cc_unregister_commands();
	return TRUE;
}

/* libpurple plugin registration code */
static PurplePluginProtocolInfo prpl_info = {
	/* options */
	OPT_PROTO_PASSWORD_OPTIONAL | OPT_PROTO_SLASH_COMMANDS_NATIVE,
	NULL,	/* user_splits */
	NULL,	/* protocol_options */
	NO_BUDDY_ICONS,	/* icon_spec */
	/**/cyanchat_list_icon,	/* list_icon */
	NULL,	/* list_emblems */
	NULL,	/* status_text */
	NULL,	/* tooltip_text */
	/**/cyanchat_status_types,	/* status_types */
	NULL,	/* blist_node_menu */
	/**/cyanchat_chat_info,	/* chat_info */
	NULL,	/* chat_info_defaults */
	/**/cyanchat_login,	/* login */
	cyanchat_close,	/* close */
	cyanchat_send_im,	/* send_im */
	NULL,	/* set_info */
	NULL,	/* send_typing */
	cc_get_info,	/* get_info */
	NULL,	/* set_status */
	NULL,	/* set_idle */
	NULL,	/* change_passwd */
	NULL,	/* add_buddy */
	NULL,	/* add_buddies */
	NULL,	/* remove_buddy */
	NULL,	/* remove_buddies */
	NULL,	/* add_permit */
	NULL,	/* add_deny */
	NULL,	/* rem_permit */
	NULL,	/* rem_deny */
	NULL,	/* set_permit_deny */
	cyanchat_chat_join,	/* join_chat */
	NULL,	/* reject chat invite */
	cyanchat_chat_name,	/* get_chat_name */
	NULL,	/* chat_invite */
	cyanchat_chat_leave,	/* chat_leave */
	NULL,	/* chat_whisper */
	cyanchat_chat_send,	/* chat_send */
	/**/cyanchat_ping,	/* keepalive */
	NULL,	/* register_user */
	NULL,	/* get_cb_info */
	NULL,	/* get_cb_away */
	NULL,	/* alias_buddy */
	NULL,	/* group_buddy */
	NULL,	/* rename_group */
	NULL,	/* buddy_free */
	NULL,	/* convo_closed */
	/**/purple_normalize_nocase,	/* normalize */
	NULL,	/* set_buddy_icon */
	NULL,	/* remove_group */
	NULL,	/* get_cb_real_name */
	NULL,	/* set_chat_topic */
	NULL,	/* find_blist_chat */
	NULL,	/* roomlist_get_list */
	NULL,	/* roomlist_cancel */
	NULL,	/* roomlist_expand_category */
	NULL,	/* can_receive_file */
	NULL,	/* send_file */
	NULL,	/* new_xfer */
	NULL,	/* offline_message */
	NULL,	/* whiteboard_prpl_ops */
	cyanchat_send_raw,	/* send_raw */
	NULL,	/* roomlist_room_serialize */
	NULL,	/* unregister_user */
	NULL,	/* send_attention */
	NULL,	/* attention_types */
	sizeof(PurplePluginProtocolInfo),	/* struct_size */
	NULL
};

static PurplePluginInfo info = {
	PURPLE_PLUGIN_MAGIC,
	PURPLE_MAJOR_VERSION,
	PURPLE_MINOR_VERSION,
	PURPLE_PLUGIN_PROTOCOL,	/* type */
	NULL,	/* ui_requirement */
	0,	/* flags */
	NULL,	/* dependencies */
	PURPLE_PRIORITY_DEFAULT,	/* priority */
	CYANCHAT_PLUGIN_ID,	/* id */
	"Cyan Chat",	/* name */
	CYANCHAT_PLUGIN_VERSION,	/* version */
	"Pidgin CyanChat Plugin",	/* summary */
	"Allows Pidgin to connect to room running the CyanChat protocol, version 1",	/* description */
	"Darryl Pogue <dvpdiner2@gmail.com>",	/* author */
	"http://pidgin-cyanchat.googlecode.com/",	/* homepage */
	plugin_load,	/* load */
	plugin_unload,	/* unload */
	NULL,	/* destroy */
	NULL,	/* ui_info */
	&prpl_info,	/* extra_info */
	NULL,	/* prefs_info */
	NULL,	/* actions */
	NULL,	/* padding */
	NULL,
	NULL,
	NULL
};

/* Magic Macro to make everything work */
PURPLE_INIT_PLUGIN(cyanchat, plugin_init, info);
