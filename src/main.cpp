#include "parser.hpp"
#include <iostream>
#include <sstream>
#include "jkecmpl.hpp"


int main(int argc, char* argv[]) {
    try{
        auto config = get_cmd_path(argc, argv);
        auto input_str = content(config.def_path);
        std::istringstream input(input_str);
        quick::genesis::Parser parser(input);
        auto ast = parser.parse();
        try{
            write(config.out_path, ast->to_string());
            std::cout << "compiling succes" << std::endl;
        }catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
            return 1;
        }

    }catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
