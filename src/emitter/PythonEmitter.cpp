//
// Created by justinw on 26/03/2026.
//

#include "PythonEmitter.h"
#include <fstream>

PythonEmitter::PythonEmitter(std::vector<std::unique_ptr<ASTNode> > tree) : tree(std::move(tree)) {
}

PythonEmitter::~PythonEmitter() {
    tree.clear();
    out.clear();
}

void PythonEmitter::indentLine() {
    if (out.empty()) return;
    if (out.back() != '\n') return;
    for (int i = 0; i < indent * 4; ++i) out.push_back(' ');
}

void PythonEmitter::visit(Function *expr) {
    indentLine();
    out += "def " + expr->name + "(";
    for (int i = 0; i < expr->params.size(); ++i) {
        out += expr->params[i]->name;
        if (i != expr->params.size() - 1) out += ",";
    }
    out += "):\n";
    indent++;
    expr->body->accept(this);
    indent--;
}

void PythonEmitter::visit(ExtensionFunction *expr) {
    indentLine();
    out += "def " + expr->name + "(self, ";
    for (int i = 0; i < expr->params.size(); ++i) {
        out += expr->params[i]->name;
        if (i != expr->params.size() - 1) out += ",";
        else out += "):\n";
    }
    indent++;
    expr->body->accept(this);
    indent--;
}

void PythonEmitter::visit(ConditionalStatement *expr) {
    indentLine();
    out += "if(";
    expr->condition->accept(this);
    out += +"):\n";
    indent++;
    expr->body->accept(this);
    indent--;
}

void PythonEmitter::visit(TypeNode *expr) {
    // hiii
}

void PythonEmitter::visit(Literal *expr) {
    indentLine();
    out += expr->token.toString();
}

void PythonEmitter::visit(Identifier *expr, bool checkExisting) {
    indentLine();
    out += expr->name;
}

void PythonEmitter::visit(ReturnExpression *expr) {
    indentLine();
    out += "return ";
    expr->value->accept(this);
}

void PythonEmitter::visit(AssignExpression *expr) {
    indentLine();
    expr->left->accept(this);
    out += "=";
    expr->value->accept(this);
}

void PythonEmitter::visit(GetExpression *expr) {
    indentLine();
    expr->left->accept(this);
    out += ".";
    out += expr->propertyName;
}

void PythonEmitter::visit(FunctionCallExpression *expr) {
    indentLine();
    expr->callee->accept(this);
    out += "(";
    for (int i = 0; i < expr->args.size(); ++i) {
        const auto &arg = expr->args[i];
        arg->accept(this);
        if (i != expr->args.size() - 1) out += ",";
    }
    out += ")";
}

void PythonEmitter::visit(Parameter *expr) {
    out += expr->name;
}

void PythonEmitter::visit(ForStatement *expr) {
    indentLine();
    out += "for ";
    expr->identifier->accept(this);
    out += " in ";
    expr->iterable->accept(this);
    out += ":\n";
    indent++;
    expr->body->accept(this);
    indent--;
}

void PythonEmitter::visit(WhileStatement *expr) {
    indentLine();
    out += "while(";
    expr->condition->accept(this);
    out += "):\n";
    indent++;
    expr->body->accept(this);
    indent--;
}

void PythonEmitter::visit(UnaryExpression *expr) {
    indentLine();
    out += expr->op.toString();
    expr->right->accept(this);
}

void PythonEmitter::visit(ExternNode *expr) {
    // nothing
}

void PythonEmitter::visit(BinaryExpr *expr) {
    indentLine();
    expr->left->accept(this);
    out += expr->op.toString();
    expr->right->accept(this);
}

void PythonEmitter::visit(CompoundAssignExpression *expr) {
    indentLine();
    out += expr->name;
    out += expr->op.toString();
    expr->value->accept(this);
}

void PythonEmitter::visit(CompoundSetExpression *expr) {
    indentLine();
    expr->object->accept(this);
    out += expr->propertyName;
    out += expr->op.toString();
    expr->value->accept(this);
}

void PythonEmitter::visit(Class *expr) {
    indentLine();
    out += "class " + expr->name + ":\n";
    indent++;

    indentLine();
    if (!expr->params.empty()) {
        out += "def __init__(self,";
        for (const auto &param: expr->params) out += param->name + ",";
        out.pop_back();
        out += "):\n";
        indent++;

        for (const auto &param: expr->params) {
            indentLine();
            out += "self." + param->name + "=" + param->name + "\n";
        }
        out += "\n";
        indent--;
    }
    expr->body->accept(this);
    indent--;
}

void PythonEmitter::visit(Body *expr) {
    if (!expr->children.empty()) {
        for (const auto &node: expr->children) {
            node->accept(this);
            out += "\n";
        }
    } else {
        indentLine();
        out += "pass\n";
    }
}

void PythonEmitter::begin() {
    for (const auto &node: tree) {
        node->accept(this);
        out += "\n";
    }
}

void PythonEmitter::writeToFile(const std::string &fileName) {
    begin();
    std::ofstream outFile(fileName);
    outFile << out;
    outFile.close();
}
