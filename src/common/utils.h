//
// Created by justinw on 08/03/2026.
//

#ifndef TYPHON_VARIANT_UTILS_H
#define TYPHON_VARIANT_UTILS_H
#include <variant>

struct ASTNode;

template<typename ...NoMono, typename ...WithMono>
std::variant<NoMono...> extract_value(std::variant<WithMono...> &variant);

void print_node_kind(ASTNode* node);

#endif //TYPHON_VARIANT_UTILS_H