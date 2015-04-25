/*************************************************************************
    > File Name: csv_mmap_handler.h
    > Author: Guo Hengkai
    > Description: CSV file handler class definition with mmap for homework 8A
    > Created Time: Sat 25 Apr 2015 09:18:29 PM CST
 ************************************************************************/
#ifndef EIGHT_A_CSV_MMAP_HANDLER_H_
#define EIGHT_A_CSV_MMAP_HANDLER_H_

#include <string>
#include <vector>

namespace ghk
{
class CsvMmapHandler
{
public:
    explicit CsvMmapHandler(const std::string &file_name);
    bool Print() const;

private:
    bool LoadBin(int filedes);
    bool SaveTxt(FILE *file, int begin, int end) const;
    std::vector<std::vector<std::string>> data_;
};
}  // namespace ghk

#endif  // EIGHT_A_CSV_MMAP_HANDLER_H_

