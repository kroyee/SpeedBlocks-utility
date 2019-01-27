#ifndef NETWORKPACKETS_HPP
#define NETWORKPACKETS_HPP

#include "NetworkPacketHandler.hpp"

PACKET(Foo, 1) {
    int i1 = 0;
    int i2 = 0;
    std::string dude;

    void print() const { std::cout << i1 << " " << i2 << " " << dude << std::endl; }
};

PACKET(Foo2, 2) {
    int i = 0;
    std::string str;
    std::vector<Foo> nums;

    void print() const {
        std::cout << i << " " << str << "\n";
        for (auto val : nums) val.print();
        std::cout << std::endl;
    }
};

PACKET(Foo3, 3) {
    int i = 0;
    std::string str;
    std::vector<Foo2> vec;

    void print() const {
        std::cout << i << " " << str << "\n";
        for (auto& val : vec) val.print();
    }
};

PACKET(Foo4, 4){static void print(){std::cout << "Foo4 is empty"
                                              << "\n";
}
}
;

#endif  // NETWORKPACKETS_HPP