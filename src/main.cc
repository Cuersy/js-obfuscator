#include <iostream>
#include <fstream>
#include <vector>
#include "lexer.h"
#include "parser.h"
#include "obfuscator.h"

void printAST(const std::shared_ptr<ASTNode>& node, int indent = 0) {
    if (!node) return;

    for (int i = 0; i < indent; ++i) std::cout << "  ";
    std::cout << "ASTNodeType: ";
    switch (node->type) {
        case ASTNodeType::PROGRAM: std::cout << "Program"; break;
        case ASTNodeType::BLOCK: std::cout << "Block"; break;
        case ASTNodeType::IF_STATEMENT: std::cout << "IfStatement"; break;
        case ASTNodeType::WHILE_STATEMENT: std::cout << "WhileStatement"; break;
        case ASTNodeType::VARIABLE_DECLARATION: std::cout << "VariableDeclaration"; break;
        case ASTNodeType::FUNCTION_DECLARATION: std::cout << "FunctionDeclaration"; break;
        case ASTNodeType::FUNCTION_CALL: std::cout << "FuncCall"; break;
        case ASTNodeType::RETURN_STATEMENT: std::cout << "ReturnStatement"; break;
        case ASTNodeType::EXPRESSION: std::cout << "Expression"; break;
        case ASTNodeType::NUMBER: std::cout << "Number"; break;
        case ASTNodeType::STRING: std::cout << "String"; break;
        case ASTNodeType::IDENTIFIER: std::cout << "Identifier"; break;
        case ASTNodeType::MEMBER_EXPRESSION: std::cout << "MemberExpression"; break;
        default: std::cout << "Unknown"; break;
    }
    std::cout << ", Value: \"" << node->value << "\"" << std::endl;

    for (const auto& child : node->children) {
        printAST(child, indent + 1);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input.js>" << std::endl;
        return 1;
    }
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << argv[1] << std::endl;
        return 1;
    }
    std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();
    Parser parser(tokens);
    auto ast = parser.parseProgram();
    Obfuscator obfuscator;
    obfuscator.obfuscate(ast);
    std::cout << "=== Obfuscated AST === \\\\||" << std::endl;
    printAST(ast);
    std::string obfuscatedCode = obfuscator.generateObfuscatedCode(ast);
    std::ofstream outFile("../test/output.js");
    if (!outFile.is_open()) {
        std::cerr << "Error: Cannot open output.js for writing" << std::endl;
        return 1;
    }
    outFile << obfuscatedCode;
    outFile.close();

    std::cout << "Obfuscated code written to output.js" << std::endl;

    return 0;
}