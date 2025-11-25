#include "ast.hpp"
#include <algorithm>

namespace quick{
namespace jkecmpl{

    //Field
std::string type_to_sql(FieldType type){
    switch (type)
    {
    case FieldType::Int:{
        return "INT";
        break;
    }
    case FieldType::String:{
        return "VARCHAR";
        break;
    }
    case FieldType::Double:{
        return "DOUBLE";
        break;
    }
    case FieldType::Bool:{
        return "BOOL";
        break;
    }
    case FieldType::Char:{
        return "CHAR";
        break;
    }
    default:
    return "custom:";
        break;
    }
}
std::string type_to_string(FieldType type, bool i = 0){
    switch (type)
    {
    case FieldType::Int:{
        return "int ";
        break;
    }
    case FieldType::String:{
        if(i){return "std::string ";}
        else{
            return "std::string& ";
        }
        break;
    }
    case FieldType::Double:{
        return "double ";
        break;
    }
    case FieldType::Bool:{
        return "bool ";
        break;
    }
    case FieldType::Char:{
        return "char ";
        break;
    }
    default:
    return "custom:";
        break;
    }
}

Field::Field(const std::string& name, FieldType type) : name_(name), type_(type) {}

Field::Field(const std::string& name, const std::string& custom_type) : name_(name), type_(FieldType::Custom), custom_type_(custom_type) {}

std::string Field::to_string(){
        std::stringstream oss;
        oss << "\t" 
            << type_to_string(type_, true)
            << name_ 
            << "_" 
            << (is_primary_ ? " = 0;" : ";")
            << std::endl;
        return oss.str();
    }

std::string Field::setter(){
    std::stringstream oss;
    oss << "\t" << "void set_" << name_ << "(const " 
    << type_to_string(type_)
    << name_ << "){";
    oss << name_ << "_ = " << name_ << ";" << "}";
    return oss.str();
}

std::string Field::getter(){
    std::stringstream oss;
    oss << "\tconst " 
        << type_to_string(type_)
        << name_ 
        << (is_primary_ ? "()const override {return " : "(){return ") 
        << name_ << "_;}";
    return oss.str();
}

std::string Field::column(){
    std::stringstream oss;

    oss << "Column{\"" 
        << name_ 
        << "\", " 
        << type_to_sql(type_)
        << ", " 
        << (is_primary_ ? "true, " : "false, ")
        << (is_primary_ ? "true, " : "false, ") 
        << "false, " 
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



    //Relation
Relation::Relation(RelationType type, const std::string& target_table, const std::string& field_name) : type_(type), target_table_(target_table), field_name_(field_name) {}

std::string Relation::to_string(){
    std::stringstream oss;
    
    if(type_ == RelationType::OneToMany || type_ == RelationType::ManyToMany){
        oss << "\t//std::vector<" << target_table_ << "> " << field_name_ << "_; // " << (type_ == RelationType::OneToMany ? "one to many" : "many to many") << std::endl;
    }else{
        oss << "\tstd::shared_ptr<" << target_table_ << "> " << field_name_ << "_; // " << (type_ == RelationType::ManyToOne ? "many to one" : "one to one") << std::endl;
    }

    return oss.str();
}

std::string Relation::getter(){
    std::stringstream oss;
    if(type_ == RelationType::OneToOne){
        oss << "\tstd::shared_ptr<" << target_table_ << "> " << field_name_ << "(){ return " << field_name_ << "_;}"; 
    }
    return oss.str();
}

std::string Relation::setter(){
    std::stringstream oss;
    if(type_ == RelationType::OneToOne){
        oss << "\tvoid set_" << field_name_ << "(std::shared_ptr<" << target_table_ << "> " << field_name_ << "){ " << field_name_ << "_ = " << field_name_ << ";}"; 
    }
    return oss.str();
}

std::string Relation::column(){
    std::stringstream oss;
    if(type_ == RelationType::ManyToMany){
        return oss.str();
    }

    oss << "\t\t\t{\"" << field_name_ << "_id\", INT, false, false, false, \"\"}";

    return oss.str();
}

std::string Relation::link(){
    std::stringstream oss;
    oss << "\t\t\t{\"" << field_name_ << "_id\", \"" << target_table_  << "\", \"id\"" << "}";
    return oss.str();
}


    //Table
std::string to_lower_string(std::string str) {
    for (char &c : str) {
        c = std::tolower(c);
    }
    return str;
}

std::string to_upper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return std::toupper(c); }
    );
    return result;
}


