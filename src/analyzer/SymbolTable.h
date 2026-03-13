//
// Created by justinw on 12/03/2026.
//

#ifndef TYPHON_SYMBOL_H
#define TYPHON_SYMBOL_H
#include <memory>
#include <unordered_map>

#include "types/Type.h"
#include "common/Common.h"

struct Symbol {
    std::string name;
    const Type type;
    Visibility visibility;
    bool isInitialized;

    Symbol(std::string name, Type type, const Visibility visibility, const bool isInitialized)
        : name(std::move(name)), type(std::move(type)), visibility(visibility), isInitialized(isInitialized) {
    }

    virtual ~Symbol() = default;
};

struct ClassSymbol : Symbol {
    std::unordered_map<std::string, std::shared_ptr<Symbol>> members;
    std::string name;
    const Type type;
    Visibility visibility;

    ClassSymbol(std::string name, std::unordered_map<std::string, std::shared_ptr<Symbol>> members, Type type,
                const Visibility visibility)
        : Symbol(std::move(name), std::move(type), visibility, true), members(std::move(members)),
          name(std::move(name)), type(std::move(type)), visibility(visibility) {
        if (type.kind!=TypeKind::CLASS) throw std::runtime_error("ClassSymbol is not a class");
    }
};

class SymbolTable {
    std::vector<std::unordered_map<std::string, std::shared_ptr<Symbol>>> scopes;

public:
    SymbolTable();

    ~SymbolTable();

    void enterScope();

    void exitScope();

    void define(const std::string &name, Symbol &sym);

    std::shared_ptr<Symbol> resolve(const std::string &name);

    std::shared_ptr<Symbol> resolveClassMember(const std::string &className, const std::string &memberName);
};


#endif //TYPHON_SYMBOL_H
