#include <iostream>

int tempCount = 1;

std::string generateTemp() {
    return "t" + std::to_string(tempCount++);
}

void generateThreeAddressCode(const std::string& op, const std::string& arg1, const std::string& arg2, const std::string& result) {
    std::cout << result << " = " << arg1 << " " << op << " " << arg2 << std::endl;
}

std::string generateFactorialCode(int n) {
    std::string ret = generateTemp();
    if (n == 1) {
        generateThreeAddressCode("=", "1", "", ret);
    } else {
        std::string ans = generateTemp();
        generateThreeAddressCode("*", std::to_string(n), generateFactorialCode(n - 1), ans);
        generateThreeAddressCode("=", ans, "", ret);
    }
    return ret;
}

int main() {
    int n = 5;
    std::string result = generateFactorialCode(n);
    std::cout << "Factorial of " << n << " = " << result << std::endl;
    return 0;
}
