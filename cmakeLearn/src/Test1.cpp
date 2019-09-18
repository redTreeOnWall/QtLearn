#include "Test1.hpp"
#include <iostream>
void Test1::Say(){
    std::cout << "this is test \n";
    std::cout << "this is a test and i will say;\n";
};

void Test1::Loop(){
    this->Say();
}
