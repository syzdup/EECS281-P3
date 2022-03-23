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
    int get_column_index(std::string column_name);
    void print(bool quiet_mode);
    void print_all(std::vector<std::string> &cols_to_print, std::vector<int> &col_indices, bool quiet_mode);
    void print_where(bool quiet_mode);
};