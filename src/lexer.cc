/*
 * @Author: Cuersy 
 * @Date: 2025-10-10 18:12:11 
 * @Last Modified by:   Cuersy 
 * @Last Modified time: 2025-10-10 18:12:11 
 */
#include "../include/lexer.h"
#include <regex>
#include <string>
#include <vector>
#include <iostream>
#include <cctype>

Lexer::Lexer(const std::string& source) : sourceCode(source) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    const std::vector<std::string> keywords = {
        "if", "else", "for", "while", "return", "function",
        "const", "let", "var", "async", "await", "class", "new", "this", "super"
    };

    std::regex keywordRegex(R"((if|else|for|while|return|function|const|let|var|async|await|class|new|this|super)\b)");
    std::regex identifierRegex(R"([a-zA-Z_][a-zA-Z0-9_]*)");
    std::regex numberRegex(R"(\d+(\.\d+)?([eE][+-]?\d+)?)");
    std::regex stringRegex(R"("([^"\\]|\\.)*"|'([^'\\]|\\.)*')");
    std::regex templateStringRegex(R"(`([^`\\]|\\.)*`)");
    std::regex operatorRegex(R"(===|!==|>>>=|>>>|>>=|<<=|==|!=|<=|>=|\+\+|--|\+|-|\*|\/|%|=|<|>|\!|&&|\|\||\?|:|\^|&|\||~)");
    std::regex symbolRegex(R"([{}()\[\];,\.])");

    size_t pos = 0;
    while (pos < sourceCode.length()) {
        if (isspace(sourceCode[pos])) {
            pos++;
            continue;
        }

        std::string remaining = sourceCode.substr(pos);
        std::smatch match;

        if (std::regex_search(remaining, match, keywordRegex) && match.position() == 0) {
            tokens.push_back({ TokenType::KEYWORD, match.str() });
            pos += match.length();
            continue;
        }
        if (std::regex_search(remaining, match, identifierRegex) && match.position() == 0) {
            tokens.push_back({ TokenType::IDENTIFIER, match.str() });
            pos += match.length();
            continue;
        }
        if (std::regex_search(remaining, match, numberRegex) && match.position() == 0) {
            tokens.push_back({ TokenType::NUMBER, match.str() });
            pos += match.length();
            continue;
        }
        if (std::regex_search(remaining, match, stringRegex) && match.position() == 0) {
            tokens.push_back({ TokenType::STRING, match.str() });
            pos += match.length();
            continue;
        }
        if (std::regex_search(remaining, match, templateStringRegex) && match.position() == 0) {
            tokens.push_back({ TokenType::STRING, match.str() });
            pos += match.length();
            continue;
        }
        if (std::regex_search(remaining, match, operatorRegex) && match.position() == 0) {
            tokens.push_back({ TokenType::OPERATOR, match.str() });
            pos += match.length();
            continue;
        }
        if (std::regex_search(remaining, match, symbolRegex) && match.position() == 0) {
            tokens.push_back({ TokenType::SYMBOL, match.str() });
            pos += match.length();
            continue;
        }

        std::cerr << "Unknown token: " << sourceCode[pos] << std::endl;
        pos++;
    }

    return tokens;
}
