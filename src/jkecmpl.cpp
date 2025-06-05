#include "parser.hpp"
#include <iostream>
#include <sstream>
#include "jkecmpl.hpp"
#include "codegen.hpp"

int main(int argc, char* argv[]) {
    try{
        auto config = get_cmd_path(argc, argv);
        auto input_str = content(config.def_path);
        std::istringstream input(input_str);
        quick::jkecmpl::Parser parser(input);
        try{
            quick::jkecmpl::CodeGenerator gen(parser.parse());
            gen.write(config.out_path);
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
