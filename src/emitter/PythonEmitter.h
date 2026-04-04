//
// Created by justinw on 26/03/2026.
//

#ifndef TYPHON_PYTHONEMITTER_H
#define TYPHON_PYTHONEMITTER_H
#include <memory>
#include <vector>

#include "common/Visitor.h"
#include "parser/ASTNode.h"


class PythonEmitter : Visitor {
private:
    std::vector<std::unique_ptr<ASTNode>> tree;
    std::string out;
    int indent=0;

    void indentLine();
public:
    PythonEmitter(std::vector<std::unique_ptr<ASTNode>> tree);
    ~PythonEmitter() override;

    void visit(BinaryExpr *expr) override;
    void visit(TypeNode *expr) override;
    void visit(ConditionalStatement *expr) override;
    void visit(ForStatement *expr) override;
    void visit(WhileStatement *expr) override;
    void visit(Identifier *expr) override;
    void visit(UnaryExpression *expr) override;
    void visit(Literal *expr) override;
    void visit(FunctionCallExpression *expr) override;
    void visit(GetExpression *expr) override;
    void visit(CompoundAssignExpression *expr) override;
    void visit(CompoundSetExpression *expr) override;
    void visit(AssignExpression *expr) override;
    void visit(Class *expr) override;
    void visit(Function *expr) override;
    void visit(ExtensionFunction *expr) override;
    void visit(Parameter *expr) override;
    void visit(ReturnExpression *expr) override;
    void visit(Body *expr) override;
    void begin();
    void writeToFile(const std::string& fileName);
};


#endif //TYPHON_PYTHONEMITTER_H