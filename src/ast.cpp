#include "ast.hpp"


namespace quick{
namespace jkecmpl{

    //Field
Field::Field(const std::string& name, FieldType type) : name_(name), type_(type) {}

Field::Field(const std::string& name, const std::string& custom_type) : name_(name), type_(FieldType::Custom), custom_type_(custom_type) {}

std::string Field::to_string(){
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

std::string Field::setter(){
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

std::string Field::getter(){
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

std::string Field::column(){
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
        // << (type_ == FieldType::Int ? "\"0\"" :
        //               type_ == FieldType::String ? "\"\"" :
        //               type_ == FieldType::Double ? "\"0.0\"" :
        //               type_ == FieldType::Bool ? "\"false\"" :
        //               type_ == FieldType::Char ? "\"\"" :
        //               "\"\"")
        << "\"\""
        << "}";

    return oss.str();
}

std::string Field::value(){
    std::stringstream oss;
    oss << (type_ == FieldType::String ? "" : "std::to_string(") 
        << name_
        << (type_ == FieldType::String ? "_" : "_)");
    return oss.str();
}

std::string Field::expr(){
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
    if(type_ != FieldType::Bool){
        oss << "\tstd::string " << name_ << "_in(std::vector<" 
            << (type_ == FieldType::Int ? "int" :
                type_ == FieldType::Double ? "double" :
                type_ == FieldType::String ? "std::string" :
                type_ == FieldType::Char ? "char" : "custom: " + custom_type_)
            << "> values){" << std::endl
            << "\t\tstd::stringstream oss;" << std::endl
            << "\t\toss << \"(\";" << std::endl
            << "\t\tfor(size_t i = 0; i < values.size(); ++i){" << std::endl
            << "\t\t\toss << values.at(i);" << std::endl
            << "\t\t\tif(i != values.size() - 1){" << std::endl
            << "\t\t\t\toss << \", \";" << std::endl
            << "\t\t\t}" << std::endl
            << "\t\t}" << std::endl
            << "\t\toss << \")\";" << std::endl
            << "\t\treturn \"" << name_ << " in \" + oss.str();" << std::endl
            << "\t}" 
            << std::endl;
    }
    

    return oss.str();
}

    //Relation
Relation::Relation(RelationType type, const std::string& target_table, const std::string& field_name) : type_(type), target_table_(target_table), field_name_(field_name) {}

std::string Relation::to_string(){
    std::stringstream oss;
    
    if(type_ == RelationType::OneToMany || type_ == RelationType::ManyToMany){
        oss << "\tstd::vector<" << target_table_ << "> " << field_name_ << "_; // " << (type_ == RelationType::OneToMany ? "one to many" : "many to many") << std::endl;
    }else{
        oss << "\t" << target_table_ << " " << field_name_ << "_; // " << (type_ == RelationType::ManyToOne ? "many to one" : "one to one") << std::endl;
    }

    return oss.str();
}

std::string Relation::column(){
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
        << "\"\""
        << "}";

    return oss.str();
}

    //Table
std::string Table::to_string(){
    std::stringstream oss;
    oss << "class " << name_ << " : public SQLTable" << "{" << std::endl;
    oss << "public:" << std::endl;

    //cunstructor
    oss << "\t" << name_ << "(){}" << std::endl;

    //namefunc
    oss << "\tstd::string table_name() const override {return \"" << name_ << "\";}" << std::endl; 

    //columns 
    oss << "\tstd::vector<Column> columns() const override {" << std::endl;
    oss << "\t\treturn {" << std::endl;
    for(size_t i = 0; i < fields_.size(); ++i){
        oss << fields_.at(i)->column();
        if((i != fields_.size() - 1) || relations_.size()){
            oss << ",";
        }
        oss << std::endl;
    }
    for(size_t i = 0; i < relations_.size(); ++i){
        if(relations_.at(i)->type_ != RelationType::ManyToMany && relations_.at(i)->type_ != RelationType::OneToMany){
            oss << relations_.at(i)->column();
            if(i != relations_.size() - 1){
                oss << ",";
            }
            oss << std::endl;
        }
    }
    oss << "\t\t};" << std::endl << "\t}" << std::endl;

    //values
    oss << "\tstd::vector<std::string> values() const override {" << std::endl;
    oss << "\t\treturn {";
    for(size_t i = 0; i < fields_.size(); ++i){
        if(i){
            oss << ", ";
        }
        oss << fields_.at(i)->value();
        // if((i != fields_.size() - 1) || !relations_.size()){
            
        // }
    }
    for(size_t i = 0; i < relations_.size(); ++i){
        if(relations_.at(i)->type_ != RelationType::ManyToMany && relations_.at(i)->type_ != RelationType::OneToMany ){
            oss << ", \"" << relations_.at(i)->field_name_ << "_id\"";
        }
    }
    oss << "};\n\t}" << std::endl;

    //links
    oss << "\tstd::vector<Link> links() const override {" << std::endl;
    oss << "\t\treturn {};" << std::endl;
    oss << "\t}" << std::endl;
    
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
    oss << "\tstd::vector<std::shared_ptr<IRelation>> relations_;" << std::endl;
    oss << "};" << std::endl;
    return oss.str();
}


    //AST
std::vector<std::shared_ptr<Table>> AST::topological_sort() {
    int n = tables_.size();
    std::unordered_map<std::string, int> tableIndexMap;

    for (int i = 0; i < n; ++i) {
        tableIndexMap[tables_[i]->name_] = i;
    }

    std::vector<std::vector<int>> adj(n);
    std::vector<int> in_degree(n, 0);

    for (int i = 0; i < n; ++i) {
        const auto& table = tables_[i];
        for (const auto& relation : table->relations_) {
            const std::string& targetName = relation->target_table_;

            auto targetIt = tableIndexMap.find(targetName);
            if (targetIt == tableIndexMap.end()) {
            }
            int targetIdx = targetIt->second;

            if (relation->type_ == RelationType::ManyToOne) {
                adj[targetIdx].push_back(i);
                in_degree[i]++;
            } else if (relation->type_ == RelationType::OneToOne) {
                adj[targetIdx].push_back(i);
                in_degree[i]++;
            } else if (relation->type_ == RelationType::OneToMany) {
                const auto& targetTable = tables_[targetIdx];
                bool hasInverse = false;
                for (const auto& invRel : targetTable->relations_) {
                    if (invRel->type_ == RelationType::ManyToOne &&
                        invRel->target_table_ == table->name_) {
                        hasInverse = true;
                        break;
                    }
                }
                if (!hasInverse) {
                    throw std::runtime_error("Error: relation OneToMany from '" + table->name_ +
                                            "' to '" + targetName +
                                            "' requires a ManyToOne in the target table.");
                }
            }
        }
    }

    std::queue<int> queue;
    for (int i = 0; i < n; ++i) {
        if (in_degree[i] == 0) {
            queue.push(i);
        }
    }

    std::vector<int> result;
    while (!queue.empty()) {
        int u = queue.front();
        queue.pop();
        result.push_back(u);

        for (int v : adj[u]) {
            in_degree[v]--;
            if (in_degree[v] == 0) {
                queue.push(v);
            }
        }
    }

    if (result.size() != n) {
        throw std::runtime_error("Error: The graph contains a cycle, topological sorting is not possible.");
    }

    std::vector<std::shared_ptr<Table>> sortedTables;
    for (int idx : result) {
        sortedTables.push_back(tables_[idx]);
    }

    return sortedTables;
}

std::string AST::content(){
    auto tables = topological_sort();
    std::stringstream oss;
    oss << "//Auto-generated file" << std::endl << "//do not edit" << std::endl;
    oss << "#include <string>" << std::endl;
    oss << "#include <vector>" << std::endl;
    oss << "#include <sstream>" << std::endl;
    oss << "#include <memory>" << std::endl;

    //oss << "#include \"column.hpp\"" << std::endl;
    oss << "namespace " << database_name_ << "{" << std::endl;
    //temp Column
    oss << "struct Column {" << std::endl
        << "\tstd::string name;" << std::endl
        << "\tstd::string type;" << std::endl
        << "\tbool is_primary_key = false;" << std::endl
        << "\tbool is_auto_increment = false;" << std::endl
        << "\tbool is_nullable = true;" << std::endl
        << "\tstd::string default_value;" << std::endl
        << "};" << std::endl << std::endl;

    //temp Link
    oss << "struct Link{};" << std::endl << std::endl;
    //temp SQLTable interface
    oss << "class SQLTable{\npublic:\n\tSQLTable() = default;\n\tvirtual std::string table_name() const = 0;\n\tvirtual std::vector<Column> columns() const = 0;\n\tvirtual std::vector<Link> links() const = 0;\n\tvirtual std::vector<std::string> values() const = 0;\n\tvirtual ~SQLTable() = default;\n};" << std::endl << std::endl;

    
    oss << "struct IRelation{" << std::endl
        << "public:" << std::endl
        << "};" << std::endl
        << "struct OneToOneRelation : public IRelation {" << std::endl
        << "\tstd::shared_ptr<SQLTable> first_;" << std::endl
        << "\tstd::shared_ptr<SQLTable> second_;" << std::endl
        << "};" << std::endl
        << "struct OneToManyRelation : public IRelation {" << std::endl
        << "\tstd::shared_ptr<SQLTable> one_;" << std::endl
        << "\tstd::shared_ptr<SQLTable> many_;" << std::endl
        << "};" << std::endl
        << "struct ManyToOneRelation : public IRelation {" << std::endl
        << "\tstd::shared_ptr<SQLTable> many_;" << std::endl
        << "\tstd::shared_ptr<SQLTable> one_;" << std::endl
        << "};" << std::endl
        << "struct ManyToManyRelation : public IRelation {" << std::endl
        << "\tstd::shared_ptr<SQLTable> first_;" << std::endl
        << "\tstd::shared_ptr<SQLTable> second_;" << std::endl
        << "};" << std::endl << std::endl;
    
    for(auto table : tables) {
        oss << table->to_string() << std::endl;
    }

    oss << "static const std::vector<SQLTable> pure_tables = {";
    for(size_t i = 0; i < tables.size(); ++i){
        oss << tables.at(i)->name_ << "()";
        if(i != tables.size() - 1){
            oss << ", ";
        }
    }
    oss << "};" << std::endl;
    oss << "} // namespace " << database_name_ << std::endl;
    return oss.str();
}


}} //namespace quick::jkecmpl