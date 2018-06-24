#include <iostream>
#include "circular_buffer.h"


void mass_push_back(circular_buffer<int> &c, std::initializer_list<int> elems) {
    for (int const &e : elems)
        c.push_back(e);
}


int main() {
   /* circular_buffer<int> x;
    x.push_back(10);
    for (size_t i = 0; i < x.size(); i++)
        std::cout << x[i] << " ";
    std::cout << std::endl;
    x.push_front(15);
    for (size_t i = 0; i < x.size(); i++)
        std::cout << x[i] << " ";
    std::cout << std::endl;
    for (size_t i = 0; i < 15; i++) {
        x.push_back(i);
        for (size_t i = 0; i < x.size(); i++)
            std::cout << x[i] << " ";
        std::cout << std::endl;
    }
    for (size_t i = 0; i < x.size(); i++)
        std::cout << x[i] << " ";
    std::cout << std::endl;
    while (!x.empty()) {
        x.pop_back();
    }
    for (size_t i = 0; i < x.size(); i++)
        std::cout << x[i] << " ";
    std::cout << std::endl;
*/
    circular_buffer<int> c;
    mass_push_back(c, { 1, 2, 3, 4 });
    c.clear();
    std::cout << c.empty() << std::endl;
    std::cout << (c.begin() == c.end());
    mass_push_back(c, { 5, 6, 7, 8 });
    for (size_t i = 0; i < c.size(); i++)
        std::cout << c[i] << " ";
    std::cout << std::endl;
    return 0;
}