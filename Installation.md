# Installing CyanChat for Pidgin #
## Compiling from Source ##
Checkout the most recent code from SVN and compile using make.
```
svn checkout http://pidgin-cyanchat.googlecode.com/svn/trunk/ pidgin-cyanchat-read-only
cd ./pidgin-cyanchat-read-only
make
sudo make install
```

## Using the Debian Package (Debian, Ubuntu) ##
Download the .deb file and run it using your OS's package manager.

## Using the DLL file (Windows) ##
Download the .dll file.
Download the icons.zip file.

Drag the .dll to C:/Program Files/Pidgin/plugins/

Extract the contents of the .zip file to C:/Program Files/Pidgin/pixmaps/pidgin/protocols folder.

## Adding the Chatroom ##
  1. Create a cyanchat account in Pidgin (Accounts -> Manage)
  1. Change the server and port settings to match the server (cho.cyan.com 1812)
  1. Double click the chat icon on your buddy list