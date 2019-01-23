#ifndef NETWORKPACKETS_HPP
#define NETWORKPACKETS_HPP

#include "NetworkPacketHandler.hpp"

PACKET(Foo, 1) {
    int i1 = 0;
    int i2 = 0;
    std::string dude;

    void print() { std::cout << i1 << " " << i2 << " " << dude << std::endl; }
};

PACKET(Foo2, 2) {
    int i = 0;
    std::string str;
    std::vector<Foo> nums;

    void print() {
        std::cout << i << " " << str << "\n";
        for (auto val : nums) val.print();
        std::cout << std::endl;
    }
};

PACKET(Foo3, 3) {
    int i = 0;
    std::string str;
    std::vector<Foo2> vec;

    void print() {
        std::cout << i << " " << str << "\n";
        for (auto& val : vec) val.print();
    }
};

#endif  // NETWORKPACKETS_HPP