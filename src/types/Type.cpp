//
// Created by justinw on 12/03/2026.
//

#include "Type.h"

#include <iostream>

Type Type::Int() {
    std::vector<std::shared_ptr<Type> > wrappedTypes;
    return {TypeKind::INT, "", wrappedTypes};
}

Type Type::Float() {
    std::vector<std::shared_ptr<Type> > wrappedTypes;
    return {TypeKind::FLOAT, "", wrappedTypes};
}

Type Type::String() {
    std::vector<std::shared_ptr<Type> > wrappedTypes;
    return {TypeKind::STRING, "", wrappedTypes};
}

Type Type::Bool() {
    std::vector<std::shared_ptr<Type> > wrappedTypes;
    return {TypeKind::BOOL, "", wrappedTypes};
}

Type Type::None() {
    std::vector<std::shared_ptr<Type> > wrappedTypes;
    return {TypeKind::NONE, "", wrappedTypes};
}

Type Type::ClassSymbol(const std::string &className, const std::vector<std::shared_ptr<Type>>& wrappedTypes) {
    return {TypeKind::CLASS_SYM, className, wrappedTypes};
}

Type Type::Class(std::string className, std::vector<std::shared_ptr<Type> > wrappedTypes) {
    return {TypeKind::CLASS, std::move(className), wrappedTypes};
}

Type Type::Function(std::shared_ptr<Type> returnType, std::vector<std::shared_ptr<Type> > paramTypes, std::string& name) {
    std::vector<std::shared_ptr<Type> > vec;
    vec.push_back(std::move(returnType));
    vec.insert(vec.end(), std::make_move_iterator(paramTypes.begin()), std::make_move_iterator(paramTypes.end()));
    return {TypeKind::FUNCTION, name, vec};
}

Type Type::Any() {
    std::vector<std::shared_ptr<Type> > wrappedTypes;
    return {TypeKind::ANY, "", wrappedTypes};
}

Type Type::Error() {
    std::vector<std::shared_ptr<Type> > wrappedTypes;
    return {TypeKind::ERROR, "", wrappedTypes};
}

Type Type::Uninitialized() {
    std::vector<std::shared_ptr<Type> > wrappedTypes;
    return {TypeKind::UNINITIALIZED, "", wrappedTypes};
}

bool Type::operator==(const Type &other) const {
    if (kind != other.kind) return false;
    if (kind == TypeKind::CLASS || kind == TypeKind::FUNCTION) {
        if (kind == TypeKind::CLASS && className != other.className) return false;
        if (wrappedTypes.size() != other.wrappedTypes.size()) return false;
        for (int i = 0; i < wrappedTypes.size(); ++i) {
            if (*wrappedTypes[i] != *other.wrappedTypes[i]) return false;
        }
    }
    return true;
}

void print_type(Type *type) {
    switch (type->kind) {
        case TypeKind::INT:
            std::cout << "Int\n";
            break;
        case TypeKind::FLOAT:
            std::cout << "Float\n";
            break;
        case TypeKind::STRING:
            std::cout << "String\n";
            break;
        case TypeKind::BOOL:
            std::cout << "Bool\n";
            break;
        case TypeKind::NONE:
            std::cout << "None\n";
            break;
        case TypeKind::CLASS:
            std::cout << "Class type:"+type->className+"\n";
            break;
        case TypeKind::CLASS_SYM:
            std::cout << "Class sym:"+type->className+"\n";
            break;
        case TypeKind::FUNCTION:
            std::cout << "Function type:"+type->className+"\n";
            break;
        case TypeKind::ERROR:
            std::cout << "Error type\n";
            break;
        case TypeKind::ANY:
            std::cout << "Any\n";
            break;
        case TypeKind::UNINITIALIZED:
            std::cout << "Uninitialized\n";
            break;
    }
}
