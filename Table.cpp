// Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D
#include "Table.h"
#include <iostream>
#include <vector>
#include <string>

// Returns a TableEntry object given an EntryType
TableEntry Table::create_entry(EntryType current_type) {
    if(current_type == EntryType::Bool) {
        bool bool_entry;
        std::cin >> bool_entry;
        return TableEntry(bool_entry);
    } else if(current_type == EntryType::String) {
        std::string string_entry;
        std::cin >> string_entry;
        return TableEntry(string_entry); 
    } else if(current_type == EntryType::Int) {
        int int_entry;
        std::cin >> int_entry;
        return TableEntry(int_entry);
    } else {
        double double_entry;
        std::cin >> double_entry;
        return TableEntry(double_entry);
    }
}

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

void Table::insert(bool quiet_mode) {
    size_t num_rows;
    std::string junk;
    std::cin >> num_rows;
    std::cin >> junk; // throw away ("ROWS")

    data.reserve(data.size() + num_rows);
    size_t end_size =  (unsigned long)(data.size() + num_rows);

    for(size_t i = data.size(); i < end_size; ++i) {
        std::vector<TableEntry> new_row;
        new_row.reserve(col_names.size());

        for(size_t j = 0; j < col_names.size(); ++j) {
            //std::cout << "SECOND FOR LOOP ENTERED\n";
            EntryType current_type = col_types[j];
            new_row.emplace_back(create_entry(current_type));
        }
        data.emplace_back(new_row);
    }
    if(!quiet_mode) {
        std::cout << "Added " << num_rows << " rows to " << name << " from position " << data.size() - num_rows <<
        " to " << data.size() - 1 << "\n";
    }
}