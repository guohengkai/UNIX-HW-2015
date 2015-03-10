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

SingleNobleBoard::SingleNobleBoard(bool extend)
{

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

    *steps = std::move(vector(history_));
}

void SingleNobleBoard::GetValidSteps(vector<ChessmanStep>* steps) const
{

}
}  // namespace ghk

