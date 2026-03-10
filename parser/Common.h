//
// Created by justinw on 10/03/2026.
//

#ifndef PYC___COMMON_H
#define PYC___COMMON_H

enum Precedence {
    NONE,
    ASSIGNMENT,
    OR,
    AND,
    EQUALITY,
    COMPARISON,
    TERM,
    FACTOR,
    EXPONENT,
    UNARY,
    CALL,
    PRIMARY
};

enum class Visibility {
    PUBLIC, PRIVATE, INTERNAL
};

#endif //PYC___COMMON_H