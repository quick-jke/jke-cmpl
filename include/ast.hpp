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



    Field(const std::string& name, FieldType type)
        : name_(name), type_(type) {}

    Field(const std::string& name, const std::string& custom_type)
        : name_(name), type_(FieldType::Custom), custom_type_(custom_type) {}


    std::string type(){
        std::stringstream oss;
        oss << (type_ == FieldType::Int ? "int " :
                          type_ == FieldType::String ? "std::string " :
                          type_ == FieldType::Double ? "double " :
                          type_ == FieldType::Bool ? "bool " :
                          type_ == FieldType::Char ? "char " :
                          "custom:" + custom_type_);
        return oss.str();
    }

    std::string to_string(){
        std::stringstream oss;
        oss << "\t" << type() << name_ << "_;" << std::endl;
        return oss.str();
    }

    std::string setter(){
        std::stringstream oss;
        oss << "\t" << "void set_" << name_ << "(" << 
        (type_ == FieldType::Int ? "int " :
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
        oss << "\t" << type() << "get_" << name_ << "(){return " << name_ << "_;}";
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

    Relation(RelationType type, const std::string& target_table, const std::string& field_name)
        : type_(type), target_table_(target_table), field_name_(field_name) {}

    std::string to_string(){
        std::stringstream oss;
        
        if(type_ == RelationType::OneToOne){
            oss << "\t" << target_table_ << " " << field_name_ << ";" << std::endl;
        }else if(type_ == RelationType::ManyToMany || type_ == RelationType::OneToMany){
            oss << "\tstd::vector<" << target_table_ << "> " << field_name_ << "_;" << std::endl;
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
    
};

struct Table {
    std::string name_;
    std::vector<std::shared_ptr<Field>> fields_;
    std::vector<std::shared_ptr<Relation>> relations_;
    std::string to_string(){
        std::stringstream oss;
        oss << "class " << name_ /*<< " : public SQLTable"*/ << "{" << std::endl;
        oss << "public:" << std::endl;
        oss << "\t" << name_ << "(){}" << std::endl;
        for(auto field : fields_){
            oss << field->setter() << std::endl;
            oss << field->getter() << std::endl;
        }
        oss << "\nprivate:" << std::endl;
        for(auto field : fields_){
            oss << field->to_string();
        }
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
        for (auto table : tables_) {
            oss << table->to_string() << std::endl;
        }

        return oss.str();
    }
};
}} //namespace quick::jkecmpl
#endif // AST_H