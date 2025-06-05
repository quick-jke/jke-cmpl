#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>
#include <sstream>
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
            << "get_" 
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
        }else if(type_ == RelationType::ManyToMany || type_ == RelationType::OneToMany){
            oss << "\tstd::vector<" << target_table_ << "> " << field_name_ << "_; //" 
            << (type_ == RelationType::ManyToMany ? "many to many" : "one to many")
            << std::endl;
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
        oss << "\tstd::string name(){return \"" << name_ << "\";}" << std::endl; 

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
            if(relations_.at(i)->type_ != RelationType::ManyToMany){
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
        //private fields
        oss << "\nprivate:" << std::endl;
        for(auto field : fields_){
            oss << field->to_string();
        }
        //relations
        oss << "//Relations:\n";
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

    std::vector<std::shared_ptr<Import>> imports_;
    std::vector<std::shared_ptr<Table>> tables_;
    
    std::string content(){
        std::stringstream oss;
        oss << "#include <string>" << std::endl;
        oss << "#include <vector>" << std::endl;

        //oss << "#include \"column.hpp\"" << std::endl;

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

        return oss.str();
    }
};
}} //namespace quick::jkecmpl
#endif // AST_H