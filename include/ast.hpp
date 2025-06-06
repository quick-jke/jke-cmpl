#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <map>
namespace quick {
namespace jkecmpl{
enum class FieldType {
    Int,
    String,
    Double,
    Bool,
    Char,
    Custom
};

struct Field {
    std::string name_;
    FieldType type_;
    std::string custom_type_; 
    bool is_primary_ = false;

    Field(const std::string& name, FieldType type) : name_(name), type_(type) {}

    Field(const std::string& name, const std::string& custom_type) : name_(name), type_(FieldType::Custom), custom_type_(custom_type) {}

    std::string to_string(){
        std::stringstream oss;
        oss << "\t" 
            << (type_ == FieldType::Int ? "int " :
                          type_ == FieldType::String ? "std::string " :
                          type_ == FieldType::Double ? "double " :
                          type_ == FieldType::Bool ? "bool " :
                          type_ == FieldType::Char ? "char " :
                          "custom:" + custom_type_)
            << name_ 
            << "_;" 
            << std::endl;
        return oss.str();
    }

    std::string setter(){
        std::stringstream oss;
        oss << "\t" << "void set_" << name_ << "(" 
        << (type_ == FieldType::Int ? "int " :
                          type_ == FieldType::String ? "const std::string& " :
                          type_ == FieldType::Double ? "double " :
                          type_ == FieldType::Bool ? "bool " :
                          type_ == FieldType::Char ? "char " :
                          "custom:" + custom_type_)
        << name_ << "){";
        oss << name_ << "_ = " << name_ << ";" << "}";
        return oss.str();
    }

    std::string getter(){
        std::stringstream oss;
        oss << "\t" 
            << (type_ == FieldType::Int ? "int " :
                          type_ == FieldType::String ? "std::string " :
                          type_ == FieldType::Double ? "double " :
                          type_ == FieldType::Bool ? "bool " :
                          type_ == FieldType::Char ? "char " :
                          "custom:" + custom_type_)
            << name_ 
            << "(){return " 
            << name_ << "_;}";
        return oss.str();
    }

    std::string column(){
        std::stringstream oss;

        oss << "\t\t\t{\"" 
            << name_ 
            << "\", \"" 
            << (type_ == FieldType::Int ? "int" :
                          type_ == FieldType::String ? "string" :
                          type_ == FieldType::Double ? "double" :
                          type_ == FieldType::Bool ? "bool" :
                          type_ == FieldType::Char ? "char" :
                          "custom:" + custom_type_)
            << "\", " 
            << (is_primary_ ? "true, " : "false, ")
            << (is_primary_ ? "true, " : "false, ") 
            << "false, " 
            << (type_ == FieldType::Int ? "\"0\"" :
                          type_ == FieldType::String ? "\"\"" :
                          type_ == FieldType::Double ? "\"0.0\"" :
                          type_ == FieldType::Bool ? "\"false\"" :
                          type_ == FieldType::Char ? "\"\"" :
                          "\"\"")
            << "}";

        return oss.str();
    }

    std::string value(){
        std::stringstream oss;
        oss 
        << (type_ == FieldType::String ? "" : "std::to_string(") 
        << name_
        << (type_ == FieldType::String ? "_" : "_)");
        return oss.str();
    }

    std::string expr(){
        //int, float, decimal, bigint
        // > < >= <= ==
        // between and
        // in 
        // ariphmetic
        
        //strings
        // == !=
        // like
        // in

        //date datetime timestamp
        // == > < <= >=
        // between and
        // year month day 

        //boolean
        // true false

        // null
        std::stringstream oss;

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
            {"_equal", " = \" + "},             // str = 'str'
            {"_not_equal", " != \" + "},         // str !='str'
            {"_like", " LIKE \" + "},           // str LIKE 'str'
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


        if(type_ == FieldType::Int || type_ == FieldType::Double){
            for(auto pair : int_exprs){
                oss << "\tstd::string " << name_ << pair.first << "(" 
                << (type_ == FieldType::Int ? "int " :
                    type_ == FieldType::Double ? "double " :
                    type_ == FieldType::Bool ? "bool " :
                    type_ == FieldType::Char ? "char " :
                    "custom: " + custom_type_)
                << "value){ return \"" << name_ << pair.second << "\" + " << "std::to_string(value);}" << std::endl;
            }
            oss << "\tstd::string " << name_ << "_between_and(int val1, int val2){ return \"" << name_ << " BETWEEN\" + " << "std::to_string(val1)" << " + \" AND \" + " << "std::to_string(val2);}" << std::endl;
            
            oss << "\tstd::string " << name_ << "_in(std::vector<int> values){" << std::endl
            << "\t\tstd::stringstream oss;" << std::endl
            << "\t\toss << \"(\";" << std::endl
            << "\t\tfor(size_t i = 0; i < values.size(); ++i){" << std::endl
            << "\t\t\toss << values.at(i);" << std::endl
            << "\t\t\tif(i != values.size() - 1){" << std::endl
            << "\t\t\t\toss << \", \";" << std::endl
            << "\t\t\t}" << std::endl
            << "\t\t}" << std::endl
            << "\t\toss << \")\";" << std::endl
            << "\t\treturn \"rating in \" + oss.str();" << std::endl
            << "\t}" 
            << std::endl; 
        }else if(type_ == FieldType::String){
            for(auto pair : str_exprs){
                oss << "\tstd::string " << name_ << pair.first 
                << "(const std::string& str_val){"
                << "return \"" << name_ << pair.second << " str_val;}" 
                << std::endl;
            }
        }else if(type_ == FieldType::Bool){
            for(auto pair : bool_exprs){
                oss << "\tstd::string " << name_ << pair.first
                << "(){ return \"" << name_ << pair.second << "\";}"
                << std::endl;
            }
        }

        for(auto pair : null_exprs){
            oss << "\tstd::string " << name_ << pair.first
                << "(){ return \"" << name_ << pair.second << "\";}"
                << std::endl;
        }

        

        return oss.str();
    }

};

enum class RelationType {
    OneToOne,
    OneToMany,
    ManyToOne,
    ManyToMany
};

struct Relation {
    RelationType type_;
    std::string target_table_;
    std::string field_name_;

