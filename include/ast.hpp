#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>
#include <sstream>
namespace quick {
namespace genesis{
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

    std::string to_string(){
        std::stringstream oss;
        oss << "      " << name_ << " (" 
                      << (is_primary_ ? "PRIMARY " : "")
                      << (type_ == FieldType::Int ? "int" :
                          type_ == FieldType::String ? "string" :
                          type_ == FieldType::Double ? "double" :
                          type_ == FieldType::Bool ? "bool" :
                          type_ == FieldType::Char ? "char" :
                          "custom:" + custom_type_) << ")\n";
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
        oss << "      " << field_name_ << " -> " << target_table_ << " ("
                      << (type_ == RelationType::OneToOne ? "ONE_TO_ONE" :
                          type_ == RelationType::OneToMany ? "ONE_TO_MANY" :
                          type_ == RelationType::ManyToOne ? "MANY_TO_ONE" :
                          "MANY_TO_MANY") << ")\n";

                          return oss.str();
    }
    
};

struct Table {
    std::string name_;
    std::vector<std::shared_ptr<Field>> fields_;
    std::vector<std::shared_ptr<Relation>> relations_;
    std::string to_string(){
        std::stringstream oss;
        oss << "  Table: " << name_ << "\n";
        oss << "    Fields:\n";
        for(auto field : fields_){
            oss << field->to_string();
        }
        oss << "    Relations:\n";
        for(auto rel : relations_){
            oss << rel->to_string();
        }
        return oss.str();
    }
};

struct Import {
    std::string table_name_;
};

struct AST {
    std::vector<std::shared_ptr<Import>> imports_;
    std::vector<std::shared_ptr<Table>> tables_;
};
}} //namespace quick::genesis
#endif // AST_H