//
// Created by justinw on 10/03/2026.
//

#include "SemanticAnalyzer.h"

#include <algorithm>

#include "errors/errors.h"
#include "parser/ASTNode.h"

SemanticAnalyzer::SemanticAnalyzer() = default;

void SemanticAnalyzer::visit(BinaryExpr *expr) {
    expr->left->accept(this);
    expr->right->accept(this);

    if (*expr->left->evaluatedType != *expr->right->evaluatedType) {
        throw std::runtime_error("TypeError");
    }
    expr->evaluatedType = expr->right->evaluatedType;
}

void SemanticAnalyzer::visit(ConditionalStatement *expr) {
    expr->condition->accept(this);
    expr->body->accept(this);
    expr->elseNode->accept(this);

    if (*expr->condition->evaluatedType != Type::Bool()) {
        throw std::runtime_error("TypeError");
    }
}

void SemanticAnalyzer::visit(Body *expr) {
    for (const auto& child : expr->children) child->accept(this);
}

void SemanticAnalyzer::visit(TypeNode *expr) {
    if (symbolTable.resolve(expr->typeClass)==nullptr) throw std::runtime_error("Unknown Type");
    for (const auto& type: expr->genericParams) type->accept(this);
}

void SemanticAnalyzer::visit(ForStatement *expr) {
    expr->identifier->accept(this);
    expr->iterable->accept(this);
    expr->body->accept(this);

    if (*expr->identifier->evaluatedType != *expr->iterable->evaluatedType) {
        throw std::runtime_error("TypeError");
    }
}

void SemanticAnalyzer::visit(WhileStatement *expr) {
    expr->condition->accept(this);
    expr->body->accept(this);

    if (*expr->condition->evaluatedType != Type::Bool()) {
        throw std::runtime_error("TypeError");
    }
}

void SemanticAnalyzer::visit(Identifier *expr) {
}

void SemanticAnalyzer::visit(UnaryExpression *expr) {
    expr->right->accept(this);
    expr->evaluatedType = expr->right->evaluatedType;
}

void SemanticAnalyzer::visit(Literal *expr) {
    switch (expr->token.type) {
        case TokenType::IntLiteral:
            expr->evaluatedType = std::make_shared<Type>(Type::Int());
            break;
        case TokenType::FloatLiteral:
            expr->evaluatedType = std::make_shared<Type>(Type::Float());
            break;
        case TokenType::StringLiteral:
            expr->evaluatedType = std::make_shared<Type>(Type::String());
            break;
        case TokenType::False:
        case TokenType::True:
            expr->evaluatedType = std::make_shared<Type>(Type::Bool());
            break;
        default: break;
    }
}

void SemanticAnalyzer::visit(FunctionCallExpression *expr) {
    expr->callee->accept(this);
    const auto &args = expr->args;
    for (int i = 0; i < expr->args.size(); ++i) {
        expr->args[i]->accept(this);
        if (auto &argType = *expr->evaluatedType->wrappedTypes[i + 1].get(); argType != *args[i]->evaluatedType) {
            throw std::runtime_error("TypeError");
        }
    }
}

void SemanticAnalyzer::visit(GetExpression *expr) {
    expr->left->accept(this);
    const auto &leftType = expr->left->evaluatedType;
    if (const auto& classMember = symbolTable.resolveClassMember(leftType->className, expr->propertyName);
        classMember == nullptr || classMember->visibility == Visibility::PRIVATE)
        throw std::runtime_error("Illegal Access");
}

void SemanticAnalyzer::visit(Class *expr) {
    SymbolTable classSymbolTable;
    classSymbolTable.enterScope();

    auto params = defineParameters(classSymbolTable, expr->params);
    SymbolTable &tempTable = symbolTable;
    symbolTable = classSymbolTable;

    expr->body->accept(this);

    auto classSymbolTablePointer = std::make_unique<SymbolTable>(classSymbolTable);
    auto classSymbolType = std::make_unique<Type>(Type::Class(expr->name, std::move(params)));
    const auto classSymbol = std::make_shared<ClassSymbol>(expr->name, std::move(classSymbolTablePointer),
                                                           classSymbolType, expr->visibility);
    tempTable.define(expr->name, classSymbol);
    symbolTable = tempTable;
}

void SemanticAnalyzer::visit(Function *expr) {
    symbolTable.enterScope();
    auto params = defineParameters(symbolTable, expr->params);
    expr->body->accept(this);
    symbolTable.exitScope();

    auto functionReturnType = typeFromNode(expr->returnType.get());
    auto functionType = std::make_unique<Type>(Type::Function(std::move(functionReturnType), std::move(params)));
    const auto functionSymbol = std::make_shared<Symbol>(std::move(expr->name), functionType, expr->visibility,
                                                   true);
    symbolTable.define(expr->name, functionSymbol);
}

void SemanticAnalyzer::visit(ExtensionFunction *expr) {
    symbolTable.enterScope();
    auto params = defineParameters(symbolTable, expr->params);
    expr->body->accept(this);
    // TODO: Complete
}

void SemanticAnalyzer::visit(AssignExpression *expr) {
    expr->left->accept(this);
    expr->value->accept(this);

    if (*expr->left->evaluatedType != *expr->value->evaluatedType) {
        throw std::runtime_error("TypeError");
    }
}

void SemanticAnalyzer::visit(Parameter *expr) {
    // nothing needs to be done
}

void SemanticAnalyzer::visit(CompoundSetExpression *expr) {
    expr->object->accept(this);
    expr->value->accept(this);

    if (*expr->object->evaluatedType != *expr->value->evaluatedType) {
        throw std::runtime_error("TypeError");
    }
}

void SemanticAnalyzer::visit(CompoundAssignExpression *expr) {
    auto sym = symbolTable.resolve(expr->name);
    if (sym==nullptr) throw std::runtime_error("Undeclared identifier");
    expr->value->accept(this);
    if (*expr->value->evaluatedType != *sym->type) throw std::runtime_error("TypeError");
}

std::vector<std::unique_ptr<ASTNode>> SemanticAnalyzer::analyze(std::vector<std::unique_ptr<ASTNode> > ast) {
    for (const auto& node : ast) node->accept(this);
    return ast;
}

std::vector<std::unique_ptr<Type>> SemanticAnalyzer::defineParameters(
    SymbolTable &thisSymbolTable, std::vector<Parameter> &params) {
    std::vector<std::unique_ptr<Type>> out;
    for (auto &param: params) {
        param.accept(this);
        std::vector<std::unique_ptr<Type>> genericParams;
        for (auto &generic: param.type->genericParams) {
            auto type = typeFromNode(generic.get());
            genericParams.push_back(std::move(type));
        }

        auto paramType = std::make_unique<Type>(Type::Class(param.name, std::move(genericParams)));
        const auto paramSymbol = std::make_shared<Symbol>(param.name, paramType, param.visibility, false);
        out.push_back(std::move(paramType));
        thisSymbolTable.define(paramSymbol->name, paramSymbol);
    }
    return out;
}

std::unique_ptr<Type> SemanticAnalyzer::typeFromNode(const TypeNode *node) {
    if (node->genericParams.empty()) {
        return std::make_unique<Type>(Type::Class(node->typeClass, {}));
    }
    std::vector<std::unique_ptr<Type> > generics;
    for (auto &generic: node->genericParams) {
        auto type = typeFromNode(generic.get());
        generics.push_back(std::move(type));
    };
    return std::make_unique<Type>(Type::Class(node->typeClass, std::move(generics)));
}
