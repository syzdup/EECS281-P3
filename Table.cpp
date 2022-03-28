// Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D
#include "Table.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

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

// Returns the index where 'column_name' matches the name of the column in the table: O(n)
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
            getline(std::cin, temp_name);
            std::cout << "Error during PRINT: " << cols_to_print[i] << " does not name a column in " << name << "\n";
            return;
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
        print_where(cols_to_print, col_indices, quiet_mode);
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

void Table::bst_print_where(std::vector<std::string> &cols_to_print, std::vector<int> &col_indices, bool quiet_mode, CompType comp_type, TableEntry entry) {
    size_t row_count = 0;
    std::vector<size_t> row_indices;
    if(comp_type == CompType::Less) {
        auto lower_bound = bst_index.lower_bound(entry);
        for(auto it = bst_index.begin(); it != lower_bound; ++it) {
            for(size_t row = 0; row < it->second.size(); ++row) {
                row_count += 1;
                row_indices.push_back(it->second[row]);
            }
        }
    } else if(comp_type == CompType::Greater) {
        auto upper_bound = bst_index.upper_bound(entry);
        for(auto it = upper_bound; it != bst_index.end(); ++it) {
            for(size_t row = 0; row < it->second.size(); ++row) {
                row_count += 1;
                row_indices.push_back(it->second[row]);
            }
        }
    } else {
        auto match = bst_index.find(entry);
        if(match != bst_index.end()) {
            for(size_t row = 0; row < match->second.size(); ++row) {
                row_count += 1;
                row_indices.push_back(match->second[row]);
            }
        }
    }
    for(size_t r = 0; r < row_indices.size(); ++r) {
        for(size_t col = 0; col < cols_to_print.size(); ++col) {
            if(!quiet_mode) {
                std::cout << data[row_indices[r]][(unsigned long)col_indices[col]];
                std::cout << " ";
            }
        }
        if(!quiet_mode) {
            std::cout << "\n";
        }
    }
    std::cout << "Printed " << row_count << " matching rows from " << name << "\n";
}

void Table::print_where_helper(std::vector<std::string> &cols_to_print, std::vector<int> &col_indices, bool quiet_mode, Entry_Comp entry_comparator) {
    size_t row_count = 0;
    for(size_t row = 0; row < data.size(); ++row) {
        if(entry_comparator(data[row])) {
            row_count += 1;
            for(size_t col = 0; col < cols_to_print.size(); ++col) {
                if(!quiet_mode) {
                    std::cout << data[row][(unsigned long)col_indices[col]];
                    std::cout << " ";
                }
            }
        }
        if(entry_comparator(data[row])) {
            if(!quiet_mode) {
                std::cout << "\n";
            }
        }
    }
    std::cout << "Printed " << row_count << " matching rows from " << name << "\n";
}

void Table::print_where(std::vector<std::string> &cols_to_print, std::vector<int> &col_indices, bool quiet_mode) {
    std::string column_name;
    std::string compare_operator;
    int col_index;
    std::cin >> column_name;
    std::cin >> compare_operator;
    col_index = get_column_index(column_name);
    if(col_index == -1) {
        getline(std::cin, compare_operator);
        std::cout << column_name << " is not the name of a column in the table specified by " << name << "\n";
        return;
    }
    EntryType compare_type = col_types[(unsigned long)col_index];
    
    if(compare_operator == "<") {
        if(bst_index_on && (unsigned long)col_index == indexed_column) {
            bst_print_where(cols_to_print, col_indices, quiet_mode, CompType::Less, create_entry(compare_type));  
        } else {
            Entry_Comp entry_comparator((unsigned long)col_index, create_entry(compare_type), CompType::Less);
            print_where_helper(cols_to_print, col_indices, quiet_mode, entry_comparator);
        }
    } else if(compare_operator == ">") {
        if(bst_index_on && (unsigned long)col_index == indexed_column) {
            bst_print_where(cols_to_print, col_indices, quiet_mode, CompType::Greater, create_entry(compare_type));  
        } else {
            Entry_Comp entry_comparator((unsigned long)col_index, create_entry(compare_type), CompType::Greater);
            print_where_helper(cols_to_print, col_indices, quiet_mode, entry_comparator);
        }
    } else {
        if(bst_index_on && (unsigned long)col_index == indexed_column) {
            bst_print_where(cols_to_print, col_indices, quiet_mode, CompType::Equal, create_entry(compare_type));  
        } else {
            Entry_Comp entry_comparator((unsigned long)col_index, create_entry(compare_type), CompType::Equal);
            print_where_helper(cols_to_print, col_indices, quiet_mode, entry_comparator);
        }
    }
}

