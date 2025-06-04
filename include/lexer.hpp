#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>
#include <vector>

namespace quick {
namespace genesis{
enum TOKEN {
    tok_eof = -1,
    tok_import = -2,
    tok_table = -3,
    tok_primary = -4,
    tok_span = -5,
    tok_type = -6,
    tok_one_to_one = -7,    
    tok_one_to_many = -8,    
    tok_many_to_one = -9,    
    tok_many_to_many = -10, 
    tok_name = -11,   
    tok_ref = -12  
};

class Lexer{
    int last_char_;
    double num_val_;
    int in_ref_;
    std::string id_str_;
    const std::vector<std::string> data_types_;
    int is_data_type(const std::string& str);
public:
    Lexer();
    std::string id_str();
    int get_tok(std::istream& input);
};

}} //namespace quick::genesis

#endif // TOKEN_H