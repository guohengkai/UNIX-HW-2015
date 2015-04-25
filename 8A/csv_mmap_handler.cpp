/*************************************************************************
    > File Name: csv_mmap_handler.cpp
    > Author: Guo Hengkai
    > Description: CSV file hanlder class implementation with mmap for homework 8A
    > Created Time: Sat 25 Apr 2015 09:22:27 PM CST
 ************************************************************************/
#include "csv_mmap_handler.h"
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace ghk
{
CsvMmapHandler::CsvMmapHandler(const string &file_name)
{
    int filedes;
    if ((filedes = open(file_name.c_str(), O_RDONLY)) < 0)
    {
        printf("Error to open file %s.\n", file_name.c_str());
        return;
    }
    LoadBin(filedes);
    close(filedes);
}

bool CsvMmapHandler::SaveTxt(FILE *file, int begin, int end) const
{
    vector<size_t> col_len;
    for (size_t i = begin; i < data_[begin].size(); ++i)
    {
        col_len.push_back(data_[begin][i].size());
    }
    
    for (int i = begin + 1; i < end; ++i)
        for (size_t j = 0; j < data_[i].size(); ++j)
        {
            col_len[j] = std::max(col_len[j], data_[i][j].size());
        }

    for (size_t i = 0; i < col_len.size(); ++i)
    {
        fprintf(file, " ");
        for (size_t j = 0; j < col_len[i]; ++j)
        {
            fprintf(file, "-");
        }
    }
    fprintf(file, "\n");

    for (int i = 0; i < end; ++i)
    {
        for (size_t j = 0; j < data_[i].size(); ++j)
        {
            string temp = data_[i][j];
            size_t pos;
            while ((pos = temp.find("\n")) != string::npos)
            {
                // Replace \n with '_' for simple print
                temp.replace(pos, 1, "_");  
            }
            fprintf(file, "|%s", temp.c_str());
            auto diff = col_len[j] - data_[i][j].size();
            for (size_t k = 0; k < diff; ++k)
            {
                fprintf(file, " ");
            }
        }
        fprintf(file, "|\n");

        for (size_t j = 0; j < col_len.size(); ++j)
        {
            fprintf(file, " ");
            for (size_t k = 0; k < col_len[j]; ++k)
            {
                fprintf(file, "-");
            }
        }
        fprintf(file, "\n");
    }
    return true;
}

bool CsvMmapHandler::Print() const
{
    return SaveTxt(stdout, 0, static_cast<int>(data_.size()));
}

size_t ReadSizet(char *&buf)
{
    size_t result = 0;
    for (int i = 0; i < 2; ++i)
    {
        int temp_byte = *buf;
        result = result * 256 + temp_byte;
        ++buf;
    }
    return result;
}

bool CsvMmapHandler::LoadBin(int filedes)
{
    struct stat stat_buf;
    if (fstat(filedes, &stat_buf) < 0)
    {
        printf("fstat error.\n");
        return false;
    }

    void *src;
    if ((src = mmap(0, stat_buf.st_size, PROT_READ, MAP_SHARED, filedes, 0))
            == MAP_FAILED)
    {
        printf("mmap error for input.\n");
        return false;
    }

    char *bin_buf = static_cast<char*>(src);
    bin_buf += 6;  // Skip the identifier

    size_t rows = ReadSizet(bin_buf);
    size_t cols = ReadSizet(bin_buf);
    for (size_t i = 0; i < rows; ++i)
    {
        vector<string> row_data;
        for (size_t j = 0; j < cols; ++j)
        {
            size_t len = ReadSizet(bin_buf);
            char temp[65536];
            memcpy(temp, bin_buf, len);
            temp[len] = '\0';
            string data(temp);
            row_data.push_back(data);

            bin_buf += len + 1;
        }
        data_.push_back(row_data);
    }

    return true;
}
}  // namespace ghk
