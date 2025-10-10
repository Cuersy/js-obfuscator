/*
 * @Author: Cuersy 
 * @Date: 2025-10-10 18:12:24 
 * @Last Modified by:   Cuersy 
 * @Last Modified time: 2025-10-10 18:12:24 
 */
#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

enum class TokenType {
    IDENTIFIER,
    KEYWORD,
    NUMBER,
    STRING,
    OPERATOR,
    SYMBOL
};

struct Token {
    TokenType type;
    std::string value;
};

class Lexer {
public:
    Lexer(const std::string& source);
    std::vector<Token> tokenize();

private:
    std::string sourceCode;
};

#endif
