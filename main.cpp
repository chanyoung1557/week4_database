#include <iostream>
#include <cstring>
#include <string>
#include <cstdlib>
#include "database.h"

using namespace std;

// 엔트리 출력 함수
void printEntry(void* value, Type type, int size = 0) {
    switch (type) {
    case INT: {
        int* data = static_cast<int*>(value); // INT 타입의 값을 int 포인터로 캐스팅
        if (data != nullptr) { // 유효한 포인터인 경우
            if (size != 0) { // 배열의 경우
                for (int i = 0; i < size; ++i) {
                    std::cout << "int -> " << data[i] << std::endl; // 배열 요소 출력
                }
            }
            else {
                std::cout << "int -> " << *data << std::endl; // 배열이 아닌 경우 단일 값 출력
            }
        }
        else {
            std::cerr << "Invalid int value." << std::endl; // 유효하지 않은 포인터인 경우 오류 메시지 출력
        }
        break;
    }
    case DOUBLE: {
        double* data = static_cast<double*>(value); // DOUBLE 타입의 값을 double 포인터로 캐스팅
        if (data != nullptr) {
            if (size != 0) {
                for (int i = 0; i < size; ++i) {
                    std::cout << "double -> " << data[i] << std::endl;
                }
            }
            else {
                std::cout << "double -> " << *data << std::endl;
            }
        }
        else {
            std::cerr << "Invalid double value." << std::endl;
        }
        break;
    }
    case STRING: {
        std::string* data = static_cast<std::string*>(value); // STRING 타입의 값을 string 포인터로 캐스팅
        if (data != nullptr) {
            std::cout << "string -> \"" << *data << "\"" << std::endl;
        }
        else {
            std::cerr << "Invalid string value." << std::endl;
        }
        break;
    }
    case ARRAY: {
        Array* arr = static_cast<Array*>(value); // ARRAY 타입의 값을 Array 포인터로 캐스팅
        if (arr != nullptr) {
            std::cout << "array -> [";
            for (int i = 0; i < arr->size; ++i) {
                if (i > 0) std::cout << ", ";
                printEntry(arr->items, arr->type); // 배열의 항목 출력
            }
            std::cout << "]" << std::endl;
        }
        else {
            std::cerr << "Invalid array value." << std::endl;
        }
        break;
    }
    default:
        std::cerr << "Invalid entry type." << std::endl; // 잘못된 항목 타입 오류 출력
        break;
    }
}


// 배열 입력 함수
Array* inputArray() {
    Array* array = new Array; // 배열에 대한 포인터 동적 할당
    cout << "size: "; // 사용자에게 배열의 크기 입력을 요청하는 메시지 출력
    cin >> array->size; // 배열의 크기 입력 받기
    array->type; // 배열의 타입 설정
    array->items = new void* [array->size]; // 배열의 항목에 대한 포인터 배열 동적 할당

    for (int i = 0; i < array->size; ++i) {
        cout << "type (int, double, string, array): "; // 항목 타입 입력 요청
        string itemTypeStr;
        cin >> itemTypeStr; // 항목 타입 입력 받기

        // 항목 타입에 따라 처리
        if (itemTypeStr == "int")
        {
            int* value = new int[array->size]; // int 형 값에 대한 포인터 동적 할당
            int a = 0;
            cin >> a;
            value[i] = a;
            array->items = value;
            //array->items = value; // 배열의 항목에 값 할당
            array->type = INT;
        }
        else if (itemTypeStr == "double") {
            double* value = new double[array->size]; // double 형 값에 대한 배열 동적 할당
            double a = 0;
            cin >> a;
            value[i] = a;
            array->items = value; // 배열의 항목에 값 할당
            array->type = DOUBLE;
        }
        else if (itemTypeStr == "string") {
            string* value = new string[array->size]; // 문자열 값에 대한 배열 동적 할당

            cin.ignore(); // 이전 cin 입력의 개행 문자를 무시
            getline(cin, value[i]); // 공백을 포함한 문자열 입력 받기
            array->items = value; // 배열의 항목에 값 할당
            array->type = STRING;
        }
        else if (itemTypeStr == "array") {
            // 재귀적으로 배열 입력 받기
            Array* subArray = inputArray();
            if (subArray != nullptr) {
                array->items = (void*)subArray; // 배열의 항목에 배열 포인터 할당
                array->type = ARRAY;
            }
            else {
                delete[] array->items; // 동적 할당된 배열 메모리 해제
                delete array; // 동적 할당된 메모리 해제
                return nullptr; // nullptr 반환
            }
        }

    }

    return array; // 입력된 배열 포인터 반환
}

