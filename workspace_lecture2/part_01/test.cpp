#include <iostream>
#include "test.hpp"
// int add(int a, int b); // 函数声明（已注释）

int main() {
    int x = 3, y = 4;
    int result = add(x, y); // 调用已注释的函数
    std::cout << "Result: " << result << std::endl;
    //std::cout << "函数已被注释，仅引用。" << std::endl;
    return 0;
}

// 函数定义（已注释）
int add(int a, int b) {
    return a + b;
}
