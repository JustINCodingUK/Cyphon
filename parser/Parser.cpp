#include "Parser.h"

Parser::Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {}

Parser::~Parser() = default;

bool Parser::isAtEnd() const {
    return tokens[index].type == TokenType::EndOfFile;
}

Token Parser::consume() {
    if (!isAtEnd()) index++;
    return tokens[index - 1];
}

Token Parser::consume(TokenType type, const std::string &error) {
    if (match(type)) return consume();
    perror(error.c_str());
    exit(100);
}

bool Parser::match(TokenType type) {
    if (tokens[index].type == type) {
        consume();
        return true;
    }
    return false;
}

Token Parser::peek() {
    return tokens[index];
}

std::vector<std::unique_ptr<ASTNode> > Parser::parse() {
}

Parameter Parser::parameter() {
    Token name = consume(TokenType::Identifier, "Expected parameter name");
    consume(TokenType::Colon, "Expected parameter declaration");
    Token type = consume(TokenType::Identifier, "Expected parameter type");
    return {name.toString(), type.toString()};
}

std::vector<Parameter> Parser::parameterList() {
    consume(TokenType::LParen, "Expected '('");
    std::vector<Parameter> params;
    if (!match(TokenType::RParen)) {
        params.push_back(parameter());
        while (match(TokenType::Comma)) params.push_back(parameter());
    }
    return params;
}

std::unique_ptr<ASTNode> Parser::functionDeclaration() {
    consume(TokenType::Def, "Expected function definition");
    Token name = consume(TokenType::Identifier, "Expected function name");
    auto parameters = parameterList();
    consume(TokenType::Arrow, "Expected return type");
    Token returnType = consume(TokenType::Identifier, "Expected return type");
    consume(TokenType::LBrace, "Expected '{'");
    auto body = block();

    return std::make_unique<Function>(name.toString(), returnType.toString(), parameters, std::move(body));
}

std::unique_ptr<ASTNode> Parser::classDeclaration() {
    consume(TokenType::Class, "Expected class definition");
    Token name = consume(TokenType::Identifier, "Expected class name");
    auto parameters = parameterList();
    consume(TokenType::LBrace, "Expected '{'");
    auto body = block();

    return std::make_unique<Class>(name.toString(), parameters, std::move(body));
}

std::unique_ptr<ASTNode> Parser::extensionFunctionDeclaration() {
    consume(TokenType::Ext, "Expected extension function definition");
    consume(TokenType::Def, "Expected extension function definition");
    consume(TokenType::Identifier, "Expected extension type");
    consume(TokenType::Dot, "Expected '.'");
    Token name = consume(TokenType::Identifier, "Expected extension function name");
    auto parameters = parameterList();
    consume(TokenType::Arrow, "Expected return type");
    Token returnType = consume(TokenType::Identifier, "Expected return type");
    consume(TokenType::LBrace, "Expected '{'");
    auto body = block();

    return std::make_unique<ExtensionFunction>(name.toString(), returnType.toString(), name.toString(), parameters,
                                               std::move(body));
}

std::unique_ptr<ASTNode> Parser::conditionalStatement() {
    consume(TokenType::If, "Expected if");
    auto condition = expression();
    consume(TokenType::LBrace, "Expected '{'");
    auto body = block();
    std::unique_ptr<ASTNode> elseNode;
    if (match(TokenType::Else)) {
        consume(TokenType::Else, "Expected else");

    }
    return std::make_unique<ConditionalStatement>(std::move(condition), std::move(body), std::move(elseNode));
}

std::unique_ptr<BinaryExpr> Parser::expression() {

}
