/*************************************************************************
    > File Name: csv_handler.cpp
    > Author: Guo Hengkai
    > Description: CSV file handler class implementation for homework 6A
    > Created Time: Sat 28 Mar 2015 05:13:31 PM CST
 ************************************************************************/
#include "csv_handler.h"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace ghk
{
CsvHandler::CsvHandler(string &file_name)
{
    Load(file_name);
}

bool CsvHandler::Save(string &file_name) const
{
    if (!has_data())
    {
        printf("No data for saving.\n");
        return false;
    }

    FileType type = GetFileTypeFromName(file_name);
    string mode;
    if (type == FileType::BIN)
    {
        mode = "wb";
    }
    else
    {
        mode = "wt";
    }

    FILE *out_file;
    if ((out_file = fopen(file_name.c_str(), mode.c_str())) == nullptr)
    {
        printf("Fail to write file %s.\n", file_name.c_str());
        return false;
    }

    bool flag = Save(out_file, type);
    fclose(out_file);
    return flag;
}

bool CsvHandler::Load(string &file_name)
{
    FileType type = GetFileTypeFromName(file_name);
    string mode;
    if (type == FileType::BIN)
    {
        mode = "rb";
    }
    else
    {
        mode = "rt";
    }

    FILE *in_file;
    if ((in_file = fopen(file_name.c_str(), mode.c_str())) == nullptr)
    {
        printf("Fail to read file %s.\n", file_name.c_str());
        return false;
    }

    bool flag = Load(in_file, type);
    fclose(in_file);
    return flag;
}

bool CsvHandler::Print() const
{
    return Save(stdout, FileType::TXT);
}

void WriteSizet(size_t size, FILE *file)
{
    fputc(static_cast<char>(size / 256), file);
    fputc(static_cast<char>(size % 256), file);
}

bool CsvHandler::Save(FILE *file, FileType type) const
{
    if (data_.empty())
    {
        printf("No data to be saved.\n");
        return false;
    }

    if (type == FileType::NONE)
    {
        printf("The file type does not support saving.\n");
        return false;
    }

    if (type == FileType::CSV)
    {
        for (size_t i = 0; i < data_.size(); ++i)
        {
            for (size_t j = 0; j < data_[i].size(); ++j)
            {
                string temp = data_[i][j];
                bool need_quote = false;
                size_t k = 0;
                while (k < temp.size())
                {
                    if (temp[k] == '\n' || temp[k] == ',')
                    {
                        need_quote = true;
                    }
                    else if (temp[k] == '"')
                    {
                        if (k == 0 || k + 1 == temp.size())
                        {
                            need_quote = true;
                        }
                        temp.insert(k, 1, '"');
                        ++k;
                    }
                    ++k;
                }

                if (need_quote)
                {
                    temp.insert(0, 1, '"');
                    temp.append(1, '"');
                }
                fprintf(file, "%s", temp.c_str());
                if (j + 1 < data_[i].size())
                {
                    fprintf(file, ",");
                }
                else
                {
                    fprintf(file, "\n");
                }
            }
        }
    }
    else if (type == FileType::BIN)
    {
        // Write the identifier of binary file
        fwrite(BIN_HEAD, sizeof(char), BIN_HEAD_LEN, file);

        // Write number of row and column (Max: 256 * 256 - 1)
        WriteSizet(data_.size(), file);
        WriteSizet(data_[0].size(), file);

        // Write table data
        for (size_t i = 0; i < data_.size(); ++i)
            for (size_t j = 0; j < data_[i].size(); ++j)
            {
                WriteSizet(data_[i][j].size(), file);  // Length
                fwrite(data_[i][j].c_str(), sizeof(char),
                       data_[i][j].size(), file);  // Data
                fputc(BIN_PARSER, file);  // Parser
            }
    }
    else  // FileType::TXT
    {
        vector<size_t> col_len;
        for (size_t i = 0; i < data_[0].size(); ++i)
        {
            col_len.push_back(data_[0][i].size());
        }
        
        for (size_t i = 1; i < data_.size(); ++i)
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

        for (size_t i = 0; i < data_.size(); ++i)
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
    }

    return true;
}

size_t ReadSizet(FILE *file)
{
    size_t result = 0;
    for (int i = 0; i < 2; ++i)
    {
        int temp_byte = fgetc(file);
        result = result * 256 + temp_byte;
    }
    return result;
}

bool CsvHandler::Load(FILE *file, FileType type)
{
    if (type == FileType::TXT || type == FileType::NONE)
    {
        printf("The file type does not support loading.\n");
        return false;
    }
    
    has_data_ = false;
    data_.clear();
    if (type == FileType::CSV)
    {
        bool is_escape = false;
        bool wait_quote = false;
        vector<string> current_row;
        string current_str;
        char ch;
        while (true)
        {
            ch = fgetc(file);
            if (ch == '"')
            {
                if (current_str.empty())
                {
                    is_escape = true;
                }
                else
                {
                    if (wait_quote)
                    {
                        current_str.append(1, '"');
                        wait_quote = false;
                    }
                    else
                    {
                        wait_quote = true;
                    }
                }
            }
            else
            {
                if (ch == ',' || ch == '\n' || ch == EOF)
                {
                    if (wait_quote && !is_escape)
                    {
                        printf("Error when parsing the csv file"
                               " with wrong quote.\n");
                        data_.clear();
                        return false;
                    }
                    else if (is_escape && wait_quote)  // end of string
                    {
                        is_escape = false;
                        wait_quote = false;
                    }
                    else if (is_escape && !wait_quote)  // escape or die
                    {
                        if (ch == EOF)
                        {
                            printf("Error when parsing the csv file"
                                   " with wrong quote before EOF.\n");
                            data_.clear();
                            return false;
                        }
                        current_str.append(1, ch);
                        continue;
                    }

                    current_row.push_back(current_str);
                    current_str = "";
                    if (ch == '\n')
                    {
                        // Check the number of column
                        if (!data_.empty() && data_[0].size() != current_row.size())
                        {
                            printf("Error when parsing the csv file"
                                   " with wrong column at row %zu.\n",
                                   data_.size() + 1);
                            data_.clear();
                            return false;
                        }
                        data_.push_back(current_row);
                        current_row.clear();
                    }
                    else if (ch == EOF)
                    {
                        break;
                    }

                }
                else
                {
                    current_str.append(1, ch);
                }
            }
        }
    }
    else  // FileType::BIN
    {
        // Read the identifier of binary file
        char head[BIN_HEAD_LEN + 1];
        fread(head, sizeof(char), BIN_HEAD_LEN, file);
        if (strcmp(head, BIN_HEAD) != 0)
        {
            printf("Error header in binary file format: %s.\n", head);
            return false;
        }
        
        // Read number of rows and columns
        size_t rows = ReadSizet(file);
        size_t cols = ReadSizet(file);
        for (size_t i = 0; i < rows; ++i)
        {
            vector<string> row_data;
            for (size_t j = 0; j < cols; ++j)
            {
                size_t len = ReadSizet(file);
                char temp[65536];
                fread(temp, sizeof(char), len, file);
                temp[len] = '\0';
                string data(temp);
                row_data.push_back(data);

                int parser = fgetc(file);
                if (parser != BIN_PARSER)
                {
                    printf("Error parser in binary file format"
                           " in (%zu, %zu): %d.\n", i, j, parser);
                    data_.clear();
                    return false;
                }
            }
            data_.push_back(row_data);
        }
    }

    has_data_ = true;
    return true;
}

FileType CsvHandler::GetFileTypeFromName(string &file_name) const
{
    auto pos = file_name.find_last_of(".");
    if (pos != string::npos)
    {
        string ext_name = file_name.substr(pos + 1);
        if (ext_name == "csv")
        {
            return FileType::CSV;
        }
        else if (ext_name == "bin")
        {
            return FileType::BIN;
        }
        else if (ext_name == "txt")
        {
            return FileType::TXT;
        }
    }

    return FileType::NONE;
}

}  // namespace ghk
