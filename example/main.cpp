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

table lol{
    PRIMARY int id;
    string email;
}
)";

    std::istringstream input(input_str);
    quick::genesis::Parser parser(input);
    auto ast = parser.parse();
    std::cout << ast->to_string() << std::endl;
    // std::cout << parser.analyze() << std::endl;
    

    return 0;
}
