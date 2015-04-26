/*************************************************************************
    > File Name: csv_ipc_map_reducer.cpp
    > Author: Guo Hengkai
    > Description: CSV Map Reducer class implementation using IPC for homework 8A
    > Created Time: Sat 25 Apr 2015 11:35:47 PM CST
 ************************************************************************/
#include "csv_ipc_map_reducer.h"
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string>
#include <vector>
#include "../6A/csv_handler.h"

using std::string;
using std::vector;

namespace ghk
{
const int MAXLINE = 65536;
/*
const char BIN_HEAD[] = "BINARY";
const int BIN_HEAD_LEN = 6;
const char BIN_PARSER = 255;

void WriteSizet(size_t size, FILE *file)
{
    fputc(static_cast<char>(size / 256), file);
    fputc(static_cast<char>(size % 256), file);
}
*/
size_t CsvIpcMapReducer::CsvLine2Bin(const string &line, string *bin_line)
{
    bool is_escape = false;
    bool wait_quote = false;
    bool is_empty = true;
    string current_str;
    size_t col = 0;

    for (char ch : line)
    {
        if (ch == '"')
        {
            if (is_empty)
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
            is_empty = false;
        }
        else
        {
            if (ch == ',' || ch == '\n')
            {
                if (wait_quote && !is_escape)
                {
                    *bin_line = "";
                    return 0;
                }
                else if (is_escape && wait_quote)
                {
                    is_escape = false;
                    wait_quote = false;
                }
                else if (is_escape && !wait_quote)
                {
                    current_str.append(1, ch);
                    continue;
                }

                string size_str(2, '0');
                size_str[0] = static_cast<char>(current_str.size() / 256);
                size_str[1] = static_cast<char>(current_str.size() % 256);
                *bin_line += size_str + current_str + static_cast<char>(BIN_PARSER);
                ++col;
                current_str = "";
                if (ch == '\n')
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
    return col;
}

void CsvIpcMapReducer::Map(int fd[2], int child_id, int out_fds)
{
    close(fd[1]);
    int n;
    char line[MAXLINE];
    char size_buf[4];
    while (true)
    {
        n = read(fd[0], size_buf, 4);
        if (n <= 0)
            break;
        n = static_cast<int>(size_buf[2]) * 256 + size_buf[3];
        if (n == 0)
            break;
        n = read(fd[0], line, n);
        line[n] = '\0';
        printf("Child %d: %s\n", child_id, line);
        // memcpy(size_buf + 4, line, n);
        // write(out_fds, size_buf, n + 4);
        
        string bin_line;
        auto col = CsvLine2Bin(string(line) + '\n', &bin_line);
        line[0] = col / 256;
        line[1] = col % 256;
        line[2] = size_buf[0];
        line[3] = size_buf[1];
        line[4] = static_cast<char>(bin_line.size() / 256);
        line[5] = static_cast<char>(bin_line.size() % 256);
        memcpy(line + 6, bin_line.c_str(), bin_line.size());
        write(out_fds, line, bin_line.size() + 6);
    }
    printf("Child %d: end.\n", child_id);
}

void CsvIpcMapReducer::Reduce(FILE *out_file, int in_fds)
{
    int n;
    size_t m;
    char size_buf[4];
    vector<char*> output;
    vector<int> length;
    char line_buf[1000][MAXLINE];
    size_t col = 0;
    int l = 0;

    while (true)
    {
        n = read(in_fds, size_buf, 6);
        if (n <= 0)
            break;
        size_t tmp_col = static_cast<size_t>(size_buf[0]) * 256 + size_buf[1];
        n = static_cast<int>(size_buf[4]) * 256 + size_buf[5];
        m = static_cast<size_t>(size_buf[2]) * 256 + size_buf[3];
        if (n <= 0)
            break;
        if (col > 0 && col != tmp_col)  // Column nunber error
        {
            printf("Column number error.\n");
            break;
        }
        col = tmp_col;

        n = read(in_fds, line_buf[l], n);
        if (n < 0)
            break;
        printf("Reduce: number %zu, length %d\n", m, n);
        if (m >= output.size())
        {
            output.resize(m + 1);
            length.resize(m + 1);
        }
        output[m] = line_buf[l];
        length[m] = n;
        ++l;
    }
    printf("Reduce end.\n");

    fwrite(BIN_HEAD, sizeof(char), BIN_HEAD_LEN, out_file);
    WriteSizet(output.size(), out_file);
    WriteSizet(col, out_file);

    for (size_t i = 0; i < output.size(); ++i)
    {
        // printf("%s\n", output[i].c_str());
        fwrite(output[i], sizeof(char), length[i], out_file);
    }
}

bool CsvIpcMapReducer::GetCsvLine(FILE *file, string *line)
{
    *line = "";
    bool is_escape = false;
    bool wait_quote = false;
    bool is_empty = true;
    char ch;

    while (true)
    {
        ch = fgetc(file);
        if (ch == EOF)
        {
            break;
        }
        line->append(1, ch);

        if (ch == '"')
        {
            if (is_empty)
            {
                is_escape = true;
            }
            else
            {
                if (wait_quote)
                {
                    wait_quote = false;
                }
                else
                {
                    wait_quote = true;
                }
            }
            is_empty = false;
        }
        else
        {
            if (ch == ',' || ch == '\n' || ch == EOF)
            {
                if (wait_quote && !is_escape)
                {
                    printf("Error when parsing the csv file"
                           " with wrong quote.\n");
                    *line = "";
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
                        *line = "";
                        return false;
                    }
                    is_empty = false;
                    continue;
                }

                is_empty = true;
                if (ch == '\n' || ch == EOF)
                {
                    break;
                }
            }
        }
    }

    return !line->empty();
}

bool CsvIpcMapReducer::MapReduce(const string &input_name,
                                 const string &output_name)
{
    // Set up pipes and fork child processes
    int pipes[split_num()][2];
    vector<pid_t> pid_list(split_num());
    pid_t pid_reduce;
    bool flag = true;
    
    const char FIFO[5] = "FIFO";
    if (mkfifo(FIFO, 0754) < 0 && errno != EEXIST)
    {
        printf("Error on create FIFO.\n");
        return false;
    }

    for (int i = 0; i < split_num(); ++i)
    {
        if (pipe(pipes[i]) < 0)
        {
            printf("Error on creating %dth pipe.\n", i);
            flag = false;
            break;
        }

        if ((pid_list[i] = fork()) < 0)
        {
            printf("Error on forking %dth map process.\n", i);
            flag = false;
            break;
        }

        if (pid_list[i] == 0)  // Child for map
        {
            int out_fd = open(FIFO, O_WRONLY);
            Map(pipes[i], i, out_fd);
            close(out_fd);
            exit(0);
        }
    }

    if ((pid_reduce = fork()) < 0)
    {
        printf("Error on forking reduce process.\n");
        flag = false;
    }
    else
    {
        if (pid_reduce == 0)  // Child for reduce
        {
            FILE *out_file;
            if ((out_file = fopen(output_name.c_str(), "wb")) == nullptr)
            {
                printf("Fail to create file %s.\n", output_name.c_str());
                exit(-1);
            }

            int in_fd = open(FIFO, O_RDONLY);
            Reduce(out_file, in_fd);
            fclose(out_file);
            close(in_fd);
            exit(0);
        }
    }
   
    if (flag)
    {
        for (int i = 0; i < split_num(); ++i)
        {
            close(pipes[i][0]);
        }

        // Read the csv file and send the lines to child processes
        FILE *in_file;
        if ((in_file = fopen(input_name.c_str(), "rt")) == nullptr)
        {
            printf("Fail to read file %s.\n", input_name.c_str());
            flag = false;
        }
        else
        {
            int n = 0;
            string line;
            while (GetCsvLine(in_file, &line))
            {
                size_t line_size = line.size() - 1;
                char size_buf[line_size + 3];
                size_buf[0] = static_cast<char>(n / 256);
                size_buf[1] = static_cast<char>(n % 256);
                size_buf[2] = static_cast<char>(line_size / 256);
                size_buf[3] = static_cast<char>(line_size % 256);
                memcpy(size_buf + 4, line.c_str(), line.size() - 1);
                write(pipes[n % split_num()][1], size_buf, line.size() + 3);
                ++n;
            }
            fclose(in_file);
        }
    }

    for (int i = 0; i < split_num(); ++i)
    {
        char size_buf[] = {0, 0, 0, 0};
        write(pipes[i][1], size_buf, 4);
        close(pipes[i][1]);
    }

    for (int i = 0; i < split_num(); ++i)
    {
        waitpid(pid_list[i], NULL, 0);
    }
    unlink(FIFO);
    waitpid(pid_reduce, NULL, 0);
    return flag;
}
}  // namespace ghk
