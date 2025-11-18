
//Auto-generated file
//do not edit
#ifndef one_to_one_hpp
#define one_to_one_hpp
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include "session.hpp"
#include "Passport.hpp"
#include "User.hpp"
namespace one_to_one{
inline const std::string DATABASE_NAME = "one_to_one";
static const std::vector<std::shared_ptr<quick::ultra::sqljke::SQLTable>> tables = {std::make_shared<Passport>(), std::make_shared<User>()};
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
}
#endif
