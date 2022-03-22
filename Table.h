// Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D
#include <vector>
#include <string>

class Table {
    public:

    Table(std::string name_in, std::vector<EntryType> types_in, std::vector<std::string> names_in)
        :name{name_in}, col_types{types_in}, col_names{names_in} { }

    // Metadata
    std::string name;
    std::vector<EntryType> col_types;
    std::vector<std::string> col_names;

    // Actual data
    std::vector<std::vector<TableEntry> > data;
};