#ifndef AST_H
#define AST_H
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <memory>
#include <cctype>
#include <set>
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

enum class RelationType {
    OneToOne,
    OneToMany,
    ManyToOne,
    ManyToMany
};

struct Field {
    Field(const std::string& name, FieldType type);
    Field(const std::string& name, const std::string& custom_type);
    std::string to_string();
    std::string setter();
    std::string getter();
    std::string column();
    std::string value();
    // std::string expr();

    std::string name_;
    FieldType type_;
    std::string custom_type_; 
    bool is_primary_ = false;
    
};

struct Relation {
    Relation(RelationType type, const std::string& target_table, const std::string& field_name);
    std::string to_string();
    std::string column();
    std::string link();

    RelationType type_;
    std::string target_table_;
    std::string field_name_;
};

struct Table {
    std::string content();
    std::string name_;
    std::vector<std::shared_ptr<Field>> fields_;
    std::vector<std::shared_ptr<Relation>> relations_;
};

struct Import {
    std::string table_name_;
};

struct AST {
    std::vector<std::shared_ptr<Table>> topological_sort();
    std::string content();
    std::string struct_column();
    std::string struct_link();
    std::string sql_table_class();

   


    std::string database_name_;
    std::vector<std::shared_ptr<Import>> imports_;
    std::vector<std::shared_ptr<Table>> tables_;
    std::set<std::pair<std::string, FieldType>> exprs_;
};
}} //namespace quick::jkecmpl
#endif 
