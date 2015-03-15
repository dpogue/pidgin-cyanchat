# 1.6 #
  * Fix more userlist issues (especially when disconnected)
  * Change user flags slightly

# 1.5 #
  * Fix some potential memory leaks in the PurpleCmds
  * Menu item for bringing up the user information dialog
  * Fix a userlist issue where you would appear to rename upon joining the room

# 1.4 #
  * Fixed an issue with pipe characters in chat messages
  * Set the main port (1812) as default, rather than the debug server

# 1.3 #
  * Added support for /rot13 command to send "encrypted" text
  * Fixed the !version response to that it says "libpurple" rather than "Pidgin"
  * Fixed issues with sending < and > in chat messages

# 1.2 #
Mostly fixes to nickname stuff, including the /nick command.
  * Do not change Pidgin Alias if the /nick command is denied
  * If the nickname already exists, append an underscore

# 1.1 #
  * Fixed an issue which would cause errors of GTK <2.14
  * Added makefile for MINGW

# 1.0 #
  * Fixed a number of issues with the userlist and people renaming

# 0.3 #
  * Connecting to a server will automatically add the chat to your buddy list
  * Closing a chat will allow it to be reopened later
  * Better handling of nickname/alias stuff
  * /whois command will bring up user info
  * Invalid usernames will no longer disconnect you
  * Private Messages handle user renames

# 0.2 #
  * Userlist is now updated on a diff basis (looks for changes)
  * Support for the /nick command
  * Chat Server messages won't kill the client (should work on the debug server)

# 0.1 #
  * Initial revisions
