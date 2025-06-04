/*
#include "parser.hpp"
#include <iostream>
#include <sstream>

int main() {
    const std::string input_str = R"(
import Tab1;
import Tab2;
import Tab3;
import Tab4;

table Tab5{
    PRIMARY int id;
    string name;
    double rating;
    bool is_b;
    char c;
    ONE_TO_ONE(Tab1) tab1;
    ONE_TO_MANY(Tab2) tabs;
    MANY_TO_ONE(Tab3) tab3;
    MANY_TO_MANY(Tab4) tabs2;
}
)";

    std::istringstream input(input_str);
    


    int token;
    quick::genesis::Lexer lexer;
    while ((token = lexer.get_tok(input)) != quick::genesis::tok_eof) {
        switch (token) {
            case quick::genesis::tok_import:
                std::cout << "IMPORT\n";
                break;
            case quick::genesis::tok_table:
                std::cout << "TABLE\n";
                break;
            case quick::genesis::tok_primary:
                std::cout << "PRIMARY\n";
                break;
            case quick::genesis::tok_type:
                std::cout << "TYPE: " << lexer.id_str() << "\n";
                break;
            case quick::genesis::tok_name:
                std::cout << "NAME: " << lexer.id_str() << "\n";
                break;
            case quick::genesis::tok_one_to_one:
                std::cout << "ONE_TO_ONE\n";
                break;
            case quick::genesis::tok_one_to_many:
                std::cout << "ONE_TO_MANY\n";
                break;
            case quick::genesis::tok_many_to_one:
                std::cout << "MANY_TO_ONE\n";
                break;
            case quick::genesis::tok_many_to_many:
                std::cout << "MANY_TO_MANY\n";
                break;
            case quick::genesis::tok_ref:
                std::cout << "REF: " << lexer.id_str() << "\n";
                break;
            case ';':
                std::cout << "SEMICOLON\n\n";
                break;
            case '{':
                std::cout << "LBRACE\n";
                break;
            case '}':
                std::cout << "RBRACE\n";
                break;
            default:
                std::cout << "CHAR: " << static_cast<char>(token) << "\n";
        }
    }

    return 0;
}
*/


#include "parser.hpp"
#include <iostream>
#include <sstream>

int main() {
    const std::string input_str = R"(
import Tab1;
import Tab2;
import Tab3;
import Tab4;

table Tab5{
    PRIMARY int id;
    string name;
    double rating;
    bool is_b;
    char c;
    ONE_TO_ONE(Tab1) tab1;
    ONE_TO_MANY(Tab2) tabs;
    MANY_TO_ONE(Tab3) tab3;
    MANY_TO_MANY(Tab4) tabs2;
}
)";

    std::istringstream input(input_str);
    quick::genesis::Parser parser(input);
    auto ast = parser.parse();

    std::cout << "Imports:\n";
    for (auto imp : ast->imports_) {
        std::cout << "  " << imp->table_name_ << "\n";
    }


    std::cout << "\nTables:\n";
    for (auto table : ast->tables_) {
        std::cout << table->to_string() << std::endl;
    }

    return 0;
}
