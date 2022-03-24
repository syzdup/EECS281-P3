// Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D
#include "TableEntry.h"
#include <vector>
#include <string>

class Table {
    public:

    // Default ctor added to bypass compile error:
    // std::unordered_map needs a default ctor provided for init
    Table(){}

    Table(std::string name_in, std::vector<EntryType> types_in, std::vector<std::string> names_in)
        :name{name_in}, col_types{types_in}, col_names{names_in} { }

    // Metadata
    std::string name;
    std::vector<EntryType> col_types;
    std::vector<std::string> col_names;

    // Actual data
    std::vector<std::vector<TableEntry> > data;

    // Member functions
    TableEntry create_entry(EntryType current_type);
    int get_column_index(std::string column_name);
    void insert(bool quiet_mode);
    void print(bool quiet_mode);
    void print_all(std::vector<std::string> &cols_to_print, std::vector<int> &col_indices, bool quiet_mode);
    void print_where(std::vector<std::string> &cols_to_print, std::vector<int> &col_indices, bool quiet_mode);
    void delete_where(bool quiet_mode);
};

// Functors
class Less_Comp {
    private:
    size_t col_index;
    TableEntry comp_val;

    public:
    Less_Comp(size_t col_index_in, TableEntry comp_val_in) 
        : col_index(col_index_in), comp_val(comp_val_in) {}
    
    bool operator()(std::vector<TableEntry> row) {
        return row[col_index] < comp_val;
    }
};