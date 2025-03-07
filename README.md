**UBIQUE**

Ubique is a modular Linux implant that uses GitHub as a C2. C2 communications are encrypted via TLS. The execution of Ubique takes the following simple steps:

1. Establish C2 Contact over TLS
2. Establish persistence via SSH keys and cron jobs
3. Confirm persistence has been established on the victim
4. Loop, waiting to stage modules supplied by the C2

GitHub based C2 communications require a fine-grained personal access token to read instructions and modules. Based on the functionality of some modules, a write access token may be necessary.

**Compilation:**

```
g++ -s core.cpp main.cpp clean-encoding.cpp run_command.cpp ssh_persistence.cpp crontab_persistence.cpp grab_read_access_token.cpp read_c2_message.cpp -o test -lssl -lcrypto -lpthread -I./json/include -I./base64/include
```
or
```
./compile.sh
```

**Dependencies:**


1. [JSON Library for C++](https://github.com/nlohmann/json)
2. [Base64 encoder/decoder for C++](https://github.com/tobiaslocker/base64/tree/master)
3. [C++ HTTP(S) Library](https://github.com/yhirose/cpp-httplib)
4. All other dependencies (linux-vdso.so, libssl.so, libcrypto.so, libstdc++.so, libgcc_s.so, libc.so, libm.so) should already be on the platform used for compilation with g++.

**TODO:**

1. Implement compile time obfuscation for GHPAT (removing the need for grab_read_access_token())
2. Write the Core::loop routine
3. Implement an update mechanism
4. Implement a self-destruct routine

