/*************************************************************************
    > File Name: csv_ipc_map_reducer.cpp
    > Author: Guo Hengkai
    > Description: CSV Map Reducer class implementation using IPC for homework 8A
    > Created Time: Sat 25 Apr 2015 11:35:47 PM CST
 ************************************************************************/
#include "csv_ipc_map_reducer.h"
#include <cstdio>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace ghk
{
const int MAXLINE = 65536;
bool CsvIpcMapReducer::MapReduce(const string &input_name,
                                 const string &output_name)
{
    // Set up pipes and fork child processes
    int pipes[split_num()][2];
    vector<pid_t> pid_list(split_num());
    for (int i = 0; i < split_num(); ++i)
    {
        if (pipe(pipes[i]) < 0)
        {
            printf("Error on creating %dth pipe.\n", i);
            return false;
        }

        if ((pid_list[i] = fork()) < 0)
        {
            printf("Error on forking %dth process.\n", i);
            return false;
        }

        if (pid_list[i] == 0)  // Child
        {
            close(pipes[i][1]);
            char line[MAXLINE];
            int n;
            while ((n = read(pipes[i][0], line, MAXLINE)) > 0)
            {
                printf("Child %d: %s\n", i, line);
            }
            printf("Child %d: end.\n", i);
            exit(0);
        }
    }
    
    for (int i = 0; i < split_num(); ++i)
    {
        close(pipes[i][0]);
        for (int j = 0; j < 3; ++j)
        {
            char line[MAXLINE];
            sprintf(line, "hehe%d %d", i, j);
            write(pipes[i][1], line, 7);
        }
        close(pipes[i][1]);
    }

    for (int i = 0; i < split_num(); ++i)
    {
        waitpid(pid_list[i], NULL, 0);
    }

    return false;
}
}  // namespace ghk
