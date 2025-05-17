#include "obfuscator.h"
#include <sstream>
#include<iostream>
#include <iomanip>

Obfuscator::Obfuscator() : nameCounter(0), stringCounter(0) {
    reservedNames = {"console", "log"};
}

std::string Obfuscator::generateNewName() {
    std::stringstream ss;
    ss << "_0x" << std::hex << std::setw(6) << std::setfill('0') << (nameCounter++ * 0x1234 % 0xFFFFFF);
    return ss.str();
}

std::string Obfuscator::getObfuscatedName(const std::string& original) {
    if (reservedNames.count(original)) {
        return original;
    }
    if (nameMap.find(original) == nameMap.end()) {
        nameMap[original] = generateNewName();
    }
    return nameMap[original];
}

std::string Obfuscator::getStringIndex(const std::string& str) {
    if (stringTable.find(str) == stringTable.end()) {
        stringTable[str] = stringCounter++;
        stringList.push_back(str);
    }
    return "0x" + std::to_string(stringTable[str]);
}

std::string Obfuscator::obfuscateNumber(const std::string& num) {
    try {
        long value = std::stol(num);
        return std::to_string(value);
    } catch (...) {
        return num;
    }
}

void Obfuscator::obfuscateNode(std::shared_ptr<ASTNode> node) {
    if (!node) return;
    if (node->type == ASTNodeType::IDENTIFIER || 
        node->type == ASTNodeType::FUNCTION_DECLARATION) {
        node->value = getObfuscatedName(node->value);
    } else if (node->type == ASTNodeType::NUMBER) {
        node->value = obfuscateNumber(node->value);
    }
    for (auto& child : node->children) {
        obfuscateNode(child);
    }
}

void Obfuscator::obfuscate(std::shared_ptr<ASTNode> ast) {
    obfuscateNode(ast);
}

std::string Obfuscator::generateStringTable(std::string& funcName) {
    std::string tableName = generateNewName();
    funcName = generateNewName();
    std::stringstream ss;
    ss << "var " << tableName << "=['";
    for (size_t i = 0; i < stringList.size(); ++i) {
        if (i > 0) ss << "','";
        ss << stringList[i];
    }
    ss << "'];";
    ss << "var " << funcName << "=function(_0x1){return " << tableName << "[_0x1];};";
    return ss.str();
}

std::string Obfuscator::generateCode(std::shared_ptr<ASTNode> node, int indent) {
    if (!node) return "";

    std::stringstream ss;
    std::string indentStr(indent * 2, ' ');
    static std::string stringFunc;

    switch (node->type) {
        case ASTNodeType::PROGRAM: {
            stringFunc.clear();
            ss << "(async () => {\n";
            if (!stringList.empty()) {
                ss << "  " << generateStringTable(stringFunc) << "\n";
            }
            for (const auto& child : node->children) {
                ss << generateCode(child, 1);
            }
            ss << "})(0x1,(0xB-0x2));\n";
            break;
        }
        case ASTNodeType::FUNCTION_DECLARATION: {
            ss << indentStr << "function " << node->value << "(";
            bool first = true;
            for (size_t i = 0; i < node->children.size() - 1; ++i) {
                if (!first) ss << ",";
                ss << generateCode(node->children[i], 0);
                first = false;
            }
            ss << ") {\n";
            ss << generateCode(node->children.back(), indent + 1);
            ss << indentStr << "}\n";
            break;
        }
        case ASTNodeType::BLOCK: {
            for (const auto& child : node->children) {
                ss << generateCode(child, indent);
            }
            break;
        }
        case ASTNodeType::RETURN_STATEMENT: {
            ss << indentStr << "return ";
            if (!node->children.empty()) {
                ss << generateCode(node->children[0], 0);
            }
            ss << ";\n";
            break;
        }
        case ASTNodeType::FUNCTION_CALL: {
            ss << indentStr;
            if (node->children[0]->type == ASTNodeType::MEMBER_EXPRESSION) {
                auto member = node->children[0];
                if (!stringFunc.empty()) {
                    ss << generateCode(member->children[0], 0) << "[" << stringFunc << "(";
                    ss << getStringIndex(member->children[1]->value) << ")]";
                } else {
                    ss << generateCode(member->children[0], 0) << "." << generateCode(member->children[1], 0);
                }
            } else {
                ss << generateCode(node->children[0], 0);
            }
            ss << "(";
            for (size_t i = 1; i < node->children.size(); ++i) {
                if (i > 1) ss << ",";
                if (node->children[i]->type == ASTNodeType::FUNCTION_CALL) {
                    std::string argCode = generateCode(node->children[i], 0);
                    if (argCode.size() >= 2 && argCode.substr(argCode.size() - 2) == ";\n") {
                        argCode = argCode.substr(0, argCode.size() - 2);
                    }
                    ss << argCode;
                } else {
                    ss << generateCode(node->children[i], 0);
                }
            }
            ss << ")";
            if (indent > 0) {
                ss << ";\n";
            }
            break;
        }
        case ASTNodeType::MEMBER_EXPRESSION: {
            if (!stringFunc.empty()) {
                ss << generateCode(node->children[0], 0) << "[" << stringFunc << "(";
                ss << getStringIndex(node->children[1]->value) << ")]";
            } else {
                ss << generateCode(node->children[0], 0) << "." << generateCode(node->children[1], 0);
            }
            break;
        }
        case ASTNodeType::EXPRESSION: {
            ss << "(" << generateCode(node->children[0], 0) << node->value 
               << generateCode(node->children[1], 0) << ")";
            break;
        }
        case ASTNodeType::IDENTIFIER:
        case ASTNodeType::NUMBER: {
            std::cout << std::endl <<node->value << "added..." << std::endl;
            ss << node->value;
            break;
        }
        default:
            break;
    }
    return ss.str();
}

std::string Obfuscator::generateObfuscatedCode(std::shared_ptr<ASTNode> ast) {
    return generateCode(ast, 0);
}