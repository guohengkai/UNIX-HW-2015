/*************************************************************************
    > File Name: main.cpp
    > Author: Guo Hengkai
    > Description: Main program of multi-thread csv handler for homework 7B
    > Created Time: Fri 03 Apr 2015 03:55:08 PM CST
 ************************************************************************/
#include <cstdio>
#include <cstring>
#include <string>
#include "csv_map_reducer.h"

using std::string;
using ghk::CsvMapReducer;

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("usage: csv_map_reduce <in_csv_file> <out_binary_file>"
               "[split_num]\n");
        return -1;
    }

    string input_name(argv[1]);
    string output_name(argv[2]);
    int split_num = 3;
    if (argc >= 4)
    {
        split_num = atoi(argv[3]);
    }

    CsvMapReducer csv_mr(split_num);
    if (csv_mr.MapReduce(input_name, output_name))
    {
        printf("Success to save binary file.\n");
    }
    else
    {
        printf("Fail to save binary file.\n");
    }

    return 0;
}
