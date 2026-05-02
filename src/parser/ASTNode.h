//
// Created by justinw on 06/03/2026.
//

#ifndef TYPHON_ASTNODE_H
#define TYPHON_ASTNODE_H
#include <memory>
#include <utility>
#include <vector>

#include "common/Common.h"

#include "types/Type.h"
#include "lexer/Token.h"
#include "common/Visitor.h"

struct ASTNode {
    virtual ~ASTNode() = default;

    NodeKind kind;

    virtual void accept(Visitor *visitor) = 0;

    ASTNode(NodeKind kind) : kind(kind) {
    }
};

struct TypeNode : ASTNode {
    std::string typeClass;
    std::vector<std::unique_ptr<TypeNode> > genericParams;

    TypeNode(std::string typeClass, std::vector<std::unique_ptr<TypeNode> > genericParams) : ASTNode(NodeKind::TYPE),
        typeClass(std::move(typeClass)),
        genericParams(std::move(genericParams)) {
    }

    TypeNode(std::string typeClass) : ASTNode(NodeKind::TYPE), typeClass(std::move(typeClass)) {
    }

    void accept(Visitor *visitor) override {
        visitor->visit(this);
    }
};

struct Parameter : ASTNode {
    std::string name;
    std::unique_ptr<TypeNode> type;
    Visibility visibility;

    Parameter(std::string name, std::unique_ptr<TypeNode> type,
              const Visibility visibility) : ASTNode(NodeKind::PARAMETER), name(std::move(name)),
                                             type(std::move(type)), visibility(visibility) {
    }

    void accept(Visitor *visitor) override {
        visitor->visit(this);
    }
};

struct Body : ASTNode {
    std::vector<std::unique_ptr<ASTNode> > children;

    Body(std::vector<std::unique_ptr<ASTNode> > children) : ASTNode(NodeKind::BODY), children(std::move(children)) {
    }

    void accept(Visitor *visitor) override {
        visitor->visit(this);
    }
};

struct ExternNode : ASTNode {
    std::unique_ptr<Body> body;

    ExternNode(std::unique_ptr<Body> body) : ASTNode(NodeKind::EXTERN), body(std::move(body)) {
    }

    void accept(Visitor *visitor) override {
        visitor->visit(this);
    }
};

struct Function : ASTNode {
    std::string name;
    std::unique_ptr<TypeNode> returnType;
    std::vector<std::unique_ptr<Parameter> > params;
    std::vector<std::string> genericParams;
    std::unique_ptr<Body> body;
    Visibility visibility;

    Function(std::string name, std::unique_ptr<TypeNode> returnType, std::vector<std::string> genericParams,
             std::vector<std::unique_ptr<Parameter> > params, std::unique_ptr<Body> body,
             const Visibility visibility)
        : ASTNode(NodeKind::FUNCTION), name(std::move(name)), returnType(std::move(returnType)),
          params(std::move(params)),
          genericParams(std::move(genericParams)), body(std::move(body)),
          visibility(visibility) {
    }

    void accept(Visitor *visitor) override {
        visitor->visit(this);
    }
};

struct ExtensionFunction : ASTNode {
    std::string name;
    std::unique_ptr<TypeNode> returnType, extensionOn;
    std::vector<std::unique_ptr<Parameter> > params;
    std::vector<std::string> genericParams;
    std::unique_ptr<Body> body;
    Visibility visibility;

    ExtensionFunction(std::string name, std::unique_ptr<TypeNode> returnType, std::unique_ptr<TypeNode> extensionOn,
                      std::vector<std::unique_ptr<Parameter> > params, std::vector<std::string> genericParams,
                      std::unique_ptr<Body> body, const Visibility visibility)
        : ASTNode(NodeKind::EXT_FUNCTION), name(std::move(name)), returnType(std::move(returnType)),
          extensionOn(std::move(extensionOn)),
          params(std::move(params)), genericParams(std::move(genericParams)), body(std::move(body)),
          visibility(visibility) {
    }

    void accept(Visitor *visitor) override {
        visitor->visit(this);
    }
};

struct Class : ASTNode {
    std::string name;
    std::vector<std::unique_ptr<Parameter> > params;
    std::vector<std::string> genericParams;
    std::unique_ptr<Body> body;
    Visibility visibility;

    Class(std::string name, std::vector<std::unique_ptr<Parameter> > params, std::vector<std::string> genericParams,
          std::unique_ptr<Body> body, const Visibility visibility)
        : ASTNode(NodeKind::CLASS), name(std::move(name)), params(std::move(params)),
          genericParams(std::move(genericParams)),
          body(std::move(body)),
          visibility(visibility) {
    }

    void accept(Visitor *visitor) override {
        visitor->visit(this);
    }
};


struct Expression : ASTNode {
    ~Expression() override = default;

    std::shared_ptr<Type> evaluatedType;

    Expression(NodeKind kind, const std::shared_ptr<Type> &evaluatedType) : ASTNode(kind),
                                                                            evaluatedType(evaluatedType) {
    };
};

struct BinaryExpr : Expression {
    std::unique_ptr<Expression> left, right;
    Token op;

    BinaryExpr(Token op, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
        : Expression(NodeKind::BINARY_EXPR, std::make_shared<Type>(Type::Uninitialized())), left(std::move(left)),
          right(std::move(right)), op(std::move(op)) {
    }

    void accept(Visitor *visitor) override {
        visitor->visit(this);
    }
};

struct ConditionalStatement : ASTNode {
    std::unique_ptr<Expression> condition;
    std::unique_ptr<ASTNode> body;
    std::unique_ptr<ASTNode> elseNode;

