#include "lexer.hpp"
#include <cctype>
#include <cstdlib>
#include <vector>

namespace quick {
namespace jkecmpl {

Lexer::Lexer() 
    : last_char_(' '), 
    num_val_(0.0), 
    data_types_({"int", "string", "double", "bool", "char"}){

}
std::string Lexer::id_str(){
    return id_str_;
}
int Lexer::is_data_type(const std::string& str) {
    for (const auto& type : data_types_) {
        if (str == type) {
            return true;
        }
    }
    return false;
}

int Lexer::get_tok(std::istream& input) {
    while (std::isspace(last_char_)) {
        last_char_ = input.get();
    }

    if (std::isalpha(last_char_)) {
        id_str_.clear();
        do {
            id_str_ += static_cast<char>(last_char_);
            last_char_ = input.get();
        } while (std::isalnum(last_char_) || last_char_ == '_');

        if (id_str_ == "import") return tok_import;
        if (id_str_ == "table") return tok_table;
        if (id_str_ == "PRIMARY") return tok_primary;
        if (id_str_ == "ONE_TO_ONE") return tok_one_to_one;
        if (id_str_ == "ONE_TO_MANY") return tok_one_to_many;
        if (id_str_ == "MANY_TO_ONE") return tok_many_to_one;
        if (id_str_ == "MANY_TO_MANY") return tok_many_to_many;
        if (id_str_ == "database") return tok_database;

        if (is_data_type(id_str_)) {
            return tok_type;
        }


        return tok_name;
    }

    if (std::isdigit(last_char_) || last_char_ == '.') {
        std::string num_str;
        do {
            num_str += static_cast<char>(last_char_);
            last_char_ = input.get();
        } while (std::isdigit(last_char_) || last_char_ == '.');

        num_val_ = std::atof(num_str.c_str());
        return tok_type;
    }

    if (last_char_ == '#') {
        do {
            last_char_ = input.get();
        } while (last_char_ != EOF && last_char_ != '\n' && last_char_ != '\r');

        if (last_char_ != EOF) {
            return get_tok(input);
        }
    }

    if (last_char_ == EOF) {
        return tok_eof;
    }

    int this_char = last_char_;
    last_char_ = input.get();

    

    return this_char;
}
}} //namepace quick::jkecmpl