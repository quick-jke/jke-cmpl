#ifndef PARSER_H
#define PARSER_H
#include "ast.hpp"
#include "lexer.hpp"  
namespace quick {
namespace jkecmpl{
class Parser {
public:
    Parser(std::istream& input);
    std::shared_ptr<AST> parse();
    std::string analyze();

private:
    std::istream& input_;
    int current_token_;

    void advance();
    void parse_imports();
    void parse_tables();
    void parse_database();
    std::shared_ptr<Table> parse_table();
    std::shared_ptr<Field> parse_field();
    std::shared_ptr<Relation> parse_relation();
    std::string parse_identifier();
    RelationType get_relation_type(int token);
    std::shared_ptr<AST> ast_;
    std::shared_ptr<Lexer> lexer_;
};
}} //namespace quick::jkecmpl
#endif // PARSER_H