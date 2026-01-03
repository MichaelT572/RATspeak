#include "parser.h"

const std::string built_in_funcs[13] = {"🐀", "🧀", "💊", "🕳", "🏭", "🧪", "🧱", "🗑", "🔼", "🧵", "📡", "☣", "tile"};
const int N_FUNCS = 13;

Token::Token(token_type type, std::string value, int line) {
    this->type = type;
    this->value = value;
    this->line = line;
}

std::string load_file(std::string file_name, std::set<std::string>* seen) {
    std::ifstream file(file_name);

    if (!file.is_open()) {
        throw std::runtime_error("File \"" + file_name + "\" not found");
    }

    if (seen->count(file_name)) {
        return "";
    }
    seen->insert(file_name);

    std::string text;
    std::string line;
    bool first = true;
    while (std::getline(file, line)) {
        std::string temp = line;
        temp.erase(0, temp.find_first_not_of(" \t\r"));
        if (temp.find("#sniff", 0) == 0) {
            int q1 = temp.find('<');
            int q2 = temp.find('>');

            if ((q1 == std::string::npos) || (q2 == std::string::npos)) {
                throw std::runtime_error("Invalid #sniff:\n\t" + line);
            }
            std::string include_file = temp.substr(q1 + 1, q2 - (q1 + 1));

            text += load_file(include_file, seen);
            text += "\n";
        } else {
            if (!first) {
                text += "\n";
            }
            text += line;
            first = false;
        }
    }

    return text;

}

bool is_whitespace(char c) {
    return ((c == ' ') || (c == '\t') || (c == '\r'));
}
bool is_next(char c) {
    return ((c == ',') || (c == ';') || (c == '<') || (c == '>'));
}
bool is_num(std::string s) {
    try {
        std::stoi(s);

        return true;
    } catch (...) {
        return false;
    }
}
int has_arg_count(std::string id, int n) {
    if (id == "tile") {
        if ((n != 5)) {
            return false;
        }
    } else if ((id == "🧱") || (id == "🧵")) {
        if ((n != 2) && (n != 4)) {
            return false;
        }
    } else if ((id == "🐀") || (id == "🧀") || (id == "💊") || (id == "🕳") || (id == "🏭") || (id == "🧪") || (id == "📡")) {
        if ((n != 2) && (n != 3)) {
            return false;
        }
    } else if (id == "🔼") {
        if (n != 4) {
            return false;
        }
    } else if (n != 2) {
        return false;
    }
    return true;
}

Parser::Parser(std::string file_name) {
    std::set<std::string> seen;

    this->text = load_file(file_name, &seen);
    tokenize();
}

void Parser::add_token(std::string value, int line) {
    if (value == ",") {
        tokens.push_back(Token(token_type::COMMA, ",", line));
    } else if (value == ";") {
        tokens.push_back(Token(token_type::SEMICOLON, ";", line));
    } else if (value == "<") {
        tokens.push_back(Token(token_type::L_ANGLE, "<", line));
    } else if (value == ">") {
        tokens.push_back(Token(token_type::R_ANGLE, ">", line));
    } else if (is_num(value)) {
        tokens.push_back(Token(token_type::NUM, value, line));
    } else {
        tokens.push_back(Token(token_type::ID, value, line));
    }
}

void Parser::tokenize() {
    int n_chars = text.size();
    int line = 0;
    std::string value;
    for (int i = 0; i < n_chars; i++) {
        unsigned char c = text[i];
        
        if (is_whitespace(c) || is_next(c) || (c == '\n')) {
            if (value != "") {
                add_token(value, line);
                value = "";
            }
            if (is_next(c)) {
                add_token(std::string(1, c), line);
            }
        } else if ((c == '/') && (i + 1 < n_chars)) {
            if (text[i + 1] == '/') {
                if (value != "") {
                    add_token(value, line);
                    value = "";
                }
                while ((i < n_chars)) {
                    if (text[i] == '\n') {c = '\n'; break;}
                    i++;
                }
            }
        } else {
            value.push_back(c);
        }

        if (c == '\n') {line++;}
    }

    if (value != "") {
        add_token(value, line);
    }
    tokens.push_back(Token(token_type::EOF_T, "EOF", line));
}

