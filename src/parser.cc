/*
 * @Author: Cuersy 
 * @Date: 2025-10-10 18:12:18 
 * @Last Modified by:   Cuersy 
 * @Last Modified time: 2025-10-10 18:12:18 
 */
#include "parser.h"
#include <iostream>

Parser::Parser(const std::vector<Token>& toks) : tokens(toks), currentIndex(0) {}

bool Parser::isAtEnd() const {
    return currentIndex >= tokens.size();
}

const Token& Parser::peek() const {
    if (isAtEnd()) throw std::runtime_error("End of tokens");
    return tokens[currentIndex];
}

void Parser::advance() {
    if (!isAtEnd()) currentIndex++;
}

bool Parser::matchToken(const std::string& val) {
    if (!isAtEnd() && tokens[currentIndex].value == val) {
        advance();
        return true;
    }
    return false;
}

bool Parser::matchTokenType(TokenType type) {
    if (!isAtEnd() && tokens[currentIndex].type == type) {
        advance();
        return true;
    }
    return false;
}

std::shared_ptr<ASTNode> Parser::parseProgram() {
    auto programNode = std::make_shared<ASTNode>(ASTNodeType::PROGRAM, "program");

    while (!isAtEnd()) {
        auto stmt = parseStatement();
        if (stmt) {
            programNode->children.push_back(stmt);
        } else {
            std::cerr << "err: UNknown node, i'll continue...\n";
            advance();
        }
    }

    return programNode;
}

std::shared_ptr<ASTNode> Parser::parseStatement() {
    if (isAtEnd()) return nullptr;

    const Token& token = peek();

    if (token.type == TokenType::KEYWORD) {
        if (token.value == "if") return parseIfStatement();
        if (token.value == "while") return parseWhileStatement();
        if (token.value == "var" || token.value == "let" || token.value == "const") return parseVariableDeclaration();
        if (token.value == "return") return parseReturnStatement();
        if (token.value == "function") return parseFunctionDeclaration();
    }

    if (token.type == TokenType::IDENTIFIER) {
        auto expr = parseExpression();
        if (!isAtEnd() && peek().value == ";") {
            advance();
        }
        return expr;
    }

    if (token.value == "{") {
        return parseBlock();
    }

    auto expr = parseExpression();
    if (!isAtEnd() && peek().value == ";") {
        advance();
    }
    return expr;
}

std::shared_ptr<ASTNode> Parser::parseFunctionDeclaration() {
    advance();
    if (isAtEnd() || peek().type != TokenType::IDENTIFIER) {
        std::cerr << "err: waiting function nanme\n";
        return nullptr;
    }
    std::string funcName = peek().value;
    advance();
    if (!matchToken("(")) {
        std::cerr << "err: waiting '('for function parameter\n";
        return nullptr;
    }
    auto funcNode = std::make_shared<ASTNode>(ASTNodeType::FUNCTION_DECLARATION, funcName);
    while (!isAtEnd() && peek().value != ")") {
        if (peek().type != TokenType::IDENTIFIER) {
            std::cerr << "err: waiting parameter name\n";
            return nullptr;
        }
        auto param = std::make_shared<ASTNode>(ASTNodeType::IDENTIFIER, peek().value);
        funcNode->children.push_back(param);
        advance();

        if (peek().value == ",") {
            advance();
        }
    }
    if (!matchToken(")")) {
        std::cerr << "err: waiting ')'\n";
        return nullptr;
    }

    auto body = parseBlock();
    if (!body) {
        std::cerr << "err: waiting function torsio\n";
        return nullptr;
    }
    funcNode->children.push_back(body);

    return funcNode;
}

std::shared_ptr<ASTNode> Parser::parseBlock() {
    if (!matchToken("{")) {
        std::cerr << "err: waiting '{' \n";
        return nullptr;
    }

    auto blockNode = std::make_shared<ASTNode>(ASTNodeType::BLOCK, "block");

    while (!isAtEnd() && peek().value != "}") {
        auto stmt = parseStatement();
        if (stmt) {
            blockNode->children.push_back(stmt);
        } else {
            advance();
        }
    }

    if (!matchToken("}")) {
        std::cerr << "err: waiting '}' \n";
    }

    return blockNode;
}

std::shared_ptr<ASTNode> Parser::parseIfStatement() {
    advance();

    if (!matchToken("(")) {
        std::cerr << "err: waiting '(' for if condition\n";
        return nullptr;
    }

    auto condition = parseExpression();

    if (!matchToken(")")) {
        std::cerr << "err: waitin ')' for if condition\n";
        return nullptr;
    }

    auto ifNode = std::make_shared<ASTNode>(ASTNodeType::IF_STATEMENT, "if");
    ifNode->children.push_back(condition);

    auto thenBlock = parseBlock();
    if (thenBlock) ifNode->children.push_back(thenBlock);

    if (!isAtEnd() && peek().value == "else") {
        advance();
        auto elseBlock = parseBlock();
        if (elseBlock) ifNode->children.push_back(elseBlock);
    }

    return ifNode;
}

