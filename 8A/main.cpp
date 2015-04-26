/*************************************************************************
    > File Name: main.cpp
    > Author: Guo Hengkai
    > Description: Main program of IPC multi-thread csv handler for homework 8A
    > Created Time: Sat 25 Apr 2015 09:03:41 PM CST
 ************************************************************************/
#include <cstdio>
#include <cstring>
#include <string>
#include "csv_ipc_map_reducer.h"
#include "csv_mmap_handler.h"

using std::string;
using ghk::CsvIpcMapReducer;
using ghk::CsvMmapHandler;

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("usage: csv_ipc <in_binary_file>\n");
        printf("               <in_csv_file> <out_binary_file> [split_num]\n");
        return -1;
    }

    string input_name(argv[1]);
    if (argc == 2)
    {
        CsvMmapHandler csv_handler(input_name);
        csv_handler.Print();
    }
    else
    {
        string output_name(argv[2]);
        int split_num = 3;
        if (argc >= 4)
        {
            split_num = atoi(argv[3]);
        }
        CsvIpcMapReducer csv_ipc(split_num);
        if (csv_ipc.MapReduce(input_name, output_name))
        {
            printf("Success to save binary file.\n");
        }
        else
        {
            printf("Fail to save binary file.\n");
        }
    }

    return 0;
}
