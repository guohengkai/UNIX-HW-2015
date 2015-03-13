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

bool DepthFirstSearch(int depth, NumberGame &game, int *number)
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
            flag &= DepthFirstSearch(depth + 1, game, number);
            game.Erase();
            if (!flag)
            {
                break;
            }
        }
        game.Erase();
        
        if (flag)
        {
            if (depth == 0)
            {
                *number = valid_number[i];
            }
            return true;
        }
    }
    return false;
}

int main(int argc, char** argv)
{
    int max_number = 29;  // The valid max number to be written

    NumberGame game(max_number);
    game.Write(5);
    game.Write(6);
    
    if (argc > 1)  // Play game with computer
    {
        int m;
        DepthFirstSearch(0, game, &m);
        game.Write(m);
        game.PrintState();

        while (true)
        {
            printf("Please enter 1 to 29: (0 for current state)\n");
            int n;
            scanf("%d", &n);
            if (n == 0)
            {
               game.PrintState(); 
            }
            else if (n == 1)
            {
                printf("You lose.\n");
                break;
            }
            else
            {
                if (game.Write(n))
                {
                    int m;
                    DepthFirstSearch(0, game, &m);
                    game.Write(m);
                    printf("Computer selected %d. \n", m);
                    game.PrintState();
                }

            }
        }
    }
    else  // Find the optimal strategy
    {
        // Find the optimal strategy for player A
        int m;
        if (DepthFirstSearch(0, game, &m))
        {
            printf("Player A has optimal strategy with selection of %d.\n", m);
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
            int m;
            flag &= DepthFirstSearch(1, game, &m);
            game.Erase();
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
    }
    return 0;
}
