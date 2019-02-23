#include <string>
#include <iostream>

class FooBar {
};

template <typename T>
void foo(T) {
    std::cout << sizeof(T);
}

int main() {
    foo(0);
    return 0;
}