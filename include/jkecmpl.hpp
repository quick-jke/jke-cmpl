#ifndef QUICK_JKE_CMPL_HPP
#define QUICK_JKE_CMPL_HPP
#include <string>
#include <fstream>
struct config{
    std::string def_path;
    std::string out_path;
};

config get_cmd_path(int argc, char* argv[]){
    config cfg;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg.find("--def_path=") == 0) {
            cfg.def_path = arg.substr(11); 
        } else if (arg.find("--cpp_out=") == 0) {
            cfg.out_path = arg.substr(10); 
        }
    }

    return cfg;
}

std::string content(const std::string& def_path){
    std::ifstream file(def_path);
    
    if (!file.is_open()) {
        throw std::runtime_error("Can not open file: " + def_path);
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();  

    return buffer.str();
}

#endif