#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>
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
};

struct Table {
    std::string name_;
    std::vector<std::shared_ptr<Field>> fields_;
    std::vector<std::shared_ptr<Relation>> relations_;
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