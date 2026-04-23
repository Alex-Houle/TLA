#include "lexer.h"

#include <cctype>
#include <stdexcept>
#include <string>
#include <vector>

// ── helpers ──────────────────────────────────────────────────────────────────

static void push(std::vector<TokenData> &v, Token type, std::string val = {}) {
    v.push_back({type, std::move(val)});
}

// ── public interface
// ──────────────────────────────────────────────────────────

std::vector<TokenData> tokenize(const std::string &src) {
    std::vector<TokenData> tokens;
    tokens.reserve(src.size() / 4); // rough heuristic to avoid reallocations

    size_t i = 0;
    const size_t n = src.size();

    // Peek at the next character without advancing
    auto peek = [&](size_t offset = 1) -> char {
        return (i + offset < n) ? src[i + offset] : '\0';
    };

    while (i < n) {
        const char c = src[i];

        // ── whitespace ───────────────────────────────────────────────────────
        if (std::isspace((unsigned char)c)) {
            ++i;
            continue;
        }

        // ── single-line comments ─────────────────────────────────────────────
        if (c == '/' && peek() == '/') {
            while (i < n && src[i] != '\n')
                ++i;
            continue;
        }

        // ── operators & punctuation ──────────────────────────────────────────
        switch (c) {

        case '+':
            if (peek() == '=') {
                push(tokens, Token::PLUS_EQ);
                i += 2;
            } else if (peek() == '+') {
                push(tokens, Token::INC);
                i += 2;
            } else {
                push(tokens, Token::PLUS);
                ++i;
            }
            continue;

        case '-':
            if (peek() == '=') {
                push(tokens, Token::MINUS_EQ);
                i += 2;
            } else if (peek() == '-') {
                push(tokens, Token::DEC);
                i += 2;
            } else {
                push(tokens, Token::MINUS);
                ++i;
            }
            continue;

        case '*':
            if (peek() == '=') {
                push(tokens, Token::MULT_EQ);
                i += 2;
            } else {
                push(tokens, Token::MULT);
                ++i;
            }
            continue;

        case '/':
            if (peek() == '=') {
                push(tokens, Token::DIV_EQ);
                i += 2;
            } else {
                push(tokens, Token::DIV);
                ++i;
            }
            continue;

        case '%':
            push(tokens, Token::MOD);
            ++i;
            continue;

        case '=':
            if (peek() == '=') {
                push(tokens, Token::EQ);
                i += 2;
            } else {
                push(tokens, Token::ASSIGN);
                ++i;
            }
            continue;

        case '>':
            if (peek() == '=') {
                push(tokens, Token::GREATER_EQ);
                i += 2;
            } else {
                push(tokens, Token::GREATER);
                ++i;
            }
            continue;

        case '<':
            if (peek() == '=') {
                push(tokens, Token::LESS_EQ);
                i += 2;
            } else {
                push(tokens, Token::LESS);
                ++i;
            }
            continue;

        case '!':
            push(tokens, Token::LOG_NOT);
            ++i;
            continue;
        case '^':
            push(tokens, Token::XOR);
            ++i;
            continue;
        case '~':
            push(tokens, Token::NOT);
            ++i;
            continue;

        case '&':
            if (peek() == '&') {
                push(tokens, Token::LOG_AND);
                i += 2;
            } else {
                push(tokens, Token::AND);
                ++i;
            }
            continue;

        case '|':
            if (peek() == '|') {
                push(tokens, Token::LOG_OR);
                i += 2;
            } else {
                push(tokens, Token::OR);
                ++i;
            }
            continue;

        case '[':
            push(tokens, Token::LBRACK);
            ++i;
            continue;
        case ']':
            push(tokens, Token::RBRACK);
            ++i;
            continue;
        case '(':
            push(tokens, Token::LPAREN);
            ++i;
            continue;
        case ')':
            push(tokens, Token::RPAREN);
            ++i;
            continue;
        case ',':
            push(tokens, Token::COMMA);
            ++i;
            continue;
        case ';':
            push(tokens, Token::SEMI);
            ++i;
            continue;

        // ── string literals ──────────────────────────────────────────────────
        case '"': {
            ++i; // skip opening quote
            size_t start = i;
            while (i < n && src[i] != '"') {
                if (src[i] == '\\' && i + 1 < n)
                    i += 2; // skip escape
                else
                    ++i;
            }
            if (i >= n)
                throw std::runtime_error("unterminated string literal");

            push(tokens, Token::STRING, src.substr(start, i - start));
            ++i; // skip closing quote
            continue;
        }

        default:
            break;
        }

        // ── numeric literals ─────────────────────────────────────────────────
        if (std::isdigit((unsigned char)c)) {
            size_t start = i;
            bool seen_dot = false;

            while (i < n && (std::isdigit((unsigned char)src[i]) ||
                             (!seen_dot && src[i] == '.'))) {
                if (src[i] == '.')
                    seen_dot = true;
                ++i;
            }

            std::string num = src.substr(start, i - start);
            push(tokens, seen_dot ? Token::FLOAT : Token::INT, std::move(num));
            continue;
        }

        // ── identifiers & keywords ───────────────────────────────────────────
        if (std::isalpha((unsigned char)c) || c == '_') {
            size_t start = i;
            while (i < n &&
                   (std::isalnum((unsigned char)src[i]) || src[i] == '_'))
                ++i;

            std::string word = src.substr(start, i - start);

            if (word == "return")
                push(tokens, Token::RETURN);
            else if (word == "int" || word == "float" || word == "string")
                push(tokens, Token::TYPE, std::move(word));
            else
                push(tokens, Token::IDENTIFIER, std::move(word));

            continue;
        }

        // ── unrecognised character ───────────────────────────────────────────
        throw std::runtime_error(std::string("unexpected character: '") + c +
                                 "'");
    }

    return tokens;
}