    ConditionalStatement(std::unique_ptr<Expression> condition, std::unique_ptr<ASTNode> body,
                         std::unique_ptr<ASTNode> elseNode)
        : ASTNode(NodeKind::IF), condition(std::move(condition)), body(std::move(body)), elseNode(std::move(elseNode)) {
    }

    void accept(Visitor *visitor) override {
        visitor->visit(this);
    }
};

struct WhileStatement : ASTNode {
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Body> body;

    WhileStatement(std::unique_ptr<Expression> condition, std::unique_ptr<Body> body)
        : ASTNode(NodeKind::WHILE), condition(std::move(condition)), body(std::move(body)) {
    };

    void accept(Visitor *visitor) override {
        visitor->visit(this);
    }
};

struct Identifier : Expression {
    std::string name;
    bool addSelf;

    Identifier(std::string name, bool addSelf = false) : Expression(NodeKind::IDENTIFIER,
                                                                    std::make_shared<Type>(Type::Uninitialized())),
                                                         name(std::move(name)), addSelf(addSelf) {
    };

    void accept(Visitor *visitor) override {
        visitor->visit(this);
    }
};

struct ForStatement : ASTNode {
    std::unique_ptr<Identifier> identifier;
    std::unique_ptr<Expression> iterable;
    std::unique_ptr<Body> body;

    ForStatement(std::unique_ptr<Identifier> identifier, std::unique_ptr<Expression> iterable,
                 std::unique_ptr<Body> body)
        : ASTNode(NodeKind::FOR), identifier(std::move(identifier)), iterable(std::move(iterable)),
          body(std::move(body)) {
    }

    void accept(Visitor *visitor) override {
        visitor->visit(this);
    };
};

struct UnaryExpression : Expression {
    Token op;
    std::unique_ptr<Expression> right;

    UnaryExpression(Token op, std::unique_ptr<Expression> right) : Expression(NodeKind::UNARY_EXPR, std::make_shared<Type>(Type::Uninitialized())), op(std::move(op)),
                                                                   right(std::move(right)) {
    };

    void accept(Visitor *visitor) override {
        visitor->visit(this);
    }
};

struct Literal : Expression {
    Token token;

    Literal(Token token) : Expression(NodeKind::LITERAL, std::make_shared<Type>(Type::Uninitialized())), token(std::move(token)) {
    };

    void accept(Visitor *visitor) override {
        visitor->visit(this);
    }
};

struct FunctionCallExpression : Expression {
    std::unique_ptr<Expression> callee;
    std::vector<std::unique_ptr<Expression> > args;

    FunctionCallExpression(std::unique_ptr<Expression> callee, std::vector<std::unique_ptr<Expression> > args)
        : Expression(NodeKind::FUNC_CALL, std::make_shared<Type>(Type::Uninitialized())), callee(std::move(callee)), args(std::move(args)) {
    };

    void accept(Visitor *visitor) override {
        visitor->visit(this);
    }
};

struct GetExpression : Expression {
    std::unique_ptr<Expression> left;
    std::string propertyName;

    GetExpression(std::unique_ptr<Expression> left, std::string propertyName) : Expression(NodeKind::GET_EXPR, std::make_shared<Type>(Type::Uninitialized())),
        left(std::move(left)),
        propertyName(std::move(propertyName)) {
    };

    void accept(Visitor *visitor) override {
        visitor->visit(this);
    }
};

struct ReturnExpression : Expression {
    std::unique_ptr<Expression> value;

    ReturnExpression(std::unique_ptr<Expression> value) : Expression(NodeKind::RETURN_EXPR, std::make_shared<Type>(Type::Uninitialized())), value(std::move(value)) {
    }

    void accept(Visitor *visitor) override {
        visitor->visit(this);
    }
};

struct CompoundAssignExpression : Expression {
    std::string name;
    Token op;
    std::unique_ptr<Expression> value;

    CompoundAssignExpression(std::string name, Token op,
                             std::unique_ptr<Expression> value) : Expression(NodeKind::COMP_ASSIGN_EXPR, std::make_shared<Type>(Type::Uninitialized())),
                                                                  name(std::move(name)),
                                                                  op(std::move(op)), value(std::move(value)) {
    }

    void accept(Visitor *visitor) override {
        visitor->visit(this);
    }
};

struct CompoundSetExpression : Expression {
    std::unique_ptr<Expression> object;
    std::string propertyName;
    Token op;
    std::unique_ptr<Expression> value;

    CompoundSetExpression(std::unique_ptr<Expression> object, std::string propertyName, Token op,
                          std::unique_ptr<Expression> value)
        : Expression(NodeKind::COMP_SET_EXPR, std::make_shared<Type>(Type::Uninitialized())), object(std::move(object)), propertyName(std::move(propertyName)),
          op(std::move(op)), value(std::move(value)) {
    }

    void accept(Visitor *visitor) override {
        visitor->visit(this);
    }
};

struct AssignExpression : Expression {
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> value;

    AssignExpression(std::unique_ptr<Expression> left, std::unique_ptr<Expression> value)
        : Expression(NodeKind::ASSIGN_EXPR, std::make_shared<Type>(Type::Uninitialized())), left(std::move(left)), value(std::move(value)) {
    }

    void accept(Visitor *visitor) override {
        visitor->visit(this);
    }
};

#endif //TYPHON_ASTNODE_H
