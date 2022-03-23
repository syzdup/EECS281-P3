// Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include "TableEntry.h"
#include "Table.h"

using namespace std;

TableEntry create_entry(EntryType current_type) {
    if(current_type == EntryType::Bool) {
        bool bool_entry;
        cin >> bool_entry;
        return TableEntry(bool_entry);
    } else if(current_type == EntryType::String) {
        string string_entry;
        cin >> string_entry;
        return TableEntry(string_entry); 
    } else if(current_type == EntryType::Int) {
        int int_entry;
        cin >> int_entry;
        return TableEntry(int_entry);
    } else {
        double double_entry;
        cin >> double_entry;
        return TableEntry(double_entry);
    }
}

// Returns an index of the column name where column_name matches the column. If it is not found, returns -1 as in index
int get_column_index(unordered_map<string, Table> &tables, string table_name, string column_name) {
    for(int i = 0; i < int(tables[table_name].col_names.size()); ++i) {
        if(tables[table_name].col_names[(unsigned long)i] == column_name) {
            return i;
        }
    }
    return -1;
}

void CREATE_cmd(unordered_map<string, Table> &tables, bool quiet_mode) {

    /*


        NOTE: turn into table ctor and add it to the vector of tables


    */
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
        cout << "Error during CREATE: Cannot create already existing table " << table_name << "\n";
    }
}

void INSERT_cmd(unordered_map<string,Table> &tables, bool quiet_mode) {

    /*


        NOTE: create member function


    */
    string junk;
    cin >> junk;

    string table_name;
    size_t num_rows;
    cin >> table_name;
    cin >> num_rows;
    cin >> junk;
    if(tables.find(table_name) != tables.end()) {

        size_t current_size = tables[table_name].data.size();
        tables[table_name].data.reserve(current_size + num_rows);

        for(size_t i = current_size; i < (unsigned long)(current_size + num_rows); ++i) {
            vector<TableEntry> new_row;
            new_row.reserve(tables[table_name].col_names.size());
            for(size_t j = 0; j < tables[table_name].col_names.size(); ++j) {
                EntryType current_type = tables[table_name].col_types[j];
                new_row.emplace_back(create_entry(current_type));
            }
            tables[table_name].data.emplace_back(new_row);
        }

        if(!quiet_mode) {
            cout << "Added " << num_rows << " rows to " << table_name << " from position " << tables[table_name].data.size() - num_rows <<
            " to " << tables[table_name].data.size() - 1 << "\n";
        }
    } else {
        cout << table_name << " is not the name of a table in the database\n";
    }
}

void REMOVE_cmd(unordered_map<string, Table> &tables, bool quiet_mode) {
    string table_name;
    cin >> table_name;

    if(tables.find(table_name) != tables.end()) {
        tables.erase(table_name);
        if(!quiet_mode) {
            cout << "Table " << table_name << " deleted\n";
        }
    } else {
        cout << "Error during REMOVE: " << table_name << " does not name a table in the database\n";
        getline(cin, table_name);
    }
}

// break up into print all and print where, make member function of a table 
// print where
// 1) figure out column name (if it exists) -> type -> call helper with a table entry containing the type you are looking for -> 3-way split on comparison type
// delete where
// 1) stl function remove things O(n), overwrite and rearrange deleted things (functor accepts a row)
void PRINT_cmd(unordered_map<string, Table> &tables, bool quiet_mode) {
    string table_name;
    cin >> table_name;
    cin >> table_name;
    tables[table_name].print(quiet_mode);
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
            Performance note:
            
            - can change to index strings instead of compare them? (how to find unrecognized commands)
        */
       
        if(cmd == "CREATE") {
            CREATE_cmd(tables, quiet_mode);
        } else if(cmd == "INSERT") {
            INSERT_cmd(tables, quiet_mode);
        } else if(cmd == "PRINT") {
            PRINT_cmd(tables, quiet_mode);
        } else if(cmd == "REMOVE") {
            REMOVE_cmd(tables, quiet_mode);
        } else if(cmd == "DELETE") {
            getline(cin, junk);
        } else if(cmd == "GENERATE") {
            getline(cin, junk);
        } else if(cmd == "JOIN") {
            getline(cin, junk);
        } else if(cmd == "#") {
            string junk;
            getline(cin, junk);
        } else if(cmd != "QUIT"){
            cout << "Error: unrecognized command\n";
            getline(cin, junk);
        }
    } while(cmd != "QUIT");

    if(!quiet_mode) {
        cout << "Thanks for being silly!\n";
    }
    // Use return 0 instead of exit(0) so that locally scoped non-static destructors are called
    return 0;
}