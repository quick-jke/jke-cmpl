#ifndef QUICK_JKE_CMPL_CODEGEN_HPP
#define QUICK_JKE_CMPL_CODEGEN_HPP
#include <fstream>
#include <string>
#include "ast.hpp"
namespace quick {
namespace jkecmpl{
class CodeGenerator{
    std::shared_ptr<AST> ast_;
public:
    CodeGenerator(std::shared_ptr<AST> ast) : ast_(ast){

    }
    void write(const std::string& out_path) {
        std::ofstream file(out_path);

        if (!file.is_open()) {
            throw std::runtime_error("Can not open file " + out_path);
        }

        file << ast_->content(); 
        file.close();
    }
};
}}//namespace quick::jkecmpl
#endif