int main() {
    Database database; // 데이터베이스 객체 생성
    init(database); // 데이터베이스 초기화

    string command; // 사용자 입력을 저장할 변수
    while (true) {
        cout << "command (list, add, get, del, exit): ";
        cin >> command;

        if (command == "list") {
            // 전체 엔트리 출력
            for (int i = 0; i < database.size; ++i) { // 데이터베이스의 크기만큼 반복
                Entry* entry = database.entries[i]; // 현재 엔트리 가져오기
                cout << entry->key << ": "; // 엔트리의 키 출력
                printEntry(entry->value, entry->type); // 엔트리 값 출력
            }
        }
        else if (command == "add") {
            string key, valueType, valueStr; // 키, 값의 타입, 값 문자열을 저장할 변수 선언
            cout << "key: ";
            cin >> key; // 키 입력 받기

            cout << "type (int, double, string, array): ";
            cin >> valueType; // 값의 타입 입력 받기

            Type type; // 입력된 값의 타입을 저장할 변수 선언
            // 타입에 따라 처리
            if (valueType == "int") {
                type = INT; // 입력된 값의 타입이 int일 경우
                int* value = new int; // int 형 값에 대한 포인터 동적 할당
                cin >> *value;
                add(database, create(type, key, (void*)value)); // 데이터베이스에 새 항목 추가
            }
            else if (valueType == "double") {
                type = DOUBLE; // 입력된 값의 타입이 double일 경우
                double* value = new double; // double 형 값에 대한 포인터 동적 할당
                cin >> *value;
                add(database, create(type, key, (void*)value)); // 데이터베이스에 새 항목 추가
            }
            else if (valueType == "string") {
                type = STRING; // 입력된 값의 타입이 string일 경우
                cin.ignore(); // 이전 cin 입력의 개행 문자를 무시
                getline(cin, valueStr); // 공백을 포함한 문자열 입력 받기
                string* value = new string(valueStr); // 문자열 값에 대한 포인터 동적 할당
                add(database, create(type, key, (void*)value)); // 데이터베이스에 새 항목 추가
            }
            else if (valueType == "array") {
                type = ARRAY; // 입력된 값의 타입이 array일 경우
                Array* array = inputArray(); // 배열 입력 받기
                if (array != nullptr) { // 유효한 배열이 입력된 경우
                    add(database, create(type, key, (void*)array)); // 데이터베이스에 새 항목 추가
                }
                else {
                    break;
                }
            }
            else {
                cerr << "Invalid value type." << endl; // 잘못된 값 타입 오류 출력
            }
        }
        else if (command == "get") {
            string key; // 키를 저장할 변수 선언
            cout << "Enter key: "; // 사용자에게 키 입력을 요청하는 메시지 출력
            cin >> key; // 키 입력 받기
            Entry* entry = get(database, key); // 데이터베이스에서 해당 키의 엔트리 가져오기
            if (entry != nullptr) {
                cout << entry->key << ": "; // 엔트리의 키 출력
                printEntry(entry->value, entry->type); // 엔트리 값 출력
            }
            else {
                cerr << "Entry not found." << endl; // 엔트리가 없는 경우 오류 출력
            }
        }
        else if (command == "del") {
            string key; // 삭제할 키를 저장할 변수 선언
            cout << "Enter key: "; // 사용자에게 키 입력을 요청하는 메시지 출력
            cin >> key; // 키 입력 받기
            remove(database, key); // 데이터베이스에서 해당 키의 항목 삭제
        }
        else if (command == "exit") {
            destroy(database); // 데이터베이스 객체 소멸
            break; // 루프 종료
        }
        else {
            cerr << "Invalid command." << endl; // 잘못된 명령어 오류 출력
        }
    }

    return 0; // 프로그램 종료
}

