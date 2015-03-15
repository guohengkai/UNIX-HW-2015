/*************************************************************************
    > File Name: main.cpp
    > Author: Guo Hengkai
    > Description: Main program of word check for homework 3A
    > Created Time: Fri 13 Mar 2015 11:09:30 PM CST
 ************************************************************************/
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::string;

int main(int argc, char **argv)
{
    int n = 25;
    if (argc > 1)
    {
        n = atoi(argv[1]);
    }

    map<string, int> word_counter;
    for (int i = 0; i < n; ++i)
    {
        string word;
        cin >> word;
        if (word_counter.count(word) == 0)
        {
            word_counter[word] = 1;
        }
        else
        {
            ++word_counter[word];
        }
    }

    for (auto iter = word_counter.begin();
            iter != word_counter.end(); ++iter)
    {
        if (iter->second > 1)
        {
            cout << iter->first << ": " << iter->second << endl;
        }
    }

    return 0;
}
