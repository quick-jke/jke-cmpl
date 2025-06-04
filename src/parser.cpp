#include "parser.hpp"
#include <stdexcept>

namespace quick{
namespace genesis{

Parser::Parser(std::istream& input) : input_(input) {
    ast_ = std::make_shared<AST>();
    lexer_ = std::make_shared<Lexer>();
    advance();
}

std::shared_ptr<AST> Parser::parse() {
    while (current_token_ != tok_eof) {
        if (current_token_ == tok_import) {
            parse_imports();
        } else if (current_token_ == tok_table) {
            parse_tables();
        } else {
            throw std::runtime_error("Unexpected token");
        }
    }
    return ast_;
}

void Parser::advance() {
    current_token_ = lexer_->get_tok(input_);
    // std::cerr << "Token: " << current_token_ << " (" << lexer_->id_str() << ")" << std::endl;
}

void Parser::parse_imports() {
    do {
        advance(); 
        std::string tableName = parse_identifier();
        ast_->imports_.push_back(std::make_shared<Import>(Import{tableName}));
        advance(); 
    } while (current_token_ == tok_import);
}

void Parser::parse_tables() {
    do {
        advance(); 
        std::string table_name = parse_identifier();
        advance();

        auto table = std::make_shared<Table>();
        table->name_ = table_name;

        while (current_token_ != '}') {
            if (current_token_ == tok_primary) {
                advance(); 
                auto field = parse_field();
                field->is_primary_ = true;
                table->fields_.push_back(field);
            } else if (
                current_token_ == tok_one_to_one ||
                current_token_ == tok_one_to_many ||
                current_token_ == tok_many_to_one ||
                current_token_ == tok_many_to_many) {
                table->relations_.push_back(parse_relation());
            } else {
                table->fields_.push_back(parse_field());
            }
        }

        advance();
        ast_->tables_.push_back(table);
    } while (current_token_ == quick::genesis::tok_table);
}

std::string Parser::parse_identifier() {
    if (current_token_ != quick::genesis::tok_name) {
        std::cerr << "Expected identifier but got token: " << current_token_ << std::endl;
        // std::cerr << "Identifier string: " << lexer_->id_str() << std::endl;
        // throw std::runtime_error("Expected identifier");
    }
    std::string name = lexer_->id_str();
    advance();
    return name;
}


std::shared_ptr<Field> Parser::parse_field() {
    std::string type_str;
    FieldType type;

    switch (current_token_) {
        case quick::genesis::tok_type:
            type_str = lexer_->id_str();
            advance();
            if (type_str == "int") type = FieldType::Int;
            else if (type_str == "string") type = FieldType::String;
            else if (type_str == "double") type = FieldType::Double;
            else if (type_str == "bool") type = FieldType::Bool;
            else if (type_str == "char") type = FieldType::Char;
            else type = FieldType::Custom;
            break;
        default:
            throw std::runtime_error("Expected type");
    }
    std::string field_name = parse_identifier();
    advance(); 

    if (type == FieldType::Custom) {
        return std::make_shared<Field>(field_name, type_str);
    } else {
        return std::make_shared<Field>(field_name, type);
    }
}

RelationType Parser::get_relation_type(int token) {
    switch (token) {
        case tok_one_to_one:
            return RelationType::OneToOne;
        case tok_one_to_many:
            return RelationType::OneToMany;
        case tok_many_to_one:
            return RelationType::ManyToOne;
        case tok_many_to_many:
            return RelationType::ManyToMany;
        default:
            throw std::runtime_error("Unknown relation type");
    }
}

std::shared_ptr<Relation> Parser::parse_relation() {
    RelationType rel_type = get_relation_type(current_token_);
    advance(); // переход к '('

    advance(); // пропуск '('
    std::string target_table = parse_identifier(); // Tab1
    advance(); // пропуск ')'

    std::string field_name = parse_identifier(); // tab1
    advance(); // пропуск ';'

    return std::make_shared<Relation>(rel_type, target_table, field_name);
}
}}