#ifndef AST_H
#define AST_H

#include <vector>
#include <string>
#include <memory>

enum class node_type {
    PROGRAM,
    LITERAL,
    FUNCDEC,
    CALLEXP
};

class Node {
public:
    node_type type;
    int line = 0;

    virtual ~Node() = default;
};

class Program : public Node {
public:
    std::vector<std::unique_ptr<Node>> body;
    Program() {
        this->type = node_type::PROGRAM;
    }
};

class Literal : public Node {
public:
    std::string value;
    Literal() {
        this->type = node_type::LITERAL;
    }
    Literal(std::string value) {
        this->type = node_type::LITERAL;
        this->value = value;
    }
};

class FuncDec : public Node {
public:
    std::string id;
    std::vector<std::unique_ptr<Node>> body;
    FuncDec() {
        this->type = node_type::FUNCDEC;
    }
};

class CallExpression : public Node {
public:
    std::string id;
    std::vector<std::unique_ptr<Node>> args;
    CallExpression() {
        this->type = node_type::CALLEXP;   
    }
    CallExpression(std::string id) {
        this->type = node_type::CALLEXP;
        this->id = id;
    }
};


#endif