std::shared_ptr<ASTNode> Parser::parseWhileStatement() {
    advance();

    if (!matchToken("(")) {
        std::cerr << "err: waiting '(' for while condifiton\n";
        return nullptr;
    }

    auto condition = parseExpression();

    if (!matchToken(")")) {
        std::cerr << "err: waiting ')' for while condition\n";
        return nullptr;
    }

    auto whileNode = std::make_shared<ASTNode>(ASTNodeType::WHILE_STATEMENT, "while");
    whileNode->children.push_back(condition);

    auto body = parseBlock();
    if (body) whileNode->children.push_back(body);

    return whileNode;
}

std::shared_ptr<ASTNode> Parser::parseVariableDeclaration() {
    std::string kind = peek().value;
    advance();
    if (isAtEnd() || peek().type != TokenType::IDENTIFIER) {
        std::cerr << "Hata: Degişken ismi bekleniyor\n";
        return nullptr;
    }

    std::string varName = peek().value;
    advance();

    std::shared_ptr<ASTNode> initExpr = nullptr;

    if (!isAtEnd() && peek().value == "=") {
        advance();
        initExpr = parseExpression();
    }

    if (!isAtEnd() && peek().value == ";") {
        advance();
    }

    auto varDeclNode = std::make_shared<ASTNode>(ASTNodeType::VARIABLE_DECLARATION, varName);
    if (initExpr) varDeclNode->children.push_back(initExpr);

    return varDeclNode;
}
std::shared_ptr<ASTNode> Parser::parseFunctionCall(std::shared_ptr<ASTNode> callee) {
    if (!matchToken("(")) {
        std::cerr << "err: waiting '(' for func calling\n";
        return nullptr;
    }

    std::string callValue = callee->value;
    if (callee->type == ASTNodeType::MEMBER_EXPRESSION) {
        callValue = callee->children[1]->value;
    }

    auto callNode = std::make_shared<ASTNode>(ASTNodeType::FUNCTION_CALL, callValue);
    callNode->children.push_back(callee);

    int argIndex = 0;

    while (!isAtEnd() && peek().value != ")") {
        auto arg = parseExpression();
        if (!arg) {
            std::cerr << "err: Unexpected argument at index " << argIndex << "\n";
            break;
        }
        callNode->children.push_back(arg);
        argIndex++;

        if (peek().value == ",") {
            advance();
        } else if (peek().value != ")") {
            std::cerr << "err: waiting ',' or ')' after argument at index " << argIndex << "\n";
            break;
        }
    }

    if (!matchToken(")")) {
        std::cerr << "err: waiting ')' for func calling \n";
        return nullptr;
    }

    if (!isAtEnd() && peek().value == ";") {
        advance();
    }

    return callNode;
}

std::shared_ptr<ASTNode> Parser::parseForLoop() {
    if (!matchToken("for")) return nullptr;

    if (!matchToken("(")) {
        std::cerr << "err: waiting '(' for for-loop\n";
        return nullptr;
    }

    auto forNode = std::make_shared<ASTNode>(ASTNodeType::FOR_LOOP, "for");

    auto init = parseStatement();
    if (!init) {
        std::cerr << "err: Invalid initialization statement in for-loop\n";
        return nullptr;
    }
    forNode->children.push_back(init);

    auto condition = parseExpression();
    if (!condition) {
        std::cerr << "err: Invalid condition in for-loop\n";
        return nullptr;
    }
    forNode->children.push_back(condition);

    if (!matchToken(";")) {
        std::cerr << "err: waiting ';' after for-loop condition\n";
        return nullptr;
    }

    auto increment = parseStatement();
    if (!increment) {
        std::cerr << "err: Invalid increment statement in for-loop\n";
        return nullptr;
    }
    forNode->children.push_back(increment);

    if (!matchToken(")")) {
        std::cerr << "err: waiting ')' after for-loop increment\n";
        return nullptr;
    }

    auto body = parseBlock();
    if (!body) {
        std::cerr << "err: Invalid block in for-loop\n";
        return nullptr;
    }
    forNode->children.push_back(body);

    return forNode;
}

std::shared_ptr<ASTNode> Parser::parseWhileLoop() {
    if (!matchToken("while")) return nullptr;

    if (!matchToken("(")) {
        std::cerr << "err: waiting '(' for while-loop\n";
        return nullptr;
    }

    auto whileNode = std::make_shared<ASTNode>(ASTNodeType::WHILE_LOOP, "while");

    auto condition = parseExpression();
    if (!condition) {
        std::cerr << "err: Invalid condition in while-loop\n";
        return nullptr;
    }
    whileNode->children.push_back(condition);

    if (!matchToken(")")) {
        std::cerr << "err: waiting ')' after while-loop condition\n";
        return nullptr;
    }

    auto body = parseBlock();
    if (!body) {
        std::cerr << "err: Invalid block in while-loop\n";
        return nullptr;
    }
    whileNode->children.push_back(body);

    return whileNode;
}

