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
            file << "#ifndef " << table->name_ << "_hpp" << std::endl;
            file << "#define " << table->name_ << "_hpp" << std::endl; 
            file << "#include <string>" << std::endl;
            file << "#include <vector>" << std::endl;
            file << "#include <sstream>" << std::endl;
            file << "#include <memory>" << std::endl;
            file << "#include \"session.hpp\"" << std::endl;
            file << "using namespace quick::ultra::sqljke;" << std::endl;

            // for(auto rel : table->relations_){
            //     file << "#include \"" << rel->target_table_ << ".hpp\"" << std::endl;
            // }
            
            file << "namespace " << ast_->database_name_ << "{" << std::endl;
            

            file << table->content() << std::endl;
            

            file << "} // namespace " << ast_->database_name_ << std::endl;
            file << "#endif" << std::endl;
            file.close();
        }
        
        std::ofstream dbname(out_path + "/" + ast_->database_name_ + ".hpp");

        dbname << "//Auto-generated file" << std::endl << "//do not edit" << std::endl;
        dbname << "#ifndef " << ast_->database_name_ << "_hpp" << std::endl;
        dbname << "#define " << ast_->database_name_ << "_hpp" << std::endl;
        dbname << "#include <string>" << std::endl;
        dbname << "#include <vector>" << std::endl;
        dbname << "#include <sstream>" << std::endl;
        dbname << "#include <memory>" << std::endl;
        dbname << "#include \"session.hpp\"" << std::endl;
        for(size_t i = 0; i < ast_->topological_sort().size(); ++i) {
            dbname << "#include \"" << ast_->topological_sort().at(i)->name_ << ".hpp\"" << std::endl;
        }
        dbname << "namespace " << ast_->database_name_ << "{" << std::endl;
        dbname << "inline const std::string DATABASE_NAME = \"" << ast_->database_name_ << "\";" << std::endl;
        dbname << "static const std::vector<std::shared_ptr<quick::ultra::sqljke::SQLTable>> TABLES = {";
            for(size_t i = 0; i < ast_->topological_sort().size(); ++i) {
                dbname << "std::make_shared<" << ast_->topological_sort().at(i)->name_ << ">()";
                if(i != ast_->topological_sort().size() - 1) dbname << ", ";
            }
        dbname << "};" << std::endl;
                                             
        dbname << "}" << std::endl;
        dbname << "#endif" << std::endl;
        dbname.close();
        
    }
};
}}//namespace quick::jkecmpl
#endif