std::string Table::content(){
    std::stringstream oss;
    
    oss << "class " << name_ << " : public SQLTable" << "{" << std::endl;
    oss << "public:" << std::endl;

    //cunstructor
    oss << "\t" << name_ << "(){}" << std::endl;

    //resultset constructor
    if(fields_.size()){
        oss << "\texplicit " << name_ << "(quick::ultra::ResultSet& rs){" << std::endl;
        for(auto field : fields_){
            oss << "\t\t" << field->name_ << "_ = rs.get";
            switch (field->type_)
            {
            case FieldType::Bool:{
                oss << "_bool";
                break;
            }
            case FieldType::Char:{
                oss << "_char";
                break;
            }
            case FieldType::Double:{
                oss << "_double";
                break;
            }
            case FieldType::Int:{
                oss << "_int";
                break;
            }
            case FieldType::String:{
                oss << "_string";
                break;
            }
            
            default:
                break;
            }
            oss << "(\"" << field->name_ << "\");" << std::endl;
        }
        oss << "\t}" << std::endl;
    }
    
    //operator == 
    if(fields_.size()){
        oss << "\tfriend bool operator==(const " << name_ << " &a, const " << name_ << " &b){" << std::endl
            << "\t\treturn" << std::endl;
        for(size_t i = 0; i < fields_.size(); ++i){
            if(!fields_.at(i)->is_primary_){
                oss << "\t\t\t(a." << fields_.at(i)->name_ << "_ == b." << fields_.at(i)->name_ << "_)"; 
                if(i != fields_.size() - 1){
                    oss << " && ";
                }else{
                    oss << ";";
                }
                oss << std::endl;
            }
            

        }
        oss << "\t}" << std::endl;
    }
    
    

    //namefunc
    oss << "\tinline static const Table TABLE_NAME = Table(\"" << name_ << "\");" << std::endl; 
    oss << "\tstd::string table_name() const override {return \"" << name_ << "\";}" << std::endl; 
    //columns 
    oss << "\tinline static const std::vector<std::variant<Column, Aggregate, Scalar>> COLUMNS = {" << std::endl;
    for(size_t i = 0; i < fields_.size(); ++i){
        if(!fields_.at(i)->is_primary_){
            oss << "\t\t" << fields_.at(i)->column();
            if((i != fields_.size() - 1) || relations_.size()){
                oss << ",";
            }
            oss << std::endl;
        }
    }
 
    oss << "\t};" << std::endl << std::endl;
    for(const auto& field : fields_){
        oss << "\tinline static const Column " << to_upper(field->name_) << " = " << field->column() << ";" << std::endl; 
    }

    oss << "\tstd::vector<Column> columns() const override {" << std::endl;
    oss << "\t\treturn {" << std::endl;
    for(size_t i = 0; i < fields_.size(); ++i){
        oss << "\t\t\t" << fields_.at(i)->column();
        if((i != fields_.size() - 1) || relations_.size()){
            oss << ",";
        }
        oss << std::endl;
        
    }

    oss << "\t\t};" << std::endl << "\t}" << std::endl;

    //column_names
    oss << "\tinline static const std::vector<std::string> COLUMN_NAMES = {";
    for(size_t i = 0; i < fields_.size(); ++i){
        if(!fields_.at(i)->is_primary_){
            oss << "\"" << fields_.at(i)->name_ << "\"";
            if((i != fields_.size() - 1) || relations_.size()){
                oss << ",";
            }
        }
    }
    for(size_t i = 0; i < relations_.size(); ++i){
        if(relations_.at(i)->type_ != RelationType::ManyToMany && relations_.at(i)->type_ != RelationType::OneToMany){
            oss << "\"" << relations_.at(i)->field_name_ << "_id\"";
            if(i != relations_.size() - 1){
                oss << ",";
            }
        }
    }
    oss << "};" << std::endl;

    oss << "\tstd::vector<std::string> column_names() const override {" << std::endl
        << "\t\treturn {";
    for(size_t i = 0; i < fields_.size(); ++i){
        if(!fields_.at(i)->is_primary_){
            oss << "\"" << fields_.at(i)->name_ << "\"";
            if((i != fields_.size() - 1) || relations_.size()){
                oss << ",";
            }
        }
    }
    for(size_t i = 0; i < relations_.size(); ++i){
        if(relations_.at(i)->type_ != RelationType::ManyToMany && relations_.at(i)->type_ != RelationType::OneToMany){
            oss << "\"" << relations_.at(i)->field_name_ << "_id\"";
            if(i != relations_.size() - 1){
                oss << ",";
            }
        }
    }

    oss << "};\n\t}" << std::endl;
    //values
    oss << "\tstd::vector<std::string> values() const override {" << std::endl;
    oss << "\t\treturn {";
    for(size_t i = 0; i < fields_.size(); ++i){
        if(!fields_.at(i)->is_primary_){
            oss << fields_.at(i)->value();
            if(i != fields_.size() - 1){
                oss << ", ";
            }
        }
        
    }

    oss << "};\n\t}" << std::endl;

    //links
    oss << "\tstd::vector<Link> links() const override {" << std::endl;
    oss << "\t\treturn {" << std::endl;

    oss << "\t\t};" << std::endl;
    oss << "\t}" << std::endl;
    
    //setters/getters
    if(!fields_.size()){
        oss << "const int id() const override {return -1;}" << std::endl;
    }
    for(auto field : fields_){
        oss << field->setter() << std::endl;
        oss << field->getter() << std::endl;
    }


    oss << exrs();

    //private fields
    oss << "\nprivate:" << std::endl;
    for(auto field : fields_){
        oss << field->to_string();
    }

    oss << "};" << std::endl;
    return oss.str();
}

