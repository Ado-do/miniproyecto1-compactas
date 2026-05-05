#include "hello.hpp"

#include <sdsl/int_vector.hpp>
#include <iostream>

void hello(std::string name) {
    sdsl::int_vector<> v;
    std::cout << "Hola " << name << "!!! TKM\n";
}