std::shared_ptr<ASTNode> Parser::parseReturnStatement() {
    advance();
    auto expr = parseExpression();

    if (!isAtEnd() && peek().value == ";") {
        advance();
    }

    auto returnNode = std::make_shared<ASTNode>(ASTNodeType::RETURN_STATEMENT, "return");
    if (expr) returnNode->children.push_back(expr);

    return returnNode;
}

std::shared_ptr<ASTNode> Parser::parseExpression() {
    auto left = parseTerm();

    while (!isAtEnd() && (peek().value == "+" || peek().value == "-")) {
        std::string op = peek().value;
        advance();

        auto right = parseTerm();
        if (!right) {
            std::cerr << "err: waiting right operand\n";
            return left;
        }

        auto exprNode = std::make_shared<ASTNode>(ASTNodeType::EXPRESSION, op);
        exprNode->children.push_back(left);
        exprNode->children.push_back(right);
        left = exprNode;
    }

    return left;
}

std::shared_ptr<ASTNode> Parser::parseTerm() {
    auto left = parseFactor();

    while (!isAtEnd() && (peek().value == "*" || peek().value == "/")) {
        std::string op = peek().value;
        advance();

        auto right = parseFactor();
        if (!right) {
            std::cerr << "err: waiting right operand\n";
            return left;
        }

        auto exprNode = std::make_shared<ASTNode>(ASTNodeType::EXPRESSION, op);
        exprNode->children.push_back(left);
        exprNode->children.push_back(right);
        left = exprNode;
    }

    return left;
}

std::shared_ptr<ASTNode> Parser::parseFactor() {
    return parseMemberExpression();
}

std::shared_ptr<ASTNode> Parser::parseMemberExpression() {
    auto object = parsePrimary();

    while (!isAtEnd() && peek().value == ".") {
        advance();

        if (isAtEnd() || peek().type != TokenType::IDENTIFIER) {
            std::cerr << "err: waaiting identifier\n";
            return object;
        }

        auto property = std::make_shared<ASTNode>(ASTNodeType::IDENTIFIER, peek().value);
        advance();

        auto memberNode = std::make_shared<ASTNode>(ASTNodeType::MEMBER_EXPRESSION, ".");
        memberNode->children.push_back(object);
        memberNode->children.push_back(property);

        if (!isAtEnd() && peek().value == "(") {
            return parseFunctionCall(memberNode);
        }

        object = memberNode;
    }

    return object;
}

std::shared_ptr<ASTNode> Parser::parsePrimary() {
    if (isAtEnd()) return nullptr;

    const Token& token = peek();

    if (token.type == TokenType::NUMBER) {
        auto node = std::make_shared<ASTNode>(ASTNodeType::NUMBER, token.value);
        advance();
        return node;
    }

    if (token.type == TokenType::STRING) {
        auto node = std::make_shared<ASTNode>(ASTNodeType::STRING, token.value);
        advance();
        return node;
    }

    if (token.type == TokenType::IDENTIFIER) {
        auto node = std::make_shared<ASTNode>(ASTNodeType::IDENTIFIER, token.value);
        advance();
        if (!isAtEnd() && peek().value == "(") {
            return parseFunctionCall(node);
        }
        return node;
    }

    if (token.value == "(") {
        advance();
        auto expr = parseExpression();
        if (!expr) {
            std::cerr << "err: unknown val\n";
            return nullptr;
        }
        if (!matchToken(")")) {
            std::cerr << "err: waiting ')'\n";
            return nullptr;
        }
        return expr;
    }

    std::cerr << "err: unexpected token: " << token.value << "\n";
    advance();
    return nullptr;
}
std::shared_ptr<ASTNode> Parser::parseString() {
    if (isAtEnd() || peek().type != TokenType::STRING) {
        return nullptr;
    }
    
    auto node = std::make_shared<ASTNode>(ASTNodeType::STRING, peek().value);
    advance();
    
    while (!isAtEnd() && peek().value == "+") {
        advance();
        if (isAtEnd() || peek().type != TokenType::STRING) {
            std::cerr << "Error: Expected string after '+' for concatenation\n";
            return node;
        }
        
        auto right = std::make_shared<ASTNode>(ASTNodeType::STRING, peek().value);
        advance();
        
        auto concatNode = std::make_shared<ASTNode>(ASTNodeType::BINARY_EXPRESSION, "+");
        concatNode->children.push_back(node);
        concatNode->children.push_back(right);
        node = concatNode;
    }
    
    return node;
}