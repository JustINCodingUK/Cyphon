//
// Created by justinw on 06/04/2026.
//

#include "ExceptionThrower.h"

#include <iostream>

ExceptionThrower::ExceptionThrower() = default;

ExceptionThrower::~ExceptionThrower() = default;

void ExceptionThrower::identifierError(const std::string &name) {
    std::cerr << "Identifier Error\n";
    std::cerr << name << "\n";
    exit(1);
}

void ExceptionThrower::typeError(const std::string &description, const std::shared_ptr<Type> &expected, const std::shared_ptr<Type> &actual) {
    std::cerr << "Type Error\n";
    std::cerr << description << "\n";
    std::cerr << "Expected: " << expected->className << "\n";
    std::cerr << "Got: " << actual->className << "\n";
    exit(1);
}

void ExceptionThrower::visibilityError(const std::string &propertyName, const std::shared_ptr<Type> &type) {
    std::cerr << "Visibility Error\n";
    std::cerr << propertyName << " in type " << type->className << " is not publicly accessible" << "\n";
    exit(1);
}
