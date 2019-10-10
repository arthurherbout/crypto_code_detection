#include <vector>

#include "csv_utility.hpp"
#include "csv_reader.hpp"

namespace csv {
    /** Shorthand function for parsing an in-memory CSV string,
     *  a collection of CSVRow objects
     *
     *  @snippet tests/test_read_csv.cpp Parse Example
     */
    CSV_INLINE CSVCollection parse(csv::string_view in, CSVFormat format) {
        CSVReader parser(format);
        parser.feed(in);
        parser.end_feed();
        return parser.records;
    }

    /** Parse a RFC 4180 CSV string, returning a collection
     *  of CSVRow objects
     *
     *  @par Example
     *  @snippet tests/test_read_csv.cpp Escaped Comma
     *
     */
    CSV_INLINE CSVCollection operator ""_csv(const char* in, size_t n) {
        return parse(csv::string_view(in, n));
    }

    /** Return a CSV's column names
     *
     *  @param[in] filename  Path to CSV file
     *  @param[in] format    Format of the CSV file
     *
     */
    CSV_INLINE std::vector<std::string> get_col_names(const std::string& filename, CSVFormat format) {
        CSVReader reader(filename, format);
        return reader.get_col_names();
    }

    /**
     *  Find the position of a column in a CSV file or CSV_NOT_FOUND otherwise
     *
     *  @param[in] filename  Path to CSV file
     *  @param[in] col_name  Column whose position we should resolve
     *  @param[in] format    Format of the CSV file
     */
    CSV_INLINE int get_col_pos(
        const std::string filename,
        const std::string col_name,
        const CSVFormat format) {
        CSVReader reader(filename, format);
        return reader.index_of(col_name);
    }

    /** Get basic information about a CSV file
     *  @include programs/csv_info.cpp
     */
    CSV_INLINE CSVFileInfo get_file_info(const std::string& filename) {
        CSVReader reader(filename);
        CSVFormat format = reader.get_format();
        for (auto& row : reader) {
            #ifndef NDEBUG
            SUPPRESS_UNUSED_WARNING(row);
            #endif
        }

        CSVFileInfo info = {
            filename,
            reader.get_col_names(),
            format.get_delim(),
            reader.correct_rows,
            (int)reader.get_col_names().size()
        };

        return info;
    }
}