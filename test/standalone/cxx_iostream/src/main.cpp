#include <iostream>
#include <iomanip>

int main(void) {

    std::ios_base::Init();

    long double n = 2.718281828459045235360287472662L;

    std::cout << std::setprecision(20) << n;

    return 0;
}
