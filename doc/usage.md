# Usage
## launch command
```
jkecmpl --def_path=<path_to_file.def> --cpp_out=<output_file.hpp>
```


## Params

| Parameter    | Description                |
| :----------- | :------------------------- |
| `--def_path` | **Required**. Path to the file with the model (e.g. model.def) |
| `--cpp_out`  | **Required**. Path to output C++ header file (e.g. models.hpp) |

## Example of use
1. Create a model.def file:
```
table User {
    PRIMARY int id;
    string name;
};

table Group {
    PRIMARY int id;
    string title;
    MANY_TO_MANY(User) users;
};
```

2. Run the compiler:
```
jkecmpl --def_path=model.def --cpp_out=models.hpp
```

3. Result
```cpp
#include <string>
#include <vector>
struct Column {
	std::string name;
	std::string type;
	bool is_primary_key = false;
	bool is_auto_increment = false;
	bool is_nullable = true;
	std::string default_value;
};
class User{
public:
	User(){}
	std::string name(){return "User";}
	std::vector<Column> columns(){
		return {
			{"id", "int", true, true, false, "0"},
			{"name", "string", false, false, false, ""}
		};
	}
	std::vector<std::string> values(){
		return {std::to_string(id_), name_};
	};
	void set_id(int id){id_ = id;}
	int get_id(){return id_;}
	void set_name(const std::string& name){name_ = name;}
	std::string get_name(){return name_;}

private:
	int id_;
	std::string name_;
//Relations:
};

class Group{
public:
	Group(){}
	std::string name(){return "Group";}
	std::vector<Column> columns(){
		return {
			{"id", "int", true, true, false, "0"},
			{"title", "string", false, false, false, ""},
		};
	}
	std::vector<std::string> values(){
		return {std::to_string(id_), title_, };
	};
	void set_id(int id){id_ = id;}
	int get_id(){return id_;}
	void set_title(const std::string& title){title_ = title;}
	std::string get_title(){return title_;}

private:
	int id_;
	std::string title_;
//Relations:
	std::vector<User> users_; //many to many
};


```




