#pragma once

#include "lexer.h"
#include <string>
#include <vector>

std::vector<TokenData> tokenize(const std::string &code);
