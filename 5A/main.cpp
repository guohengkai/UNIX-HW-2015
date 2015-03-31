/*************************************************************************
    > File Name: main.cpp
    > Author: Guo Hengkai
    > Description: Main program of freport for homework 5A
    > Created Time: Mon 30 Mar 2015 08:18:14 PM CST
 ************************************************************************/
#include <cstdio>
#include <cctype>
#include <ftw.h>
#include <getopt.h>
#include <string>

using std::string;

void PrintUsage(FILE *stream, char *program_name)
{
    fprintf(stream, "Usage: %s [options] dir_name\n", program_name);
    fprintf(stream,
            "   -h --help         Print help information\n"
            "   -l --longer-than  Print files "
            "longer than specified length(byte)\n"
            "   -a --access-day   Print files that "
            "accessed in specified days\n");
}

void GetTimeString(const time_t &time, string *time_str)
{
    char str_buffer[50];
    strftime(str_buffer, sizeof(str_buffer),
            "%F %T", localtime(&time));
    *time_str = string(str_buffer);
}

int DisplayInfo(const char *file_path, const struct stat *stat_buff,
       int type_flag, struct FTW *ftw_buf)
{
    string type;
    switch (type_flag)
    {
        case FTW_F:
            type = "File";  // Regular file
            break;
        case FTW_D:
            type = "Dir";  // Directory
            break;
        case FTW_SL:
            type = "Link";  // Symbolic link
            break;
        default:
            type = "";  // Unknown type
    }

    string atime_str;
    GetTimeString(stat_buff->st_atime, &atime_str);
    string mtime_str;
    GetTimeString(stat_buff->st_mtime, &mtime_str);

    printf("%-40s %-5s %-5d %-8lld %-20s %-20s\n", file_path, type.c_str(),
            ftw_buf->level, static_cast<long long>(stat_buff->st_size),
            atime_str.c_str(), mtime_str.c_str());
    return 0;
}

int PrintFileTree(string &dir_name, int file_length, int access_day)
{
    printf("%-40s %-5s %-5s %-8s %-20s %-20s\n", "File Name", "Type",
            "Depth", "Size", "Last Access Time", "Last Modified Time");
    printf("---------------------------------------------------"
           "---------------------------------------------------\n");
    return nftw(dir_name.c_str(), DisplayInfo, 500, FTW_PHYS);
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
        printf("Error: require a directory name (-h for help)\n");
        return 1;
    }

    string dir_name(argv[optind]);
    return PrintFileTree(dir_name, file_length, access_day);
}

