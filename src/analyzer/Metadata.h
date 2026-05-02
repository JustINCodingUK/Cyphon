//
// Created by justinw on 20/04/2026.
//

#ifndef TYPHON_METADATA_H
#define TYPHON_METADATA_H
#include <memory>
#include <string>
#include <vector>

#include "types/Type.h"

struct Metadata {
    virtual ~Metadata() = default;
};

struct FunctionMetadata : Metadata {
    std::string name;
    std::vector<std::shared_ptr<Type> > params;
    std::shared_ptr<Type> returnType;

    FunctionMetadata(const std::string &name, const std::vector<std::shared_ptr<Type> > &params,
                     const std::shared_ptr<Type> &returnType) : name(name), params(params), returnType(returnType) {}
};

struct ClassMetadata : Metadata {
    std::string name;


};

#endif //TYPHON_METADATA_H
