#include <iostream>
#include <cstring>
#include <string>
#include <cstdlib>
#include "database.h"

using namespace std;

void printEntry(Entry *entry) {
    Type type = entry->type;
    switch (type) {
        case INT:
            cout << "int -> " << *((int*)entry->value) << endl;
            break;
        case DOUBLE:
            cout << "double -> " << *((double*)entry->value) << endl;
            break;
        case STRING:
            cout << "string -> \"" << *((string*)entry->value) << "\"" << endl;
            break;
        case ARRAY: {
            Array *arr = (Array*)entry->value;
            cout << "array -> [";
            switch (arr->type) {
                case INT:
                    for (int i = 0; i < arr->size; ++i) {
                        if (i > 0) cout << ", ";
                        cout << ((int*)arr->items)[i];
                    }
                    break;
                case DOUBLE:
                    for (int i = 0; i < arr->size; ++i) {
                        if (i > 0) cout << ", ";
                        cout << ((double*)arr->items)[i];
                    }
                    break;
                case STRING:
                    for (int i = 0; i < arr->size; ++i) {
                        if (i > 0) cout << ", ";
                        cout << "\"" << ((string*)arr->items)[i] << "\"";
                    }
                    break;
                default:
                    break;
            }
            cout << "]" << endl;
            break;
        }
        default:
            cerr << "Invalid entry type." << endl;
            break;
    }
}

int main() {
    Database database;
    init(database);

    string command;
    while (true) {
        cout << "Enter command (list/add/get/del/exit): ";
        cin >> command;

        if (command == "list") {
            // Print all entries
            for (int i = 0; i < database.size; ++i) {
                Entry *entry = database.entries[i];
                cout << entry->key << ": ";
                printEntry(entry);
            }
        } else if (command == "add") {
            string key, valueType, valueStr;
            cout << "Enter key, type, value: ";
            cin >> key >> valueType;

            Type type;
            if (valueType == "int") {
                type = INT;
                int *value = new int;
                cin >> *value;
                add(database, create(type, key, value));
            } else if (valueType == "double") {
                type = DOUBLE;
                double *value = new double;
                cin >> *value;
                add(database, create(type, key, value));
            } else if (valueType == "string") {
                type = STRING;
                cin.ignore();
                getline(cin, valueStr);
                string *value = new string(valueStr);
                add(database, create(type, key, value));
            } else if (valueType == "array") {
                type = ARRAY;
                int size;
                cout << "Enter array size: ";
                cin >> size;
                Array *array = new Array;
                array->size = size;
                array->type = type;
                array->items = nullptr;
                // Allocate memory for array items based on type
                switch (type) {
                    case INT: {
                        int *values = new int[size];
                        cout << "Enter " << size << " integer values: ";
                        for (int i = 0; i < size; ++i) {
                            cin >> values[i];
                        }
                        array->items = values;
                        break;
                    }
                    case DOUBLE: {
                        double *values = new double[size];
                        cout << "Enter " << size << " double values: ";
                        for (int i = 0; i < size; ++i) {
                            cin >> values[i];
                        }
                        array->items = values;
                        break;
                    }
                    case STRING: {
                        string *values = new string[size];
                        cin.ignore();
                        cout << "Enter " << size << " string values: ";
                        for (int i = 0; i < size; ++i) {
                            getline(cin, values[i]);
                        }
                        array->items = values;
                        break;
                    }
                    default:
                        break;
                }
                add(database, create(type, key, array));
            } else {
                cerr << "Invalid value type." << endl;
            }
        } else if (command == "get") {
            string key;
            cout << "Enter key: ";
            cin >> key;
            Entry *entry = get(database, key);
            if (entry != nullptr) {
                cout << entry->key << ": ";
                printEntry(entry);
            } else {
                cerr << "Entry not found." << endl;
            }
        } else if (command == "del") {
            string key;
            cout << "Enter key: ";
            cin >> key;
            remove(database, key);
        } else if (command == "exit") {
            destroy(database);
            break;
        } else {
            cerr << "Invalid command." << endl;
        }
    }

    return 0;
}
