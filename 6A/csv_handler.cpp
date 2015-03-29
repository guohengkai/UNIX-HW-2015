/*************************************************************************
    > File Name: csv_handler.cpp
    > Author: Guo Hengkai
    > Description: CSV file handler class implementation for homework 6A
    > Created Time: Sat 28 Mar 2015 05:13:31 PM CST
 ************************************************************************/
#include "csv_handler.h"
#include <cstdio>
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

bool CsvHandler::Save(FILE *file, FileType type) const
{
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
        bool quote = false;
        bool true_quote = false;
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
                    quote = true;
                }
                else
                {
                    if (true_quote)
                    {
                        current_str.append(1, '"');
                        true_quote = false;
                    }
                    else
                    {
                        true_quote = true;
                    }
                }
            }
            else
            {
                if (ch == ',' || ch == '\n' || ch == EOF)
                {
                    if (true_quote && !quote)
                    {
                        printf("Error when parsing the csv file"
                               " with wrong quote.\n");
                        data_.clear();
                        return false;
                    }
                    else if (quote && true_quote)  // end of string
                    {
                        quote = false;
                        true_quote = false;
                    }
                    else if (quote && !true_quote)  // escape or die
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