void Table::insert() {
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
            EntryType current_type = col_types[j];
            new_row.emplace_back(create_entry(current_type));
        }
        data.emplace_back(new_row);
        if(hash_index_on) {
            hash_index[data[i][(unsigned long)indexed_column]].push_back(i);
        } else if(bst_index_on) {
            bst_index[data[i][(unsigned long)indexed_column]].push_back(i);
        }
    }
    std::cout << "Added " << num_rows << " rows to " << name << " from position " << data.size() - num_rows <<
    " to " << data.size() - 1 << "\n";
}

void Table::delete_where() {
    size_t rows_deleted = data.size();
    std::string column_name;
    std::cin >> column_name; // throw away ("WHERE")
    std::cin >> column_name;
    std::string compare_operator;
    int col_index;
    std::cin >> compare_operator;
    col_index = get_column_index(column_name);

    if(col_index == -1) {
        getline(std::cin, compare_operator);
        std::cout << "Error during DELETE: " << column_name << " does not name a column in " << name << "\n";
        return;
    }
    if(hash_index_on || bst_index_on) {
        hash_index.clear();
        bst_index.clear();
    }
    EntryType compare_type = col_types[(unsigned long)col_index];
    if(compare_operator == "<") {
        auto it = std::remove_if(data.begin(), data.end(), Entry_Comp((unsigned long)col_index, create_entry(compare_type), CompType::Less));
        data.erase(it, data.end());

    } else if(compare_operator == ">") {
        auto it = std::remove_if(data.begin(), data.end(), Entry_Comp((unsigned long)col_index, create_entry(compare_type), CompType::Greater));
        data.erase(it, data.end());
        
    } else {
        auto it = std::remove_if(data.begin(), data.end(), Entry_Comp((unsigned long)col_index, create_entry(compare_type), CompType::Equal));
        data.erase(it, data.end());
    }
    rows_deleted -= data.size();
    if(hash_index_on) {
        for(size_t row = 0; row < data.size(); ++row) {
            hash_index[data[row][indexed_column]].push_back(row);
        }
    } else if(bst_index_on) {
        for(size_t row = 0; row < data.size(); ++row) {
            bst_index[data[row][indexed_column]].push_back(row);
        }
    }
    std::cout << "Deleted " << rows_deleted << " rows from " << name << "\n";
}

// Generates an index: H = hash index, B = bst index
void Table::generate_index(char type) {
    if(!hash_index.empty() || !bst_index.empty()) {
        hash_index.clear();
        bst_index.clear();
    }
    std::string column_name;
    std::cin >> column_name;
    int col_indice = get_column_index(column_name);
    if(col_indice == -1) {
        std::cout << "Error during GENERATE: " << column_name << " does not name a column in " << name << "\n";
        return;
    }
    if(type == 'H') {
        hash_index_on = true;
        indexed_column = (unsigned long)col_indice;
        for(size_t row = 0; row < data.size(); ++row) {
            hash_index[data[row][(unsigned long)col_indice]].push_back(row);
        }
        std::cout << "Created hash index for table " << name << " on column " << column_name << "\n";
    } else {
        bst_index_on = true;
        indexed_column = (unsigned long)col_indice;
        for(size_t row = 0; row < data.size(); ++row) {
            bst_index[data[row][(unsigned long)col_indice]].push_back(row);
        }
        std::cout << "Created bst index for table " << name << " on column " << column_name << "\n";
    }
}