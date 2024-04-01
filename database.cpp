#include <iostream>
#include <cstring>
#include <string>
#include <cstdlib>
#include "database.h"


// 엔트리를 생성한다.
Entry* create(Type type, std::string key, void* value) {
    // 새로운 Entry 구조체 생성
    Entry* entry = new Entry;

    // 입력된 값으로 Entry 설정
    entry->type = type;
    entry->key = key;
    entry->value = value;

    // 생성된 Entry 반환
    return entry;
}

// 데이터베이스를 초기화한다.
void init(Database& database) {
    // 초기 크기를 0으로 설정하고, 엔트리 배열을 nullptr로 초기화합니다.
    database.size = 0;
    database.entries = nullptr;
}

// 데이터베이스에 엔트리를 추가한다.
void add(Database& database, Entry* entry) {
    // 새로운 엔트리를 추가하기 위해 엔트리 배열의 크기를 증가시킵니다.
    int newSize = database.size + 1;
    // 새로운 엔트리 배열을 할당하고 기존 엔트리를 복사합니다.
    Entry** newEntries = new Entry * [newSize];
    for (int i = 0; i < database.size; ++i) {
        newEntries[i] = database.entries[i];
    }
    // 새로운 엔트리를 배열에 추가합니다.
    newEntries[newSize - 1] = entry;
    // 이전 엔트리 배열을 삭제하고 새로운 배열로 엔트리를 업데이트합니다.
    delete[] database.entries;
    database.entries = newEntries;
    // 데이터베이스의 크기를 업데이트합니다.
    database.size = newSize;
}

// 데이터베이스에서 키에 해당하는 엔트리를 찾는다.
Entry* get(Database& database, std::string& key) {
    // 데이터베이스 크기만큼 반복하며 키가 일치하는 엔트리를 찾는다.
    for (int i = 0; i < database.size; ++i) {
        if (database.entries[i]->key == key) {
            // 키가 일치하는 엔트리를 반환한다.
            return database.entries[i];
        }
    }
    // 일치하는 엔트리를 찾지 못한 경우 nullptr를 반환한다.
    return nullptr;
}

// 데이터베이스에서 키에 해당하는 엔트리를 제거한다.
void remove(Database& database, std::string& key) {
    // 새로운 엔트리 배열을 생성하고 초기화
    Entry** newEntries = new Entry * [database.size];
    int newSize = 0;

    // 기존 엔트리에서 키가 일치하지 않는 엔트리를 새로운 배열에 복사
    for (int i = 0; i < database.size; ++i) {
        if (database.entries[i]->key != key) {
            newEntries[newSize++] = database.entries[i];
        }
    }

    // 이전 엔트리 배열 메모리 해제 및 새 배열로 교체
    delete[] database.entries;
    database.entries = newEntries;
    database.size = newSize;
}

// 데이터베이스를 해제한다.
void destroy(Database& database) {
    // 데이터베이스에 있는 모든 엔트리 메모리를 해제한다.
    for (int i = 0; i < database.size; ++i) {
        delete database.entries[i];
    }

    // 엔트리 배열의 메모리를 해제하고 포인터를 nullptr로 초기화한다.
    delete[] database.entries;
    database.entries = nullptr;
    database.size = 0;
}
