//
// Created by justinw on 06/04/2026.
//

#ifndef TYPHON_EXCEPTIONTHROWER_H
#define TYPHON_EXCEPTIONTHROWER_H
#include <memory>
#include <string>

#include "types/Type.h"


class ExceptionThrower {
public:
    ExceptionThrower();
    ~ExceptionThrower();

    void typeError(const std::string& description, const std::shared_ptr<Type>& expected, const std::shared_ptr<Type>& actual);
    void identifierError(const std::string& name);
    void visibilityError(const std::string& propertyName, const std::shared_ptr<Type>& type);
};


#endif //TYPHON_EXCEPTIONTHROWER_H