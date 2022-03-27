// Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include "TableEntry.h"
#include "Table.h"

using namespace std;

// Creates a table
void CREATE_cmd(unordered_map<string, Table> &tables, bool quiet_mode) {
    int num_cols;
    string table_name;
    vector<EntryType> col_types;
    vector<string> col_names;
    cin >> table_name >> num_cols;
    // Check for table in map
    if(tables.find(table_name) == tables.end()) {
        col_types.resize((unsigned long)num_cols);
        col_names.resize((unsigned long)num_cols);
        // Read column types
        for(int i = 0; i < num_cols; ++i) {
            string temp;
            cin >> temp;
            
            if(temp == "bool") {
                col_types[(unsigned long)i] = EntryType::Bool;
            } else if(temp == "string") {
                col_types[(unsigned long)i] = EntryType::String;
            } else if(temp == "int") {
                col_types[(unsigned long)i] = EntryType::Int;
            } else {
                col_types[(unsigned long)i] = EntryType::Double;
            }
        }
        // Read column names 
        for(int i = 0; i < num_cols; ++i) {
            string temp;
            cin >> temp;
            col_names[(unsigned long)i] = temp;
        }
        // Create table object and add it to map
        Table new_table(table_name, col_types, col_names);
        tables.insert({table_name, new_table});

        if(!quiet_mode) {
            cout << "New table " << table_name << " with column(s) ";
            for(size_t i = 0; i < col_names.size(); ++i) {
                cout << col_names[i] << " ";
            }
            cout << "created\n";
        }
    } else {
        std::string junk;
        getline(std::cin, junk);
        cout << "Error during CREATE: Cannot create already existing table " << table_name << "\n";
    }
}

// Makes a call to "insert()" member function of table
void INSERT_cmd(unordered_map<string,Table> &tables) {
    std::string junk;
    string table_name;
    cin >> table_name; // throw away ("INTO")
    cin >> table_name;
    // input number and getline rows if wrong
    if(tables.find(table_name) != tables.end()) {
        tables[table_name].insert();
    } else {
        getline(std::cin, junk);
        cout << "Error during INSERT: " << table_name << " does not name a table in the database\n";
    }
}

// Calls ".erase()" on the specified table in map
void REMOVE_cmd(unordered_map<string, Table> &tables) {
    std::string junk;
    string table_name;
    cin >> table_name;

    if(tables.find(table_name) != tables.end()) {
        tables.erase(table_name);
        cout << "Table " << table_name << " deleted\n";
    } else {
        getline(cin, junk);
        cout << "Error during REMOVE: " << table_name << " does not name a table in the database\n";
    }
}

// Makes a call to "print(bool quiet_mode)" member function of table
void PRINT_cmd(unordered_map<string, Table> &tables, bool quiet_mode) {
    std::string junk;
    string table_name;
    cin >> table_name; // throw away ("FROM")
    cin >> table_name;
    if(tables.find(table_name) != tables.end()) {
        tables[table_name].print(quiet_mode);
    } else {
        getline(std::cin, junk);
        cout << "Error during PRINT: " << table_name << " does not name a table in the database\n";
    }
}

void print_joined_row(unordered_map<std::string, Table> &tables, std::string table_first, std::string table_second, std::vector<std::string> 
                      &print_cols, std::vector<int> &table_num, size_t row_first, size_t row_second, bool quiet_mode) {

    for(size_t col = 0; col < print_cols.size(); ++col) {
        if(table_num[col] == 1) {
            if(!quiet_mode) {
                std::cout << tables[table_first].data[row_first][(unsigned long)tables[table_first].get_column_index(print_cols[col])] << " ";
            }
        } else {
            if(!quiet_mode) {
                std::cout << tables[table_second].data[row_second][(unsigned long)tables[table_second].get_column_index(print_cols[col])] << " ";
            }
        }
    }
    std::cout << "\n";
}

// Does the heavy lifting for JOIN
void JOIN_helper(unordered_map<std::string, Table> &tables, std::string table_first, std::string col_first, std::string table_second, std::string col_second, bool quiet_mode) {
    size_t num_cols;
    std::cin >> num_cols;

    std::vector<std::string> print_cols;
    print_cols.reserve(num_cols);
    std::vector<int> table_num;
    table_num.reserve(num_cols);

    std::string col;
    int num;
    for(size_t i = 0; i < num_cols; ++i) {
        std::cin >> col;
        std::cin >> num;
        print_cols.push_back(col);
        table_num.push_back(num);
    }
    // Check that cols exist
    for(size_t i = 0; i < num_cols; ++i) {
        if(table_num[i] == 1) {
            if(tables[table_first].get_column_index(print_cols[i]) == -1) {
                std::cout << "Error during JOIN: " << print_cols[i] << " is not the name of a column in the table specified by " << table_first << "\n";
                return;
            }
        } else {
            if(tables[table_second].get_column_index(print_cols[i]) == -1) {
                std::cout << "Error during JOIN: " << print_cols[i] << " is not the name of a column in the table specified by " << table_second << "\n";
                return;
            }
        }
    }
    size_t matched_rows = 0;
    size_t col_first_indice = (unsigned long)(tables[table_first].get_column_index(col_first));
    size_t col_second_indice = (unsigned long)(tables[table_second].get_column_index(col_second));

    // Print column names
    for(size_t col = 0; col < print_cols.size(); ++col) {
        std::cout << print_cols[col] << " ";
    }
    std::cout << "\n";

    for(size_t row_tbl_one = 0; row_tbl_one < tables[table_first].data.size(); ++row_tbl_one) {
        for(size_t row_tbl_two = 0; row_tbl_two < tables[table_second].data.size(); ++ row_tbl_two) {
            if(tables[table_first].data[row_tbl_one][col_first_indice] == tables[table_second].data[row_tbl_two][col_second_indice]) {
                matched_rows += 1;
                print_joined_row(tables, table_first, table_second, print_cols, table_num, row_tbl_one, row_tbl_two, quiet_mode);
            }
        }
    }
    std::cout << "Printed " << matched_rows << " rows from joining " << table_first << " to " << table_second << "\n";
}

