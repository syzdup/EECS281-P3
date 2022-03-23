// Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D
#include "Table.h"
#include <iostream>
#include <vector>
#include <string>

// Returns the index where 'column_name' matches the name of the column in the table
int Table::get_column_index(std::string column_name) {
    for(int i = 0; i < int(col_names.size()); ++i) {
        if(col_names[(unsigned long)i] == column_name) {
            return i;
        }
    }
    return -1;
}

// Reads columns to print, gets indices, then passes on work to print_all or print_where
void Table::print(bool quiet_mode) {

    size_t num_cols;
    std::cin >> num_cols;

    std::vector<std::string> cols_to_print;
    std::vector<int> col_indices;

    cols_to_print.reserve(num_cols);
    col_indices.reserve(num_cols);

    std::string temp_name;
    for(size_t i = 0; i < num_cols; ++i) {
        std::cin >> temp_name;
        cols_to_print.push_back(temp_name);
    }
    for(size_t i = 0; i < cols_to_print.size(); ++i) {
        int col_index = get_column_index(cols_to_print[i]);
        if(col_index != -1) {
            col_indices.push_back(col_index);
        // if a column provided by input does not match any column in table
        } else {
            std::cout << "Error during PRINT: " << cols_to_print[i] << " does not name a column in " << name << "\n";
        }
    }
    // Print column names
    if(!quiet_mode) {
        for(size_t col = 0; col < cols_to_print.size(); ++col) {
            std::cout << cols_to_print[col] << " ";
        }
        std::cout << "\n";
    }
    // Check: WHERE or ALL
    std::cin >> temp_name;
    if(temp_name == "ALL") {
        print_all(cols_to_print, col_indices, quiet_mode);
    } else {
        print_where(quiet_mode);
    }
}

void Table::print_all(std::vector<std::string> &cols_to_print, std::vector<int> &col_indices, bool quiet_mode) {
    for(size_t row = 0; row < data.size(); ++row) {
        for(size_t col = 0; col < cols_to_print.size(); ++col) {
            if(!quiet_mode){
                std::cout << data[row][(unsigned long)col_indices[col]];
                std::cout << " ";
            }
        }
        if(!quiet_mode) {
            std::cout << "\n";
        }
    }
    std::cout << "Printed " << data.size() << " matching rows from " << name << "\n";
}

void Table::print_where(bool quiet_mode) {
    if(!quiet_mode) {
        std::cout << "Print where called\n";
    }
}