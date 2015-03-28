/*************************************************************************
    > File Name: csv_handler.h
    > Author: Guo Hengkai
    > Description: CSV file handler class definition for homework 6A
    > Created Time: Sat 28 Mar 2015 04:56:32 PM CST
 ************************************************************************/
#ifndef CSV_HANDLER_H_
#define CSV_HANDLER_H_

#include <string>
#include <vector>

namespace ghk
{
enum FileType
{
    CSV = 0,
    BIN,
    TXT
};

class CsvHandler
{
public:
    CsvHandler(): has_data_(false) {}
    explicit CsvHandler(std::string &file_name);
    
    bool Save(std::string &file_name) const;
    bool Load(std::string &file_name);
    bool Print() const;

    inline bool has_data() const { return has_data_; }

private:
    bool Save(FILE *file, FileType type) const;
    bool Load(FILE *file, FileType type);

    FileType GetFileTypeFromName(std::string &file_name) const;

    bool has_data_;
    std::vector<std::vector<std::string>> data_;
};
}  // namespace ghk

#endif  // CSV_HANDLER_H_
