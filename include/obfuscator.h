/*
 * @Author: Cuersy 
 * @Date: 2025-10-10 18:12:26 
 * @Last Modified by:   Cuersy 
 * @Last Modified time: 2025-10-10 18:12:26 
 */
#ifndef OBFUSCATOR_H
#define OBFUSCATOR_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "parser.h"

class Obfuscator {
private:
    std::unordered_map<std::string, std::string> nameMap;
    std::unordered_map<std::string, int> stringTable; 
    std::vector<std::string> stringList;
    int nameCounter;
    int stringCounter;
    std::unordered_set<std::string> reservedNames;

    std::string generateNewName();
    std::string getObfuscatedName(const std::string& original);
    std::string getStringIndex(const std::string& str);
    std::string obfuscateNumber(const std::string& num);
    void obfuscateNode(std::shared_ptr<ASTNode> node);
    std::string generateCode(std::shared_ptr<ASTNode> node, int indent = 0);
    std::string generateStringTable(std::string& funcName);

public:
    Obfuscator();
    void obfuscate(std::shared_ptr<ASTNode> ast);
    std::string generateObfuscatedCode(std::shared_ptr<ASTNode> ast);
};

#endif