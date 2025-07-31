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

            for(auto rel : table->relations_){
                file << "#include \"" << rel->target_table_ << ".hpp\"" << std::endl;
            }
            
            file << "namespace " << ast_->database_name_ << "{" << std::endl;
            

            file << table->content() << std::endl;
            

            file << "} // namespace " << ast_->database_name_ << std::endl;
            file << "#endif" << std::endl;
            file.close();
        }

        std::ofstream dbname(out_path + "/" + ast_->database_name_ + ".hpp");
        dbname << "" << std::endl;

        

        std::set<std::pair<std::string, FieldType>> exprs_;
        for(auto table : ast_->topological_sort()){
            for(auto field : table->fields_){
                exprs_.insert({field->name_, field->type_});
            }
        }
        std::map<std::string, std::string> int_exprs = {
            {"_more_than", " > "},        // a > 5                           
            {"_less_than", " < "},        // a < 5                    
            {"_more_or_equal", " >= "},    // a >=5                           
            {"_less_or_equal", " <= "},    // a <=5                           
            {"_equal", " = "},             // a = 5                      
            {"_not_equal", " != "}          // a !=5     
            // a between 5 and 10
            // in (1, 2, 3)                    
        };
        std::map<std::string, std::string> str_exprs = {
            {"_equal", " = \'\" + "},             // str = 'str'
            {"_not_equal", " != \'\" + "},         // str !='str'
            {"_like", " LIKE \'\" + "},           // str LIKE 'str'
            // str IN ('val1', 'val2')
        };
        std::map<std::string, std::string> bool_exprs = {
            {"_true", " IS TRUE"},
            {"_false", " IS FALSE"}
        };
        std::map<std::string, std::string> null_exprs = {
            {"_is_null", " IS NULL"},
            {"_is_not_null", " IS NOT NULL"}
        };
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
        dbname << "static const std::vector<std::shared_ptr<quick::ultra::sqljke::SQLTable>> tables = {";
            for(size_t i = 0; i < ast_->topological_sort().size(); ++i) {
                dbname << "std::make_shared<" << ast_->topological_sort().at(i)->name_ << ">()";
                if(i != ast_->topological_sort().size() - 1) dbname << ", ";
            }
        dbname << "};" << std::endl;
        for(auto [field_name, field_type] : exprs_){
            if(field_type == FieldType::Int || field_type == FieldType::Double){
                for(auto [method_name, method_value] : int_exprs){
                    dbname << "static const std::string " << field_name << method_name << "(" 
                    << (field_type == FieldType::Int ? "int " :
                        field_type == FieldType::Double ? "double " :
                        field_type == FieldType::Bool ? "bool " :
                        field_type == FieldType::Char ? "char " :
                        "custom: ")
                    << "value){ return \"" << field_name << method_value << "\" + " << "std::to_string(value);}" << std::endl;
                }
                dbname << "static const std::string " << field_name << "_between_and(int val1, int val2){ return \"" << field_name << " BETWEEN\" + " << "std::to_string(val1)" << " + \" AND \" + " << "std::to_string(val2);}" << std::endl;
                    
            }else if(field_type == FieldType::String){
                for(auto [method_name, method_value] : str_exprs){
                    dbname << "static const std::string " << field_name << method_name
                    << "(const std::string& str_val){"
                    << "return \"" << field_name << method_value << " str_val + \"\'\";}" 
                    << std::endl;
                }
            }else if(field_type == FieldType::Bool){
                for(auto [method_name, method_value] : bool_exprs){
                    dbname << "static const std::string " << field_name << method_name
                    << "(){ return \"" << field_name << method_value << "\";}"
                    << std::endl;
                }
            }

            for(auto [method_name, method_value] : null_exprs){
                dbname << "static const std::string " << field_name << method_name
                    << "(){ return \"" << field_name << method_value << "\";}"
                    << std::endl;
            }
            if(field_type != FieldType::Bool){
                dbname << "static const std::string " << field_name << "_in(std::vector<" 
                    << (field_type == FieldType::Int ? "int" :
                        field_type == FieldType::Double ? "double" :
                        field_type == FieldType::String ? "std::string" :
                        field_type == FieldType::Char ? "char" : "custom: ")
                    << "> values){" << std::endl
                    << "\tstd::stringstream oss;" << std::endl
                    << "\toss << \"(\";" << std::endl
                    << "\tfor(size_t i = 0; i < values.size(); ++i){" << std::endl
                    << "\t\toss << values.at(i);" << std::endl
                    << "\t\tif(i != values.size() - 1){" << std::endl
                    << "\t\t\toss << \", \";" << std::endl
                    << "\t\t}" << std::endl
                    << "\t}" << std::endl
                    << "\toss << \")\";" << std::endl
                    << "\treturn \"" << field_name << " in \" + oss.str();" << std::endl
                    << "}" 
                    << std::endl;
            }
        }                                               
        dbname << "}" << std::endl;
        dbname << "#endif" << std::endl;
        dbname.close();
        
    }
};
}}//namespace quick::jkecmpl
#endif
