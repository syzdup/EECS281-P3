// Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D
#include "TableEntry.h"
#include <vector>
#include <string>

// Functor (Paoletti video said this is bad and slow, make three functors)
enum class CompType : uint8_t { Less, Greater, Equal };

class Entry_Comp {
    private:
    size_t col_index;
    TableEntry comp_val;
    CompType comp_type;

    public:
    Entry_Comp(size_t col_index_in, TableEntry comp_val_in, CompType comp_type_in) 
        : col_index(col_index_in), comp_val(comp_val_in), comp_type(comp_type_in) {}
    
    bool operator()(const std::vector<TableEntry> &row) {
        if(comp_type == CompType::Less) {
            return row[col_index] < comp_val;
        } else if(comp_type == CompType::Greater) {
            return row[col_index] > comp_val;
        } else {
            return row[col_index] == comp_val;
        }
    }
};

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
    // Later: move helper functions to private
    TableEntry create_entry(EntryType current_type);
    int get_column_index(std::string column_name);
    void insert();
    void print(bool quiet_mode);
    void print_all(std::vector<std::string> &cols_to_print, std::vector<int> &col_indices, bool quiet_mode);
    void print_where_helper(std::vector<std::string> &cols_to_print, std::vector<int> &col_indices, bool quiet_mode, Entry_Comp entry_comparator);
    void print_where(std::vector<std::string> &cols_to_print, std::vector<int> &col_indices, bool quiet_mode);
    void delete_where();
};