// Looks for the index of the columns specified and returns whether the column name exists inside of the table
bool verify_join_columns(unordered_map<std::string, Table> &tables, std::string table, std::string col) {
    if(tables[table].get_column_index(col) == -1) {
        return false;
    }
    return true;
}

// Reads input, does some error handling, calls helper function
void JOIN_cmd(unordered_map<std::string, Table> &tables, bool quiet_mode) {
    std::string table_first;
    std::string table_second;
    std::string junk;

    std::cin >> table_first;
    std::cin >> junk; // throw away ("AND")
    std::cin >> table_second;

    if(tables.find(table_first) != tables.end()) {
        if(tables.find(table_second) != tables.end()) {
            // join
            std::cin >> junk; // throw away ("WHERE")
            std::string col_first;
            std::string col_second;

            std::cin >> col_first;
            std::cin >> junk; // throw away ("=", it is implied because no other operator can be specified)
            std::cin >> col_second;

            if(!verify_join_columns(tables, table_first, col_first)) {
                getline(std::cin, junk);
                std::cout << "Error during JOIN: " << col_first << " is not the name of a column in the table specified by " << table_first << "\n";
                return;
            } else if(!verify_join_columns(tables, table_second, col_second)) {
                getline(std::cin, junk);
                std::cout << "Error during JOIN: " << col_second << " is not the name of a column in the table specified by " << table_second << "\n";
                return;
            }

            std::cin >> junk; // throw away ("AND")
            std::cin >> junk; // throw away ("PRINT")

            // *** call to helper function ***
            JOIN_helper(tables, table_first, col_first, table_second, col_second, quiet_mode);

        } else {
            getline(std::cin, junk);
            std::cout << "Error during JOIN: " << table_second << " does not name a table in the database\n";

        }
    } else {
        getline(std::cin, junk);
        std::cout << "Error during JOIN: " << table_first << " does not name a table in the database\n";
    }
}

// Makes a call to "delete_where()" member function of table
void DELETE_cmd(unordered_map<string, Table> &tables) {
    std::string junk;
    string table_name;
    cin >> table_name; // throw away ("FROM")
    cin >> table_name;
    if(tables.find(table_name) != tables.end()) {
        tables[table_name].delete_where();
    } else {
        getline(std::cin, junk);
        cout << "Error during DELETE: " << table_name << " does not name a table in the database\n";
    }
}

void GENERATE_cmd(unordered_map<std::string, Table> &tables) {
    std::string table_name;
    std::cin >> table_name; // throw away ("FOR")
    std::cin >> table_name;

    if(tables.find(table_name) != tables.end()) {
        string index_type;
        cin >> index_type;
        if(index_type == "hash") {
            cin >> index_type; // throw away ("INDEX")
            cin >> index_type; // throw away ("ON")
            tables[table_name].generate_index('H');
        } else {
            cin >> index_type; // throw away ("INDEX")
            cin >> index_type; // throw away ("ON")
            tables[table_name].generate_index('B');
        }
    } else {
        std::string junk;
        getline(std::cin, junk);
        std::cout << "Error during GENERATE: " << table_name << " does not name a table in the database\n";
    }
}

int main(int argc, char * argv[]) {
    ios_base::sync_with_stdio(false);
    cin >> boolalpha;
    cout << boolalpha;

    bool quiet_mode = false;
    unordered_map<string, Table> tables;

    vector<string> arguments(argv, argv + argc);
    if(argc > 1) {
        for(string &s : arguments) {
            if(s == "-h" || s == "--help") {
                cout << "Help" << endl;
                exit(0);
            } else if(s == "-q" || s == "--quiet") {
                quiet_mode = true;
            }
        }
    }
    string cmd;
    string junk;
    do {
        if(cin.fail()) {
            cerr << "Error: Reading from cin has failed" << endl;
            exit(1);
        }
        cout << "% ";
        cin >> cmd;
        /*
            Performance notes:
            - can change to index strings instead of compare them? (how to find unrecognized commands)
            - consolidate _cmd functions 
        */
        if(cmd[0] == '#') {
            string junk;
            getline(cin, junk);
        } else if(cmd == "CREATE") {
            CREATE_cmd(tables, quiet_mode);
        } else if(cmd == "INSERT") {
            INSERT_cmd(tables);
        } else if(cmd == "PRINT") {
            PRINT_cmd(tables, quiet_mode);
        } else if(cmd == "REMOVE") {
            REMOVE_cmd(tables);
        } else if(cmd == "DELETE") {
            DELETE_cmd(tables);
        } else if(cmd == "GENERATE") {
            GENERATE_cmd(tables);
        } else if(cmd == "JOIN") {
            JOIN_cmd(tables, quiet_mode);
        } else {
            if(cmd != "QUIT") {
                getline(cin, junk);
                cout << "Error: unrecognized command\n";
            }
        }
    } while(cmd != "QUIT");

    if(!quiet_mode) {
        cout << "Thanks for being silly!\n";
    }
    // Use return 0 instead of exit(0) so that locally scoped non-static destructors are called
    return 0;
}