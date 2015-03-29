/*************************************************************************
    > File Name: main.cpp
    > Author: Guo Hengkai
    > Description: Main program of csv converter and printer for homework 6A 
    > Created Time: Sat 28 Mar 2015 04:29:19 PM CST
 ************************************************************************/
#include <cstdio>
#include <string>
#include "csv_handler.h"

using std::string;
using ghk::CsvHandler;
using ghk::FileType;

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("usage: csv_handler <input_file> [<output_file>]\n");
        return -1;
    }

    string input_name(argv[1]);
    CsvHandler csv_handler(input_name);
    if (csv_handler.has_data())
    {
        if (argc == 2)
        {
            csv_handler.Print();
        }
        else
        {
            csv_handler.Print();
            string output_name(argv[2]);
            if (!csv_handler.Save(output_name))
            {
                printf("Fail to save binary file.\n");
            }
        }
    }
    else
    {
        printf("Fail to load csv data.\n");
    }

    return 0;
}
