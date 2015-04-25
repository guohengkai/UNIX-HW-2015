/*************************************************************************
    > File Name: csv_map_reducer.cpp
    > Author: Guo Hengkai
    > Description: CSV Map Reducer class implementation for homework 7B
    > Created Time: Sun 05 Apr 2015 05:09:21 PM CST
 ************************************************************************/
#include "csv_map_reducer.h"
#include <cstdio>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <sstream>
#include <vector>
#include "../6A/csv_handler.h"

using std::string;
using std::stringstream;
using std::vector;

namespace ghk
{
bool CsvMapReducer::MapReduce(const string &input_name,
                              const string &output_name)
{
    // Read the csv file
    
    if (!handler_.Load(input_name))
    {
        printf("Fail to load the csv file.\n");
        return false;
    }

    // Fork child processes to convert csv into small binary files
    auto pos = output_name.find_last_of(".");
    string file_name_noext = output_name.substr(0, pos);
    vector<pid_t> pid_list;
    int step = (handler_.row_count() + split_num() - 1) / split_num();
    int current = 0;
    for (int i = 0; i < split_num(); ++i)
    {
        pid_t pid;
        if ((pid = fork()) < 0)
        {
            printf("Fork error with process %d.\n", i);
            return false;
        }
        else if (pid == 0)  // Child
        {
            stringstream stream;
            stream << i;
            if (!handler_.Save(file_name_noext + "_" + stream.str() + ".bin",
                        current, current + step))
            {
                printf("Fail to save the No.%d binary file.\n", i);
                exit(-1);
            }
            printf("Finish No.%d process.\n", i);
            exit(0);
        }
        else  // parent
        {
            pid_list.push_back(pid);
            current += step;
        }
    }
    
    // Merge small binary files into the result binary file
    FILE *out_file;
    if ((out_file = fopen(output_name.c_str(), "wb")) == nullptr)
    {
        printf("Fail to write file %s.\n", output_name.c_str());
        return false;
    }

    fwrite(BIN_HEAD, sizeof(char), BIN_HEAD_LEN, out_file);
    WriteSizet(handler_.row_count(), out_file);
    WriteSizet(handler_.col_count(), out_file);

    for (size_t i = 0; i < pid_list.size(); ++i)
    {
        if (waitpid(pid_list[i], NULL, 0) != pid_list[i])
        {
            printf("Waitpid error in process %zu.\n", i);
            return false;
        }

        stringstream stream;
        stream << i;
        FILE *in_file;
        if ((in_file = fopen((file_name_noext + "_" + stream.str() + ".bin")
                        .c_str(), "rb")) == nullptr)
        {
            printf("Fail to read No.%zu file.\n", i);
            return false;
        }

        if (fseek(in_file, BIN_HEAD_LEN + 4, SEEK_SET) != 0)
        {
            printf("Fail to read the file header.\n");
            return false;
        }

        char buff[1024];
        size_t read_size;
        while ((read_size = fread(buff, sizeof(char), 1024, in_file)) != 0)
        {
            fwrite(buff, sizeof(char), read_size, out_file);
        }

        fclose(in_file);
    }

    fclose(out_file);
    return true;
}

}  // namespace ghk
