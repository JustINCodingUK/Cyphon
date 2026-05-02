//
// Created by justinw on 08/03/2026.
//

#include <stdexcept>
#include "utils.h"

#include <iostream>
#include <variant>

#include "parser/ASTNode.h"

template<typename ...NoMono, typename ...WithMono>
std::variant<NoMono...> extract_value(std::variant<WithMono...> &variant) {
    return std::visit([]<typename T0>(T0&& arg) -> std::variant<WithMono... > {
        using T = std::decay_t<T0>;
        if constexpr (std::is_same_v<T, std::monostate>) {
            throw std::runtime_error("Variant contains monostate; cannot extract value.");
        } else {
            return std::variant<NoMono...>{std::forward<T0>(arg)};
        }
    }, variant);
}

void print_node_kind(ASTNode* node) {
    switch (node->kind) {
        case NodeKind::TYPE:
            std::cout << "Type Node\n";
            break;
        case NodeKind::PARAMETER:
            std::cout << "Parameter Node\n";
            break;
        case NodeKind::BODY:
            std::cout << "Body\n";
            break;
        case NodeKind::FUNCTION:
            std::cout << "Function Node\n";
            break;
        case NodeKind::EXT_FUNCTION:
            std::cout << "Extended Function Node\n";
            break;
        case NodeKind::CLASS:
            std::cout << "Class Node\n";
            break;
        case NodeKind::BINARY_EXPR:
            std::cout << "Binary Expression Node\n";
            break;
        case NodeKind::UNARY_EXPR:
            std::cout << "Unary Expression Node\n";
            break;
        case NodeKind::IF:
            std::cout << "If Node\n";
            break;
        case NodeKind::WHILE:
            std::cout << "While Node\n";
            break;
        case NodeKind::FOR:
            std::cout << "For Node\n";
            break;
        case NodeKind::IDENTIFIER:
            std::cout << "Identifier Node\n";
            break;
        case NodeKind::LITERAL:
            std::cout << "Literal Node\n";
            break;
        case NodeKind::FUNC_CALL:
            std::cout << "Function Call Node\n";
            break;
        case NodeKind::GET_EXPR:
            std::cout << "Get Expression Node\n";
            break;
        case NodeKind::COMP_ASSIGN_EXPR:
            std::cout << "Comp Assignment Expression Node\n";
            break;
        case NodeKind::COMP_SET_EXPR:
            std::cout << "Comp Set Expression Node\n";
            break;
        case NodeKind::ASSIGN_EXPR:
            std::cout << "Assign Expression Node\n";
            break;
        case NodeKind::RETURN_EXPR:
            std::cout << "Return Expression Node\n";
            break;
        case NodeKind::EXTERN:
            std::cout << "External Node\n";
            break;
    }
}