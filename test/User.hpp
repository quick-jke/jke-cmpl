//Auto-generated file
//do not edit
#ifndef User_hpp
#define User_hpp
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include "session.hpp"
#include "Passport.hpp"
namespace one_to_one{
class User : public quick::ultra::sqljke::SQLTable{
public:
	User(){}
	explicit User(quick::ultra::ResultSet& rs){
		id_ = rs.get_int("id");
		name_ = rs.get_string("name");
	}
	friend bool operator==(const User &a, const User &b){
		return
			(a.name_ == b.name_);
	}
	inline static const std::string TABLE_NAME = "User";
	std::string table_name() const override {return "User";}
	inline static const std::vector<quick::ultra::sqljke::Column> COLUMNS = {
			{"name", "VARCHAR(255)", false, false, false, ""},
			{"passport_id", "int", false, false, false, ""}
	};

	std::vector<quick::ultra::sqljke::Column> columns() const override {
		return {
			{"id", "INT", true, true, false, ""},
			{"name", "VARCHAR(255)", false, false, false, ""},
			{"passport_id", "int", false, false, false, ""}
		};
	}
	inline static const std::vector<std::string> COLUMN_NAMES = {"name","passport_id"};
	std::vector<std::string> column_names() const override {
		return {"name","passport_id"};
	}
	std::vector<std::string> values() const override {
		return {name_, std::to_string(passport_->id())};
	}
	std::vector<quick::ultra::sqljke::Link> links() const override {
		return {
			{"passport_id", "Passport", "id"}
		};
	}
	void set_id(const int id){id_ = id;}
	const int id()const override {return id_;}
	void set_name(const std::string& name){name_ = name;}
	const std::string& name(){return name_;}
	std::shared_ptr<Passport> passport(){ return passport_;}
	void set_passport(std::shared_ptr<Passport> passport){ passport_ = passport;}

	std::vector<std::shared_ptr<quick::ultra::sqljke::SQLTable>> get_dependent_objects() const override {
		std::vector<std::shared_ptr<quick::ultra::sqljke::SQLTable>> result;
		if (passport_ && passport_->id() == 0) {
			result.push_back(passport_);
		}
		return result;
	}
	static const std::string id_equal(int value){ return "id = " + std::to_string(value);}
	static const std::string id_less_or_equal(int value){ return "id <= " + std::to_string(value);}
	static const std::string id_less_than(int value){ return "id < " + std::to_string(value);}
	static const std::string id_more_or_equal(int value){ return "id >= " + std::to_string(value);}
	static const std::string id_more_than(int value){ return "id > " + std::to_string(value);}
	static const std::string id_not_equal(int value){ return "id != " + std::to_string(value);}
	static const std::string id_between_and(int val1, int val2){ return "id BETWEEN" + std::to_string(val1) + " AND " + std::to_string(val2);}
	static const std::string id_is_not_null(){ return "id IS NOT NULL";}
	static const std::string id_is_null(){ return "id IS NULL";}
	static const std::string id_in(std::vector<int> values){
		std::stringstream oss;
		oss << "(";
		for(size_t i = 0; i < values.size(); ++i){
			oss << values.at(i);
			if(i != values.size() - 1){
				oss << ", ";
			}
		}
		oss << ")";
		return "id in " + oss.str();
	}
	static const std::string name_equal(const std::string& str_val){return "name = '" +  str_val + "'";}
	static const std::string name_like(const std::string& str_val){return "name LIKE '" +  str_val + "'";}
	static const std::string name_not_equal(const std::string& str_val){return "name != '" +  str_val + "'";}
	static const std::string name_is_not_null(){ return "name IS NOT NULL";}
	static const std::string name_is_null(){ return "name IS NULL";}
	static const std::string name_in(std::vector<std::string> values){
		std::stringstream oss;
		oss << "(";
		for(size_t i = 0; i < values.size(); ++i){
			oss << values.at(i);
			if(i != values.size() - 1){
				oss << ", ";
			}
		}
		oss << ")";
		return "name in " + oss.str();
	}

private:
	int id_ = 0;
	std::string name_;
//Relations:
	std::shared_ptr<Passport> passport_; // one to one
	std::vector<std::shared_ptr<quick::ultra::sqljke::IRelation>> relations_;
};

} // namespace one_to_one
#endif
