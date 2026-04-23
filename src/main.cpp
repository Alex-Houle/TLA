#include "frontend.hpp"
#include "lexer.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

// read file into string
std::string readFile(const std::string &path) {
    FILE *f = std::fopen(path.c_str(), "rb");
    if (!f) {
        std::cout << "can not open file";
        exit(1);
    }
    std::fseek(f, 0, SEEK_END);
    long size = std::ftell(f);
    std::fseek(f, 0, SEEK_SET);

    std::string content;
    content.resize(size);
    std::fread(&content[0], 1, size, f);
    std::fclose(f);
    return content;
}

int main(int agrc, char *argv[]) {
    if (agrc < 2) {
        std::cout << "Not enough provided files" << std::endl;
        return 1;
    }

    std::string content = readFile(argv[1]);

    std::vector<TokenData> tokens = tokenize(content);

    std::cout << content << std::endl;
    return 0;
}
