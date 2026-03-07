#include "Parser.h"

Parser::Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {
}

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
    perror((error+" at line "+std::to_string(tokens[index].line)+" column "+std::to_string(tokens[index].column)+"\n").c_str());
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

std::unique_ptr<ASTNode> Parser::functionDeclaration(Visibility visibility) {
    consume(TokenType::Def, "Expected function definition");
    Token name = consume(TokenType::Identifier, "Expected function name");
    auto parameters = parameterList();
    consume(TokenType::Arrow, "Expected return type");
    Token returnType = consume(TokenType::Identifier, "Expected return type");
    consume(TokenType::LBrace, "Expected '{'");
    auto body = block();

    return std::make_unique<Function>(name.toString(), returnType.toString(), parameters, std::move(body), visibility);
}

std::unique_ptr<ASTNode> Parser::classDeclaration(Visibility visibility) {
    consume(TokenType::Class, "Expected class definition");
    Token name = consume(TokenType::Identifier, "Expected class name");
    auto parameters = parameterList();
    consume(TokenType::LBrace, "Expected '{'");
    auto body = block();

    return std::make_unique<Class>(name.toString(), parameters, std::move(body), visibility);
}

std::unique_ptr<ASTNode> Parser::extensionFunctionDeclaration(Visibility visibility) {
    consume(TokenType::Ext, "Expected extension function definition");
    consume(TokenType::Def, "Expected extension function definition");
    auto extensionOn = consume(TokenType::Identifier, "Expected extension type");
    consume(TokenType::Dot, "Expected '.'");
    Token name = consume(TokenType::Identifier, "Expected extension function name");
    auto parameters = parameterList();
    consume(TokenType::Arrow, "Expected return type");
    Token returnType = consume(TokenType::Identifier, "Expected return type");
    consume(TokenType::LBrace, "Expected '{'");
    auto body = block();

    return std::make_unique<ExtensionFunction>(name.toString(), returnType.toString(), extensionOn.toString(),
                                               parameters,
                                               std::move(body), visibility);
}

std::unique_ptr<ASTNode> Parser::conditionalStatement() {
    consume(TokenType::If, "Expected if");
    auto condition = expression();
    consume(TokenType::LBrace, "Expected '{'");
    auto body = block();
    std::unique_ptr<ASTNode> elseNode;
    if (match(TokenType::Else)) {
        consume(TokenType::Else, "Expected else");
        if (match(TokenType::If)) {
            elseNode = conditionalStatement();
        } else {
            consume(TokenType::LBrace, "Expected body");
            elseNode = block();
        }
    }
    return std::make_unique<ConditionalStatement>(std::move(condition), std::move(body), std::move(elseNode));
}

std::unique_ptr<ASTNode> Parser::forStatement() {
    consume(TokenType::For, "Expected for loop declaration");
    auto identifier = consume(TokenType::Identifier, "Expected identifier");
    consume(TokenType::In, "Expected in");
    auto iterable = expression();
    consume(TokenType::LBrace, "Expected '{'");
    auto body = block();
    return std::make_unique<ForStatement>(identifier.toString(), std::move(iterable), std::move(body));
}

std::unique_ptr<ASTNode> Parser::whileStatement() {
    consume(TokenType::While, "Expected while loop declaration");
    auto condition = expression();
    consume(TokenType::LBrace, "Expected '{'");
    auto body = block();
    return std::make_unique<WhileStatement>(std::move(condition), std::move(body));
}

std::unique_ptr<ASTNode> Parser::block() {
    auto visibilityModifier = Visibility::PUBLIC;
    switch (peek().type) {
        case TokenType::Public:
            consume(TokenType::Public, "Expected visibility modifier");
            visibilityModifier = Visibility::PUBLIC;
        case TokenType::Private:
            consume(TokenType::Private, "Expected visibility modifier");
            visibilityModifier = Visibility::PRIVATE;
            break;
        case TokenType::Internal:
            consume(TokenType::Internal, "Expected visibility modifier");
            visibilityModifier = Visibility::INTERNAL;
            break;
        default:;
    }

    switch (peek().type) {
        case TokenType::If:
            return conditionalStatement();
        case TokenType::For:
            return forStatement();
        case TokenType::While:
            return whileStatement();
        case TokenType::Def:
            return functionDeclaration(visibilityModifier);
        case TokenType::Ext:
            return extensionFunctionDeclaration(visibilityModifier);
        case TokenType::Class:
            return classDeclaration(visibilityModifier);
        case TokenType::Newline:
            consume();
            return block();
        default:
            return expression();
    }
}

std::unique_ptr<Expression> Parser::expression() {
    Token identifier;
    if (match(TokenType::Identifier) || match(TokenType::Literal)) identifier = consume();
    else {
        perror("This shouldn't happen");
        exit(100);
    }

    if (match(TokenType::LParen)) {
        std::vector<std::unique_ptr<Expression>> args;
        while (!match(TokenType::RParen)) {
            auto nextArg = expression();
            args.push_back(std::move(nextArg));
        }
        consume(TokenType::RParen, "Expected ')'");
        return std::make_unique<FunctionCall>("", identifier.toString(), std::move(args));
    }

    if (match(TokenType::Dot)) {
        consume(TokenType::Dot, "This shouldn't happen");
        auto functionName = consume(TokenType::Identifier, "Expected function call");
        consume(TokenType::LParen, "Expected function call");
        std::vector<std::unique_ptr<Expression>> args;

        while (!match(TokenType::RParen)) {
            auto nextArg = expression();
            args.push_back(std::move(nextArg));
        }
        consume(TokenType::RParen, "Expected ')'");
        return std::make_unique<FunctionCall>(identifier.toString(), functionName.toString(), std::move(args));
    }

    


}
