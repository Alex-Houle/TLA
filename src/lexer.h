#ifndef LEXER_H
#define LEXER_H

#include <string>

enum class Token {
    // Arithmetic operators
    PLUS,
    MINUS,
    MULT,
    DIV,
    MOD,
    INC,
    DEC,

    // Assignment operators
    PLUS_EQ,
    MINUS_EQ,
    MULT_EQ,
    DIV_EQ,
    ASSIGN,

    // Relational operators
    GREATER_EQ,
    LESS_EQ,
    GREATER,
    LESS,
    EQ,

    // Logical operators
    LOG_AND,
    LOG_OR,
    LOG_NOT,

    // Bitwise operators
    XOR,
    AND,
    OR,
    NOT,

    // Brackets & punctuation
    LBRACK,
    RBRACK,
    LPAREN,
    RPAREN,
    COMMA,
    SEMI,

    // Literals & identifiers
    TYPE,
    IDENTIFIER,
    INT,
    FLOAT,
    STRING,

    // Keywords
    RETURN,
};

struct TokenData {
    Token type;
    std::string val; // empty for tokens with no associated value
};

#endif
