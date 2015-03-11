/*************************************************************************
    > File Name: main.cpp
    > Author: Guo Hengkai
    > Description: Main program of number game problem for homework 2A
    > Created Time: Wed 11 Mar 2015 09:51:10 AM CST
 ************************************************************************/
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "number_game.h"

using std::vector;
using ghk::NumberGame;

bool DepthFirstSearch(NumberGame &game)
{
    vector<int> valid_number;
    game.GetValidNumbers(&valid_number);

    if (valid_number.size() == 0)
    {
        return false;
    }

    for (size_t i = 0; i < valid_number.size(); ++i)
    {
        game.Write(valid_number[i]);

        // Check if current player can win whatever the other player writes
        vector<int> valid_number_next;
        game.GetValidNumbers(&valid_number_next);
        bool flag = true;
        for (size_t j = 0; j < valid_number_next.size(); ++j)
        {
            game.Write(valid_number_next[j]);
            flag &= DepthFirstSearch(game);
            game.Erase(valid_number_next[j]);
            if (!flag)
            {
                break;
            }
        }
        game.Erase(valid_number[i]);
        
        if (flag)
        {
            return true;
        }
    }
    return false;
}

int main(int argc, char** argv)
{
    int max_number = 29;  // The valid max number to be written
    if (argc > 1)
    {
        max_number = atoi(argv[1]);
    }

    NumberGame game(max_number);
    if (argc == 1)
    {
        game.Write(5);
        game.Write(6);
    }
    
    // Find the optimal strategy for player A
    if (DepthFirstSearch(game))
    {
        printf("Player A has optimal strategy.\n");
    }
    else
    {
        printf("Player A does not have optimal strategy.\n");
    }

    // Find the optimal strategy for player B
    bool flag = true;
    vector<int> valid_number_A;
    game.GetValidNumbers(&valid_number_A);
    for (size_t i = 0; i < valid_number_A.size(); ++i)
    {
        game.Write(valid_number_A[i]);
        flag &= DepthFirstSearch(game);
        game.Erase(valid_number_A[i]);
        if (!flag)
        {
            break;
        }
    }
    if (flag)
    {
        printf("Player B has optimal strategy.\n");
    }
    else
    {
        printf("Player B does not have optimal strategy.\n");
    }

    return 0;
}