    Relation(RelationType type, const std::string& target_table, const std::string& field_name) : type_(type), target_table_(target_table), field_name_(field_name) {}

    std::string to_string(){
        std::stringstream oss;
        
        if(type_ == RelationType::OneToOne){
            oss << "\t" << target_table_ << " " << field_name_ << "; // one to one" << std::endl;
        }else if(type_ == RelationType::ManyToMany){
            oss << "\tstd::vector<" << target_table_ << "> " << field_name_ << "_; //" 
            << (type_ == RelationType::ManyToMany ? "many to many" : "one to many")
            << std::endl;
        }else if(type_ == RelationType::ManyToOne){
            oss << "\t" << target_table_ << " " << field_name_ << "_id;" << std::endl;
        }
        else{
            oss << "\t//" << field_name_ << " -> " << target_table_ << " ("
                      << (type_ == RelationType::OneToOne ? "ONE_TO_ONE" :
                          type_ == RelationType::OneToMany ? "ONE_TO_MANY" :
                          type_ == RelationType::ManyToOne ? "MANY_TO_ONE" :
                          "MANY_TO_MANY") << ")\n";
        }

        return oss.str();
    }
    
    std::string column(){
        std::stringstream oss;
        if(type_ == RelationType::ManyToMany){
            return oss.str();
        }

        oss << "\t\t\t{\"" 
            << field_name_ << "_id" 
            << "\", \"int\", "
            << "false, "
            << "false, "
            << "false, "
            << "\"0\""
            << "}";

        return oss.str();
    }

};

struct Table {
    std::string name_;
    std::vector<std::shared_ptr<Field>> fields_;
    std::vector<std::shared_ptr<Relation>> relations_;

    std::string to_string(){
        std::stringstream oss;
        oss << "class " << name_ /*<< " : public SQLTable"*/ << "{" << std::endl;
        oss << "public:" << std::endl;

        //cunstructor
        oss << "\t" << name_ << "(){}" << std::endl;

        //namefunc
        oss << "\tstd::string table_name(){return \"" << name_ << "\";}" << std::endl; 

        //columns 
        oss << "\tstd::vector<Column> columns(){" << std::endl;
        oss << "\t\treturn {" << std::endl;
        for(size_t i = 0; i < fields_.size(); ++i){
            oss << fields_.at(i)->column();
            if((i != fields_.size() - 1) || relations_.size()){
                oss << ",";
            }
            oss << std::endl;
        }
        for(size_t i = 0; i < relations_.size(); ++i){
            if(relations_.at(i)->type_ != RelationType::ManyToMany){
                oss << relations_.at(i)->column();
                if(i != relations_.size() - 1){
                    oss << ",";
                }
                oss << std::endl;
            }
        }
        oss << "\t\t};" << std::endl << "\t}" << std::endl;

        //values
        oss << "\tstd::vector<std::string> values(){" << std::endl;
        oss << "\t\treturn {";
        for(size_t i = 0; i < fields_.size(); ++i){
            oss << fields_.at(i)->value();
            if((i != fields_.size() - 1) || relations_.size()){
                oss << ", ";
            }

        }
        for(size_t i = 0; i < relations_.size(); ++i){
            if(relations_.at(i)->type_ != RelationType::ManyToMany ){
                oss << "\"" << relations_.at(i)->field_name_ << "_id\"";
                if(i != relations_.size() - 1){
                    oss << ", ";
                }
                // oss << std::endl;
            }
        }
        oss << "};\n\t};" << std::endl;
        
        //setters/getters
        for(auto field : fields_){
            oss << field->setter() << std::endl;
            oss << field->getter() << std::endl;
        }

        //expressions
        for(auto field : fields_){
            oss << field->expr() << std::endl;
        }



        //private fields
        oss << "\nprivate:" << std::endl;
        for(auto field : fields_){
            oss << field->to_string();
        }
        //relations
        relations_.size() && oss << "//Relations:\n";
        
        for(auto rel : relations_){
            oss << rel->to_string();
        }
        oss << "};" << std::endl;
        return oss.str();
    }
};

struct Import {
    std::string table_name_;
};

struct AST {

    std::string database_name_;
    std::vector<std::shared_ptr<Import>> imports_;
    std::vector<std::shared_ptr<Table>> tables_;
    
    std::string content(){
        std::stringstream oss;
        oss << "#include <string>" << std::endl;
        oss << "#include <vector>" << std::endl;
        oss << "#include <sstream>" << std::endl;

        //oss << "#include \"column.hpp\"" << std::endl;
        oss << "namespace " << database_name_ << "{" << std::endl;
        oss << "struct Column {" << std::endl
            << "\tstd::string name;" << std::endl
            << "\tstd::string type;" << std::endl
            << "\tbool is_primary_key = false;" << std::endl
            << "\tbool is_auto_increment = false;" << std::endl
            << "\tbool is_nullable = true;" << std::endl
            << "\tstd::string default_value;" << std::endl
            << "};" << std::endl;

        for (auto table : tables_) {
            oss << table->to_string() << std::endl;
        }
        oss << "} // namespace " << database_name_ << std::endl;
        return oss.str();
    }
};
}} //namespace quick::jkecmpl
#endif // AST_H