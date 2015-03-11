/*************************************************************************
    > File Name: main.cpp
    > Author: Guo Hengkai
    > Description: Main program for homework 2C 
    > Created Time: Tue 10 Mar 2015 01:54:55 PM CST
 ************************************************************************/
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "single_noble_board.h"

using std::vector;
using ghk::ChessmanStep;
using ghk::SingleNobleBoard;

bool DepthFirstSearch(size_t depth, SingleNobleBoard &board,
        vector<ChessmanStep>* best_res)
{
    if (depth >= board.chessman_num() - 1)
    {
        board.CopyHistory(best_res);
        return true;  // Find the solution of one chessman
    }
    
    vector<ChessmanStep> valid_step;
    board.GetValidSteps(&valid_step);

    if (valid_step.empty())
    {
        if (best_res->size() < depth)
        {
            board.CopyHistory(best_res);
        }
    }
    else
    {
        for (size_t i = 0; i < valid_step.size(); ++i)
        {
            board.Move(valid_step[i]);
            if (DepthFirstSearch(depth + 1, board, best_res))
            {
                return true;
            }
            board.Back();
        }
    }

    return false;
}

int main(int argc, char** argv)
{
    bool extend = false;
    if (argc > 1)
    {
        extend = (atoi(argv[1]) == 1);
    }

    // Start DFS for solution
    SingleNobleBoard board(extend);
    vector<ChessmanStep> best_res;
    DepthFirstSearch(0, board, &best_res);
    
    // Print the solution
    printf("The number of remained chessman is %zu.\n",
            board.chessman_num() - best_res.size());
    printf("The solution:\n");
    for (size_t i = 0; i < best_res.size(); ++i)
    {
        printf("%zu: ", i);
        board.PrintStep(best_res[i]);
    }

    return 0;
}

