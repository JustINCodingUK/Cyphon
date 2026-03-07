//
// Created by justinw on 06/03/2026.
//

#ifndef PYC___PARSER_H
#define PYC___PARSER_H
#include <memory>
#include <vector>

#include "ASTNode.h"

class Parser {
    int index = 0;
    std::vector<Token> tokens;
    Token peek();
    Token consume();
    Token consume(TokenType type, const std::string& error);
    bool match(TokenType type);
    bool isAtEnd() const;

public:
    explicit Parser(std::vector<Token> tokens);
    ~Parser();

    std::vector<std::unique_ptr<ASTNode>> parse();
    std::unique_ptr<ASTNode> classDeclaration(Visibility visibility);
    std::unique_ptr<ASTNode> functionDeclaration(Visibility visibility);
    std::unique_ptr<ASTNode> extensionFunctionDeclaration(Visibility visibility);
    std::vector<Parameter> parameterList();
    Parameter parameter();
    std::unique_ptr<ASTNode> conditionalStatement();
    std::unique_ptr<ASTNode> whileStatement();
    std::unique_ptr<ASTNode> forStatement();
    std::unique_ptr<ASTNode> block();
    std::unique_ptr<ASTNode> statement();
    std::unique_ptr<Expression> expression();
};


#endif //PYC___PARSER_H