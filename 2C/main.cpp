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

void DepthFirstSearch(int depth, SingleNobleBoard &board,
        vector<ChessmanStep>* best_res)
{
    if (depth >= board.chessman_num() - 1)
    {
        *best_res = board.steps();
        return true;  // Find the solution of one chessman
    }
    
    vector<ChessmanStep> valid_step;
    board.GetValidSteps(&valid_step);

    if (valid_step.empty())
    {
        if (best_res->size() < depth)
        {
            *best_res = board.steps();
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
            board.Back(valid_step[i]);
        }
    }

    return false;
}

int main(int argc, char** argv)
{
    bool extend = false;
    if (argc > 1)
    {
        extend = atio(argv[1]) == 1 : true ? false;
    }

    // Start DFS for solution
    SingleNobleBoard board(extend);
    vector<ChessmanStep> best_res;
    DepthFirstSearch(0, board, &best_res);
    
    // Print the solution
    printf("The number of remained chessman is %d.\n",
            board.chessman_num() - best_res.size());
    printf("The solution:\n");
    for (size_t i = 0; i < best_res.size(); ++i)
    {
        printf("%zu: ", i);
        best_res[i].Print();
    }

    return 0;
}

