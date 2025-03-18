// resource: https://quuxplusone.github.io/blog/2023/09/08/constexpr-string-firewall/
// resource for caesar cipher: https://www.geeksforgeeks.org/caesar-cipher-in-cryptography/

/*
 * This example demonstrates the use of a constexpr function to hide the value of a hard-coded secret from (static) string analysis
 */

#include <iostream>

#define KEY 3

constexpr char * compile_time_obfuscate(char * input_string) {

    for (int i = 0; *(input_string + i) != '\0'; i++) {
        *(input_string + i) = *(input_string + i) ^ KEY;
    }

    return input_string;

}

int main() {

    char secret[] = "Wkjp.Jp.Wkf.wfpw.pwqjmd";                              // should be "This-Is-The-test-string" at runtime
    std::cout << "[+] Result: " << compile_time_obfuscate(secret) << '\n';

}

