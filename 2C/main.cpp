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

#define REPEAT_CHECK 0

using std::vector;
using ghk::BoardState;
using ghk::ChessmanStep;
using ghk::SingleNobleBoard;

vector<vector<vector<BoardState>>> state_history;

bool IsUnique(size_t depth, vector<vector<BoardState>> &state)
{
    if (depth + 1 > state_history.size())
    {
        state_history.push_back(vector<vector<BoardState>>());
        return true;
    }
    for (size_t i = 0; i < state_history[depth].size(); ++i)
    {
        for (size_t j = 0; j < state.size(); ++j)
        {
            if (state[j] == state_history[depth][i])
            {
                return false;
            }
        }
    }
    return true;
}

bool DepthFirstSearch(size_t depth, SingleNobleBoard &board,
        vector<ChessmanStep>* best_res, size_t limit)
{
    if (depth >= board.chessman_num() - limit)
    {
        board.CopyHistory(best_res);
        printf("Find solution: %zu\n", board.chessman_num() - best_res->size());
        return true;  // Find the solution of one chessman
    }
    
    vector<ChessmanStep> valid_step;
    board.GetValidSteps(&valid_step);

    if (valid_step.empty())
    {
        if (best_res->size() < depth)
        {
            board.CopyHistory(best_res);
            printf("Find solution: %zu\n", board.chessman_num() - best_res->size());
        }
    }
    else
    {
        for (size_t i = 0; i < valid_step.size(); ++i)
        {
            board.Move(valid_step[i]);

#if REPEAT_CHECK
            vector<vector<BoardState>> states;
            board.GetTransformedState(&states);
            if (IsUnique(depth, states))
            {
                for (size_t j = 0; j < states.size(); j++)
                {
                    state_history[depth].push_back(states[j]);
                }
                if (DepthFirstSearch(depth + 1, board, best_res, limit))
                {
                    return true;
                }
            }
#else
            if (DepthFirstSearch(depth + 1, board, best_res, limit))
            {
                return true;
            }
#endif
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
    size_t limit = extend ? 4 : 1;  // The best solution for 9*9 is 4
    vector<ChessmanStep> best_res;
    DepthFirstSearch(0, board, &best_res, limit);
    
    // Print the solution
    printf("The number of remained chessman is %zu.\n",
            board.chessman_num() - best_res.size());
    printf("The solution:\n");
    board = SingleNobleBoard(extend);  // Reset the board for output
    board.PrintBoard();
    for (size_t i = 0; i < best_res.size(); ++i)
    {
        printf("%zu: ", i);
        board.PrintStep(best_res[i]);
        board.Move(best_res[i]);
        board.PrintBoard();
    }

    return 0;
}

