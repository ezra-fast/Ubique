**UBIQUE**

Ubique is a modular Linux implant that uses GitHub as a C2. C2 communications are encrypted via TLS. The execution of Ubique takes the following simple steps:

1. Establish C2 Contact over TLS
2. Establish persistence via SSH keys and cron jobs
3. Establish a secondary presence on the victim
4. Wait to stage modules supplied by the C2

GitHub based C2 communications require a fine-grained personal access token to read instructions and modules. Based on the functionality of some modules, a write access token may be necessary.

Compilation:

`g++ -s core.cpp main.cpp clean-encoding.cpp run_command.cpp ssh_persistence.cpp crontab_persistence.cpp grab_read_access_token.cpp -o test -lssl -lcrypto -lpthread -I./json/include -I./base64/include`