std::string Parser::get_line(int n) {
    int n_chars = text.size();
    int line_i = 0;
    int i;
    for (i = 0; i < n_chars; i++) {
        if (line_i == n) {break;} 
        if (text[i] == '\n') {line_i++;}
    }
    if (line_i != n) {return "";}

    bool start = false;
    std::string line = "";
    while (i < n_chars) {
        if (!is_whitespace(text[i])) {
            start = true;
        }
        if (text[i] == '\n') {break;}

        if (start) {
            line += text[i];
        }

        i++;
    }

    return line;
}

std::unique_ptr<Node> Parser::build_ast() {
    this->func_list.clear();
    for (int i = 0; i < 13; i++) {
        func_list.push_back(built_in_funcs[i]);
    }

    std::unique_ptr<Program> ast = std::make_unique<Program>();
    ast->line = 0;
    this->token_i = 0;
    
    int n_tokens = tokens.size();
    
    while (token_i < n_tokens) {
        Token* token = &tokens[token_i];
        
        if (token->type == token_type::EOF_T) {break;}

        if ((token->type != token_type::ID) || ((token->value != "maze") && (token->value != "squeak"))) {
            throw std::runtime_error("Unexpected token on line " + std::to_string(token->line + 1) + 
            ":\n\t" + get_line(token->line));
        }
        
        ast->body.push_back(parse_funcdec());
    }

    return ast;
}

std::unique_ptr<Node> Parser::parse_funcdec() {
    std::unique_ptr<FuncDec> func = std::make_unique<FuncDec>();

    if (tokens[token_i].value == "squeak") {
        func->id = "squeak";
        func_list.push_back("squeak");
    } else {
        token_i++;
        if (tokens[token_i].type != token_type::ID) {
            throw std::runtime_error("Expected function name on line " + std::to_string(tokens[token_i].line + 1) + 
            ":\n\t" + get_line(tokens[token_i].line));
        } else if (func_exists(tokens[token_i].value)) {
            throw std::runtime_error("Function already defined (line " + std::to_string(tokens[token_i].line + 1) + 
            "):\n\t" + get_line(tokens[token_i].line));
        }
        func->id = tokens[token_i].value;
        func_list.push_back(tokens[token_i].value);
    }

    token_i++;
    if (tokens[token_i].type != token_type::L_ANGLE) {
        throw std::runtime_error("Expected '<' following function declaration on line " + std::to_string(tokens[token_i].line + 1) + 
        ":\n\t" + get_line(tokens[token_i].line));
    }
    int start_line = tokens[token_i].line;
    token_i++;

    int n_tokens = tokens.size();
    while (token_i < n_tokens) {
        if (tokens[token_i].type == token_type::R_ANGLE) {break;}

        if (tokens[token_i].type == token_type::ID) {
            func->body.push_back(parse_callexp());
        } else {
            throw std::runtime_error("Unexpected expression on line " + std::to_string(tokens[token_i].line + 1) + 
            ":\n\t" + get_line(tokens[token_i].line));
        }

        if (tokens[token_i].type == token_type::EOF_T) {
            throw std::runtime_error("Expected '>' after line " + std::to_string(start_line + 1) + 
            ":\n\t" + get_line(start_line));
        }
    }
    token_i++;

    return func;
}

bool Parser::func_exists(std::string id) {
    for (std::string func : func_list) {
        if (func == id) {
            return true;
        }
    }
    return false;
}

