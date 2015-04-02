/*************************************************************************
    > File Name: main.cpp
    > Author: Guo Hengkai
    > Description: Main program of freport for homework 5A
    > Created Time: Mon 30 Mar 2015 08:18:14 PM CST
 ************************************************************************/
#include <cctype>
#include <cstdio>
#include <cstring>
#include <dirent.h>
#include <getopt.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include <map>
#include <string>

using std::map;
using std::string;

const int SECS_IN_DAY = 86400;

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
       const map<char, int> &option_param, int depth)
{
    if ((option_param.count('l') > 0
            && stat_buff->st_size < option_param.at('l'))
            || (option_param.count('a') > 0
            && time(NULL) - stat_buff->st_atime
                > option_param.at('a') * SECS_IN_DAY))
    {
        // Exit when the file doesn't satisfy the condition
        return 0;
    }

    string type;
    mode_t mode = stat_buff->st_mode;
    if (S_ISREG(mode))
    {
        type = "File";  // Regular file
    }
    else if (S_ISDIR(mode))
    {
        type = "Dir";  // Directory
    }
    else if (S_ISLNK(mode))
    {
        type = "Link";  // Symbolic link
    }
    else
    {
        type = "";  // Unknown type
    }

    string atime_str;
    GetTimeString(stat_buff->st_atime, &atime_str);
    string mtime_str;
    GetTimeString(stat_buff->st_mtime, &mtime_str);

    printf("%-40s %-5s %-5d %-8lld %-20s %-20s\n", file_path, type.c_str(),
            depth, static_cast<long long>(stat_buff->st_size),
            atime_str.c_str(), mtime_str.c_str());
    return 0;
}

int MyFtw(const char *dir_path,
        int (*fn) (const char*, const struct stat*,
            const map<char, int>&, int),
        const map<char, int> &option_param, int depth)
{
    struct stat stat_buff;
    int flag = lstat(dir_path, &stat_buff);
    if (flag == -1)
    {
        perror(dir_path);
        return 1;
    }

    fn(dir_path, &stat_buff, option_param, depth);

    if (!S_ISDIR(stat_buff.st_mode))
    {
        // Not a directory, stop walking
        return 0;
    }

    DIR *dir_ptr;
    if ((dir_ptr = opendir(dir_path)) == nullptr)
    {
        // Fail to open the directory
        perror(dir_path);
        return 1;
    }

    struct dirent *entry;
    // Traversal all the entries in the directory
    while ((entry = readdir(dir_ptr)) != nullptr)
    {
        if (strcmp(entry->d_name, ".") == 0
                || strcmp(entry->d_name, "..") == 0)
        {
            continue;  // Skip "." and ".."
        }

        string sub_path = string(dir_path) + "/" + entry->d_name;
        struct stat temp_buff;
        int result = lstat(sub_path.c_str(), &temp_buff);
        if (result == -1)
        {
            perror(sub_path.c_str());
            return 1;
        }

        result = MyFtw(sub_path.c_str(), fn,
                option_param, depth + 1);
        if (result != 0)
        {
            closedir(dir_ptr);
            return result;
        }
    }
    closedir(dir_ptr);
    return 0;
}

int PrintFileTree(const char *dir_name, const map<char, int> &option_param)
{
    printf("%-40s %-5s %-5s %-8s %-20s %-20s\n", "File Name", "Type",
           "Depth", "Size", "Last Access Time", "Last Modified Time");
    printf("---------------------------------------------------"
           "---------------------------------------------------\n");

    return MyFtw(dir_name, DisplayInfo, option_param, 0);
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


    map<char, int> option_param;
    int opt;
    while ((opt = getopt_long(argc, argv,
                    short_opts, long_opts, NULL)) != -1)
    {
        switch (opt)
        {
            case 'h':
                PrintUsage(stdout, argv[0]);
                return 0;
            case 'l':
                option_param['l'] = atoi(optarg);
                break;
            case 'a':
                option_param['a'] = atoi(optarg);
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

    return PrintFileTree(argv[optind], option_param);
}

