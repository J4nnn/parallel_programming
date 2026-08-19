//
// Created by j4nn on 8/18/26.
//

#include <iostream>

int main()
{
    const int num = 14;
    int* px = nullptr;

    std::cout << num << std::endl;
    std::cout << &num << std::endl;
    std::cout << px << std::endl;
    std::cout << *px << std::endl;
    std::cout << px << std::endl;
    std::cout << *px << std::endl;
    return 0;
}