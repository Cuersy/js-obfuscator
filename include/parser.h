/*
 * @Author: Cuersy 
 * @Date: 2025-10-10 18:12:28 
 * @Last Modified by:   Cuersy 
 * @Last Modified time: 2025-10-10 18:12:28 
 */
#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <memory>
#include "lexer.h"

enum class ASTNodeType {
    PROGRAM,
    FUNCTION_DECLARATION,
    BLOCK,
    RETURN_STATEMENT,
    VARIABLE_DECLARATION,
    FUNCTION_CALL,
    MEMBER_EXPRESSION,
    EXPRESSION,
    IDENTIFIER,
    NUMBER,
    STRING,
    KEYWORD,
    IF_STATEMENT,
    WHILE_STATEMENT,
    FOR_LOOP,
    WHILE_LOOP,
    BINARY_EXPRESSION
};

struct ASTNode {
    ASTNodeType type;
    std::string value;
    std::vector<std::shared_ptr<ASTNode>> children;

    ASTNode(ASTNodeType type, const std::string& value) : type(type), value(value) {}
};

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    std::shared_ptr<ASTNode> parseProgram();

private:
    std::vector<Token> tokens;
    size_t currentIndex;

    bool isAtEnd() const;
    const Token& peek() const;
    void advance();
    bool matchToken(const std::string& val);
    bool matchTokenType(TokenType type);

    std::shared_ptr<ASTNode> parseStatement();
    std::shared_ptr<ASTNode> parseFunctionDeclaration();
    std::shared_ptr<ASTNode> parseBlock();
    std::shared_ptr<ASTNode> parseIfStatement();
    std::shared_ptr<ASTNode> parseWhileStatement();
    std::shared_ptr<ASTNode> parseVariableDeclaration();
    std::shared_ptr<ASTNode> parseFunctionCall(std::shared_ptr<ASTNode> callee);
    std::shared_ptr<ASTNode> parseReturnStatement();
    std::shared_ptr<ASTNode> parseExpression();
    std::shared_ptr<ASTNode> parseTerm();
    std::shared_ptr<ASTNode> parseFactor();
    std::shared_ptr<ASTNode> parseMemberExpression();
    std::shared_ptr<ASTNode> parsePrimary();
    std::shared_ptr<ASTNode> parseForLoop();
    std::shared_ptr<ASTNode> parseWhileLoop();
    std::shared_ptr<ASTNode> parseString();
};

#endif