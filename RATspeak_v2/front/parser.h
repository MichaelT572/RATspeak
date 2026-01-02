#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <fstream>
#include <memory>
#include <vector>
#include <set>

#include "ast.h"

extern const std::string built_in_funcs[13];
extern const int N_FUNCS;

enum class token_type {
    ID,
    NUM,
    COMMA,
    SEMICOLON,
    L_ANGLE,
    R_ANGLE,
    EOF_T
};

class Token{
public:
    token_type type;
    std::string value;
    int line;

    Token(token_type type, std::string value, int line);
};

class Parser {
private:
    std::string text;
    std::vector<Token> tokens;
    int token_i;
    std::vector<std::string> func_list;

    void add_token(std::string value, int line);
    void tokenize();
    std::unique_ptr<Node> parse_funcdec();
    std::unique_ptr<Node> parse_callexp();
    bool func_exists(std::string id);
public:
    Parser(std::string file_name);
    std::string get_line(int n);
    std::unique_ptr<Node> build_ast();
};

void print_ast(Node* node, int depth = 0);

#endif