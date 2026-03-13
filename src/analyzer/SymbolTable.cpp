//
// Created by justinw on 12/03/2026.
//

#include "SymbolTable.h"

SymbolTable::SymbolTable() {
    enterScope();
}

SymbolTable::~SymbolTable() = default;

void SymbolTable::enterScope() {
    scopes.emplace_back();
}

void SymbolTable::exitScope() {
    scopes.pop_back();
}

void SymbolTable::define(const std::string &name, Symbol &sym) {
    scopes.back().emplace(name, sym);
}

std::shared_ptr<Symbol> SymbolTable::resolve(const std::string &name) {
    int currentIndex = scopes.size()-1;
    while (currentIndex>=0) {
        if (scopes[currentIndex].contains(name)) {
            return scopes[currentIndex].at(name);
        }
        currentIndex--;
    }
    return nullptr;
}

std::shared_ptr<Symbol> SymbolTable::resolveClassMember(const std::string &className, const std::string &memberName) {
    const auto classSymbol = std::dynamic_pointer_cast<ClassSymbol>(resolve(className));
    if (classSymbol == nullptr) return nullptr;
    if (classSymbol->members.contains(memberName)) {
        return classSymbol->members.at(memberName);
    }
    return nullptr;
}