std::string Table::exrs(){
    std::stringstream oss;
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

    std::vector<std::shared_ptr<Table>> table_has_many_to_many;
    std::vector<std::shared_ptr<Table>> many_to_many_tables;
    for(auto table : sortedTables){ 
        for(auto rel : table->relations_){
            if(rel->type_ == RelationType::ManyToMany){
                table_has_many_to_many.push_back(table);
            }
        }
    }

    for (auto table : table_has_many_to_many) {
        for (auto rel : table->relations_) {
            if (rel->type_ == RelationType::ManyToMany) {
                Table mtmt;
                mtmt.name_ = rel->target_table_ + "_" + table->name_;
                std::string other_name = table->name_ + "_" + rel->target_table_;

                auto it = std::find_if(many_to_many_tables.begin(), many_to_many_tables.end(),
                    [&other_name](const std::shared_ptr<Table>& t) {
                        return t->name_ == other_name;
                    });

                if (it != many_to_many_tables.end()) {
                    (*it)->relations_.push_back(std::make_shared<Relation>(RelationType::ManyToOne, rel->target_table_, rel->target_table_));
                } else {
                    mtmt.relations_.push_back(std::make_shared<Relation>(RelationType::ManyToOne, rel->target_table_, rel->target_table_));
                    many_to_many_tables.push_back(std::make_shared<Table>(mtmt));
                }
            }
        }
    }

    for(auto tb : many_to_many_tables){
        sortedTables.push_back(tb);
    }

    return sortedTables;
}



}} //namespace quick::jkecmpl
