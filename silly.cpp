// Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include "TableEntry.h"
#include "Table.h"

using namespace std;

int main(int argc, char * argv[]) {
    bool quiet_mode = false;
    
    // Main data structure: associates table names with a table structure
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
    // Start of program
    string cmd;
    do {
        cout << "% ";
        cin >> cmd;

        // Check the command
        if(cmd == "CREATE") {
            int num_cols;
            string table_name;
            vector<EntryType> col_types;
            vector<string> col_names;

            cin >> table_name >> num_cols;

            col_types.resize((unsigned long)num_cols);

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
                col_names[i] = temp;
            }

            /*

            We now have:
            1) Vector of types 
            2) Map of column names with associated index (hash names to index)
            CALL ctor for table and create a 2d vector using the EntryTypes

            */
            
        } else if(cmd == "#") {
            string junk;
            getline(cin, junk);
        }


    } while(cmd != "QUIT");

    if(!quiet_mode) {
        cout << "Thanks for being silly!\n";
    }
    // Use return 0 instead of exit(0) so that locally scoped non-static destructors are called
    return 0;
}