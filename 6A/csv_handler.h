/*************************************************************************
    > File Name: csv_handler.h
    > Author: Guo Hengkai
    > Description: CSV file handler class definition for homework 6A
    > Created Time: Sat 28 Mar 2015 04:56:32 PM CST
 ************************************************************************/
#ifndef SIX_A_CSV_HANDLER_H_
#define SIX_A_CSV_HANDLER_H_

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
    explicit CsvHandler(const std::string &file_name);
    
    bool Save(const std::string &file_name, int begin = -1, int end = -1) const;
    bool Load(const std::string &file_name);
    bool Print() const;

    inline bool has_data() const { return has_data_; }
    inline size_t row_count() const { return data_.size(); }
    inline size_t col_count() const
    {
        if (has_data())
        {
            return data_[0].size();
        }
        else
        {
            return 0;
        }
    }

protected:
    virtual bool SaveCsv(FILE *file, int begin, int end) const;
    virtual bool SaveBin(FILE *file, int begin, int end) const;
    virtual bool SaveTxt(FILE *file, int begin, int end) const;

    virtual bool LoadCsv(FILE *file);
    virtual bool LoadBin(FILE *file);

    bool has_data_;
    std::vector<std::vector<std::string>> data_;

private:
    bool Save(FILE *file, FileType type, int begin, int end) const;
    bool Load(FILE *file, FileType type);

    FileType GetFileTypeFromName(const std::string &file_name) const;
};

void WriteSizet(std::size_t size, FILE *file);
}  // namespace ghk

#endif  // SIX_A_CSV_HANDLER_H_
