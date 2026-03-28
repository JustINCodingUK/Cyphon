//
// Created by justinw on 12/03/2026.
//

#ifndef TYPHON_SYMBOL_H
#define TYPHON_SYMBOL_H
#include <unordered_map>

#include "types/Type.h"
#include "common/Common.h"

struct Symbol {
    std::string name;
    std::unique_ptr<Type>& type;
    Visibility visibility;
    bool isInitialized;

    Symbol(std::string name, std::unique_ptr<Type>& type, const Visibility visibility, const bool isInitialized)
        : name(std::move(name)), type(type), visibility(visibility), isInitialized(isInitialized) {
    }

    virtual ~Symbol() = default;
};

class SymbolTable {
    std::vector<std::unordered_map<std::string, std::shared_ptr<Symbol>>> scopes;

public:
    SymbolTable();

    ~SymbolTable();

    void enterScope();

    void exitScope();

    void define(const std::string &name, std::shared_ptr<Symbol> sym);

    std::shared_ptr<Symbol> resolve(const std::string &name);

    std::shared_ptr<Symbol> resolveClassMember(const std::string &className, const std::string &memberName);
};

struct ClassSymbol : Symbol {
    std::unique_ptr<SymbolTable> members;
    std::string name;
    std::unique_ptr<Type>& type;
    Visibility visibility;

    ClassSymbol(std::string name, std::unique_ptr<SymbolTable> members, std::unique_ptr<Type>& type,
                const Visibility visibility)
        : Symbol(std::move(name), type, visibility, true), members(std::move(members)),
          name(std::move(name)), type(type), visibility(visibility) {
        if (type->kind != TypeKind::CLASS) throw std::runtime_error("ClassSymbol is not a class");
    }
};

#endif //TYPHON_SYMBOL_H
