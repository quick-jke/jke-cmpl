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
        for(auto table : ast_->topological_sort()){
            std::cout << table << std::endl;
            std::ofstream file(out_path + "/" + table->name_ + ".hpp");

            file << "//Auto-generated file" << std::endl << "//do not edit" << std::endl;
            file << "#include <string>" << std::endl;
            file << "#include <vector>" << std::endl;
            file << "#include <sstream>" << std::endl;
            file << "#include <memory>" << std::endl;
            file << "#include \"session.hpp\"" << std::endl;

            for(auto rel : table->relations_){
                file << "#include \"" << rel->target_table_ << ".hpp\"" << std::endl;
            }
            
            file << "namespace " << ast_->database_name_ << "{" << std::endl;
            file << "inline const std::string DATABASE_NAME = \"" << ast_->database_name_ << "\";" << std::endl;

            file << table->content() << std::endl;
            

            file << "} // namespace " << ast_->database_name_ << std::endl;
            file.close();
        }
        
    }
};
}}//namespace quick::jkecmpl
#endif
