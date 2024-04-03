#include <iostream>
#include <cstring>
#include <string>
#include <cstdlib>
#include "database.h"

int main() {
    // 사용자에게 입력할 명령어를 저장할 변수
    std::string cmType = "command (list, add, get, del, exit): ";
    std::string command, key, typeInput;
    int int_value; // int 타입의 값
    double dou_value; // double 타입의 값
    void* value = &int_value; // 입력된 값의 포인터, 초기값은 int_value로 설정

    Database database; // 데이터베이스 객체
    Entry entry; // 엔트리 객체
    Entry* entry_p = &entry; // 엔트리 포인터
    Type type = INT; // 엔트리의 타입, 초기값은 INT로 설정

    // 데이터베이스 초기화
    init(database);
    // 사용자 입력 받기
    while (true) {
        std::cout << cmType; // 사용자에게 명령어 입력을 요청하는 메시지 출력
        std::cin >> command; // 사용자 입력 받기

        // command 가 list 인 경우
        if (command == "list") {
            list(database); // 데이터베이스의 모든 엔트리 출력
        }
        // command 가 add 인 경우
        else if (command == "add") {
            std::cout << "key: ";
            std::cin >> key;
            std::cout << "type (int, double, string, array): ";
            std::cin >> typeInput;
            std::cout << "value: ";
            // 타입에 따라 값 입력 받기
            if (typeInput == "int") {
                type = INT;
                std::cin >> int_value;
                value = &int_value;
                entry_p = create(type, key, value);
            }
            else if (typeInput == "double") {
                type = DOUBLE;
                std::cin >> dou_value;
                value = &dou_value;
                entry_p = create(type, key, value);
            }
            else if (typeInput == "string") {
                type = STRING;
                std::cin.ignore(); // 입력 버퍼 비우기
                std::string str_input;
                std::getline(std::cin, str_input);
                value = new std::string(str_input); // 문자열을 동적할당하여 value에 저장
                entry_p = create(type, key, value);
            }
            else if (typeInput == "array") {
                // 배열 객체 생성 및 초기화
                Array* arr_p = new Array;
                arr_p->size = 0;
                arr_p->type = INT;
                arr_p->items = nullptr;
                initArray(arr_p);
                value = static_cast<Array*>(arr_p);
                entry_p = create(ARRAY, key, value);
            }
            else {
                std::cerr << "invalid type" << std::endl; // 잘못된 타입 오류 출력
                continue; // 반복문 계속 진행
            }

            // 데이터베이스에 엔트리 추가
            if (database.size == 0) { // 데이터베이스가 비어있는 경우
                database.db_array[0] = *entry_p;
                database.size++;
            }
            else { // 데이터베이스에 이미 엔트리가 있는 경우
                add(database, entry_p);
            }

        }

        // command 가 get 인 경우
        else if (command == "get") {
            std::cout << "key: ";
            std::cin >> key;
            entry_p = get(database, key);
            if (entry_p->key == key) {
                std::cout << entry_p->key << ": ";

                // 엔트리의 값 출력
                if (entry_p->type == INT) {
                    std::cout << *(int*)(entry_p->value);
                }
                else if (entry_p->type == DOUBLE) {
                    std::cout << *(double*)(entry_p->value);
                }
                else if (entry_p->type == STRING) {
                    std::cout << "\"" << *(std::string*)(entry_p->value) << "\"";
                }
                else if (entry_p->type == ARRAY) {
                    Array* arr = static_cast<Array*>(entry_p->value);
                    listArray(*arr); // 배열의 항목 출력
                }
                std::cout << std::endl;
            }
            else { // 해당하는 key를 찾지 못한 경우
                std::cerr << "can't found" << std::endl;
            }

        }
        // command 가 del 인 경우
        else if (command == "del") {
            std::cout << "key: ";
            std::cin >> key;
            remove(database, key); // 해당 key의 엔트리 삭제
        }
        // command 가 exit 인 경우
        else if (command == "exit") {
            destroy(database); // 데이터베이스 객체 해제
            exit(1);
        }
        // 잘못된 명령어인 경우
        else {
            std::cerr << "invalid command" << std::endl;
        }

        std::cout << std::endl;
    }
    return 0;
}
