//Auto-generated file
//do not edit
#ifndef Passport_hpp
#define Passport_hpp
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include "session.hpp"
namespace one_to_one{
class Passport : public quick::ultra::sqljke::SQLTable{
public:
	Passport(){}
	explicit Passport(quick::ultra::ResultSet& rs){
		id_ = rs.get_int("id");
		num_ = rs.get_int("num");
	}
	friend bool operator==(const Passport &a, const Passport &b){
		return
			(a.num_ == b.num_);
	}
	inline static const std::string TABLE_NAME = "Passport";
	std::string table_name() const override {return "Passport";}
	inline static const std::vector<quick::ultra::sqljke::Column> COLUMNS = {
			{"num", "INT", false, false, false, ""}
	};

	std::vector<quick::ultra::sqljke::Column> columns() const override {
		return {
			{"id", "INT", true, true, false, ""},
			{"num", "INT", false, false, false, ""}
		};
	}
	inline static const std::vector<std::string> COLUMN_NAMES = {"num"};
	std::vector<std::string> column_names() const override {
		return {"num"};
	}
	std::vector<std::string> values() const override {
		return {std::to_string(num_), };
	}
	std::vector<quick::ultra::sqljke::Link> links() const override {
		return {
		};
	}
	void set_id(const int id){id_ = id;}
	const int id()const override {return id_;}
	void set_num(const int num){num_ = num;}
	const int num(){return num_;}

	std::vector<std::shared_ptr<quick::ultra::sqljke::SQLTable>> get_dependent_objects() const override {
		std::vector<std::shared_ptr<quick::ultra::sqljke::SQLTable>> result;
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
	static const std::string num_equal(int value){ return "num = " + std::to_string(value);}
	static const std::string num_less_or_equal(int value){ return "num <= " + std::to_string(value);}
	static const std::string num_less_than(int value){ return "num < " + std::to_string(value);}
	static const std::string num_more_or_equal(int value){ return "num >= " + std::to_string(value);}
	static const std::string num_more_than(int value){ return "num > " + std::to_string(value);}
	static const std::string num_not_equal(int value){ return "num != " + std::to_string(value);}
	static const std::string num_between_and(int val1, int val2){ return "num BETWEEN" + std::to_string(val1) + " AND " + std::to_string(val2);}
	static const std::string num_is_not_null(){ return "num IS NOT NULL";}
	static const std::string num_is_null(){ return "num IS NULL";}
	static const std::string num_in(std::vector<int> values){
		std::stringstream oss;
		oss << "(";
		for(size_t i = 0; i < values.size(); ++i){
			oss << values.at(i);
			if(i != values.size() - 1){
				oss << ", ";
			}
		}
		oss << ")";
		return "num in " + oss.str();
	}

private:
	int id_ = 0;
	int num_;
	std::vector<std::shared_ptr<quick::ultra::sqljke::IRelation>> relations_;
};

} // namespace one_to_one
#endif
