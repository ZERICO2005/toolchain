#include <cstdio>
#include <cstddef>
#include <string>
#include <cstdlib>
#include <iostream>

using namespace std;

std::string char_to_str(const char* x) {
    return string(x);
}

void char_to_ref(std::string& str, const char* x) {
    str = string(x);
}

void char_to_ptr(std::string* str, const char* x) {
    *str = string(x);
}

int test() {
    // string foo = std::to_string(rand());
    // cout << foo << endl;
    return 0;
}
