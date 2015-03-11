/*************************************************************************
    > File Name: single_noble_board.cpp
    > Author: Guo Hengkai
    > Description: Single noble board class implementation for homework 2C
    > Created Time: Tue 10 Mar 2015 03:25:00 PM CST
 ************************************************************************/
#include "single_noble_board.h"
#include <cstdio>
#include <vector>

using std::vector;

namespace ghk
{
const int MOVE[][2] = {
    { 1, 0 }, { -1, 0 },
    { 0, -1 }, { 0, 1 }
};

void ChessmanStep::Print() const
{
    printf("(%d, %d) ", row, col);
    switch (move)
    {
        case Movement::Down:
            printf("Down\n");
            break;
        case Movement::Up:
            printf("Up\n");
            break;
        case Movement::Left:
            printf("Left\n");
            break;
        case Movement::Right:
            printf("Right\n");
            break;
        default:
            break;
    }
}

SingleNobleBoard::SingleNobleBoard(bool extend):
    extend_(extend), chessman_num_(32)
{
    InitBoard();
}

void SingleNobleBoard::Move(ChessmanStep& step)
{

}

void SingleNobleBoard::Back(ChessmanStep& step)
{

}

void SingleNobleBoard::CopyHistory(vector<ChessmanStep>* steps) const
{
    if (steps == nullptr)
    {
        return;
    }

    *steps = std::move(vector<ChessmanStep>(history_));
}

void SingleNobleBoard::GetValidSteps(vector<ChessmanStep>* steps) const
{

}

void SingleNobleBoard::InitBoard()
{
    size_t board_size;
    size_t chess_step;
    // Add two rows and columns for board to restrict movement
    if (extend_)
    {
        board_size = 11;
        chess_step = 2;
    }
    else
    {
        board_size = 9;
        chess_step = 1;
    }

    state_ = vector<vector<BoardState>>(board_size);
    for (size_t i = 0; i < board_size; ++i)
    {
        state_[i] = vector<BoardState>(board_size, BoardState::Invalid);
    }

    
}
}  // namespace ghk

