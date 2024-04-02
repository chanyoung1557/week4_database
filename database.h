#pragma once
#include <string>

enum Type { INT, DOUBLE, STRING, ARRAY };

struct Array {
	int size;
	Type type;
	void* items;
};

struct Entry {
	Type type;
	std::string key;
	void* value;
};

struct Database {
	int size;
	Entry* db_array;
};

// 엔트리를 생성한다.
Entry* create(Type type, std::string key, void* value);

// 데이터베이스를 초기화한다.
void init(Database& database);

// 데이터베이스에 엔트리를 추가한다.
void add(Database& database, Entry* entry);

// 데이터베이스에서 키에 해당하는 엔트리를 찾는다.
Entry* get(Database& database, std::string& key);

// 데이터베이스에서 키에 해당하는 엔트리를 제거한다.
void remove(Database& database, std::string& key);

// 데이터베이스를 해제한다.
void destroy(Database& database);

// 데이터베이스의 리스트를 출력한다.
void list(Database& database);

void initArray(Array* array);

void listArray(Array& array);

