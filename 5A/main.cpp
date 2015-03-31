/*************************************************************************
    > File Name: main.cpp
    > Author: Guo Hengkai
    > Description: Main program of freport for homework 5A
    > Created Time: Mon 30 Mar 2015 08:18:14 PM CST
 ************************************************************************/
#include <cstdio>
#include <cctype>
#include <getopt.h>
#include <string>

using std::string;

void PrintUsage(FILE *stream, char *program_name)
{
    fprintf(stream, "Usage: %s [options] dir\n", program_name);
    // fprintf(stream,);
}

void PrintFileTree(string &dir_name, int file_length, int access_day)
{
    
}

int main(int argc, char **argv)
{
    const char* const short_opts = "hl:a:";
    const struct option long_opts[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"longer-than", required_argument, NULL, 'l'},
        {"access-within", required_argument, NULL, 'a'},
        {NULL, 0, NULL, 0}
    };

    int opt;
    int file_length = 0;
    int access_day = 0;
    while ((opt = getopt_long(argc, argv,
                    short_opts, long_opts, NULL)) != -1)
    {
        switch (opt)
        {
            case 'h':
                PrintUsage(stdout, argv[0]);
                return 0;
            case 'l':
                file_length = atoi(optarg);
                break;
            case 'a':
                access_day = atoi(optarg);
                break;
            case '?':
                if (optopt == 'l' || optopt == 'a')
                {
                    printf("Error: option -%c requires an argument\n", optopt);
                }
                else if (isprint(optopt))
                {
                    printf("Error: unknown option '-%c'\n", optopt);
                }
                else
                {
                    printf("Error: unknown option character '\\x%x'\n",
                            optopt);
                }
                return 1;
            default:
                printf("Error: unknown error\n");
                return 1;
        }
    }

    if (optind > argc - 1)
    {
        printf("Error: require a directory name\n");
        return 1;
    }

    string dir_name(argv[optind]);
    PrintFileTree(dir_name, file_length, access_day);
    return 0;
}

