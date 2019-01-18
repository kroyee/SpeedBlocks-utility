#include <iostream>
#include <sstream>
#include "Serialize.hpp"

struct Test {
    int i = 5;
    float f = 3.14;
    std::string s = "Hey";

    template <typename T>
    void serialize(T&& handle) const {
        handle(i, s);
    }
};

struct Foo {
    int p1;
    uint8_t p2;
};

int main() {
    Test t;
    Foo f{12, 'm'};

    std::stringstream test;

    int a, c;
    std::string b, d;

    Stream::To(std::stringstream{}) << t << " " << f >> a >> b >> c >> d;
    std::cout << std::endl << a << " " << b << " " << c << " " << d << std::endl;
}