std::unique_ptr<Node> Parser::parse_callexp() {
    std::unique_ptr<CallExpression> call = std::make_unique<CallExpression>();

    if (!func_exists(tokens[token_i].value)) {
        throw std::runtime_error("Function \"" + tokens[token_i].value + 
        "\" on line " + std::to_string(tokens[token_i].line + 1) + " undefined:\n\t" + get_line(tokens[token_i].line));
    }
    call->id = tokens[token_i].value;
    int start_line = tokens[token_i].line;
    token_i++;

    if (!(tokens[token_i].type == token_type::L_ANGLE)) {
        throw std::runtime_error("Expected '<' on line " + std::to_string(tokens[token_i].line + 1) + 
        ":\n\t" + get_line(tokens[token_i].line));
    }
    token_i++;

    int n_args = 0;
    while (true) {
        if (tokens[token_i].type == token_type::NUM) {
            if ((call->id == "tile") && (n_args == 0)) {
                throw std::runtime_error("Expected function as argument on line " + std::to_string(tokens[token_i].line + 1) + 
                ":\n\t" + get_line(tokens[token_i].line));
            }

            call->args.push_back(std::make_unique<Literal>(tokens[token_i].value));
            n_args++;
        } else if (tokens[token_i].type == token_type::ID) {
            if (call->id != "tile") {
                throw std::runtime_error("Expected integer as argument on line " + std::to_string(tokens[token_i].line + 1) + 
                ":\n\t" + get_line(tokens[token_i].line));
            } else if (n_args != 0) {
                throw std::runtime_error("Expected integer as argument on line " + std::to_string(tokens[token_i].line + 1) + 
                ":\n\t" + get_line(tokens[token_i].line));
            } else if (!func_exists(tokens[token_i].value)) {
                throw std::runtime_error("Function \"" + tokens[token_i].value + 
                "\" on line " + std::to_string(tokens[token_i].line + 1) + " undefined:\n\t" + get_line(tokens[token_i].line));
            } else if (!has_arg_count(tokens[token_i].value, 2)) {
                throw std::runtime_error("Unexpected value in argument list on line " + std::to_string(tokens[token_i].line + 1) + 
                "; can only tile functions that accept exactly two arguments:\n\t" + get_line(tokens[token_i].line));
            }

            call->args.push_back(std::make_unique<Literal>(tokens[token_i].value));
            n_args++;
        } else {
            throw std::runtime_error("Unexpected value in argument list on line " + std::to_string(tokens[token_i].line + 1) + 
            ":\n\t" + get_line(tokens[token_i].line));
        }
        token_i++;

        if (tokens[token_i].type == token_type::COMMA) {
            token_i++;
        } else if (tokens[token_i].type == token_type::R_ANGLE) {
            if (!has_arg_count(call->id, n_args)) {
                throw std::runtime_error("Unexpected number of arguments for function " + call->id + 
                ":\n\t" + get_line(start_line));
            }

            token_i++;
            break;
        } else {
            throw std::runtime_error("Expected '>' after line " + std::to_string(start_line + 1) + 
            ":\n\t" + get_line(start_line));
        }
    }
    
    if (!(tokens[token_i].type == token_type::SEMICOLON)) {
        throw std::runtime_error("Expected ';' after line " + std::to_string(start_line + 1) + 
        ":\n\t" + get_line(start_line));
    }
    token_i++;

    return call;
}

void indent(int n) {
    for (int i = 0; i < n; i++) {
        std::cout << "    ";
    }
}

void print_ast(Node* ast, int depth) {
    switch (ast->type) {
        case node_type::PROGRAM:
            indent(depth);
            std::cout << "Program {\n";
            for (int i = 0; i < ((Program*) ast)->body.size(); i++) {
                print_ast(((Program*) ast)->body[i].get(), depth + 1);
            }
            indent(depth);
            std::cout << "}\n";
            break;

        case node_type::FUNCDEC:
            indent(depth);
            std::cout << "FuncDec - " << ((FuncDec*) ast)->id <<  " {\n";
            for (int i = 0; i < ((FuncDec*) ast)->body.size(); i++) {
                print_ast(((FuncDec*) ast)->body[i].get(), depth + 1);
            }
            indent(depth);
            std::cout << "}\n";
            break;

        case node_type::CALLEXP:
            indent(depth);
            std::cout << "CallExpression - " << ((CallExpression*) ast)->id <<  " {\n";
            for (int i = 0; i < ((CallExpression*) ast)->args.size(); i++) {
                print_ast(((CallExpression*) ast)->args[i].get(), depth + 1);
            }
            indent(depth);
            std::cout << "}\n";
            break;

        case node_type::LITERAL:
            indent(depth);
            std::cout << "Literal: " << ((Literal*) ast)->value << std::endl;
            break;
    }
}