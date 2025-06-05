
# jke-cmpl — C++ Class Generator from Data Model

jke-cmpl is a lightweight compiler that transforms data model definitions (tables, relationships, types) into ready-made C++ classes. The tool is ideal for automating the creation of template code when working with databases.

## Peculiarities

- Automatic generation of classes from the model description.
- Support for relationships: ONE_TO_ONE, ONE_TO_MANY, MANY_TO_ONE, MANY_TO_MANY.
- Simple DSL for describing tables and fields.
- Support for basic types: int, string, double, bool, char.
- Generation of clean and readable C++ code.

## Example input file (model.def)

```def
table Book{
    PRIMARY int id;
    string title;
    MANY_TO_MANY(Author) authors;
    ONE_TO_ONE(Publisher) publisher;
    bool is_bn;
    string publication_date;
    string price;
}
```

## Installation
1. clone repo
```
https://github.com/quick-jke/jke-cmpl
```
2. build project
```
cd jke-cmpl
mkdir build && cd build
cmake ..
make
```
3. install
```
sudo make install
```

## Usage

Generate C++ classes from the model file:
```
jkecmpl --def_path=../example/models/model.def --cpp_out=models.hpp
```

## Output file(models.hpp)

```cpp
//just to demostrate 
//taken out of context
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
class Book{
public:
	Book(){}
	std::string name(){return "Book";}
	std::vector<Column> columns(){
		return {
			{"id", "int", true, true, false, "0"},
			{"title", "string", false, false, false, ""},
			{"is_bn", "bool", false, false, false, "false"},
			{"publication_date", "string", false, false, false, ""},
			{"price", "string", false, false, false, ""},
			{"publisher_id", "int", false, false, false, "0"}
		};
	}
	std::vector<std::string> values(){
		return {std::to_string(id_), title_, std::to_string(is_bn_), publication_date_, price_, "publisher_id"};
	};
	void set_id(int id){id_ = id;}
	int get_id(){return id_;}
	void set_title(const std::string& title){title_ = title;}
	std::string get_title(){return title_;}
	void set_is_bn(bool is_bn){is_bn_ = is_bn;}
	bool get_is_bn(){return is_bn_;}
	void set_publication_date(const std::string& publication_date){publication_date_ = publication_date;}
	std::string get_publication_date(){return publication_date_;}
	void set_price(const std::string& price){price_ = price;}
	std::string get_price(){return price_;}

private:
	int id_;
	std::string title_;
	bool is_bn_;
	std::string publication_date_;
	std::string price_;
//Relations:
	std::vector<Author> authors_; //many to many
	Publisher publisher; // one to one
};

```