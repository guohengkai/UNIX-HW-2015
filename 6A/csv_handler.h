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
    TXT,
    NONE
};

const int MAX_BUFFER = 2048;
const char BIN_HEAD[] = "BINARY";
const int BIN_HEAD_LEN = 6;
const int BIN_PARSER = 255;

class CsvHandler
{
public:
    CsvHandler(): has_data_(false) {}
    explicit CsvHandler(std::string &file_name);
    
    bool Save(std::string &file_name) const;
    bool Load(std::string &file_name);
    bool Print() const;

    inline bool has_data() const { return has_data_; }

protected:
    virtual bool SaveCsv(FILE *file) const;
    virtual bool SaveBin(FILE *file) const;
    virtual bool SaveTxt(FILE *file) const;

    virtual bool LoadCsv(FILE *file);
    virtual bool LoadBin(FILE *file);

    bool has_data_;
    std::vector<std::vector<std::string>> data_;

private:
    bool Save(FILE *file, FileType type) const;
    bool Load(FILE *file, FileType type);

    FileType GetFileTypeFromName(std::string &file_name) const;
};
}  // namespace ghk

#endif  // CSV_HANDLER_H_
