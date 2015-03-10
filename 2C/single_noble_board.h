/*************************************************************************
    > File Name: single_noble_board.h
    > Author: Guo Hengkai
    > Description: Single noble board class definition for homework 2C
    > Created Time: Tue 10 Mar 2015 02:55:43 PM CST
 ************************************************************************/
#ifndef TWO_C_SINGLE_NOBLE_BOARD_H_
#define TWO_C_SINGLE_NOBLE_BOARD_H_

#include <vector>

namespace ghk
{
enum Movement
{
    Down = 0,
    Up,
    Left,
    Right
};

enum BoardState
{
    Empty = 0,
    Occupied,
    Invalid
};

struct ChessmanStep
{
    void Print() const;

    int row;
    int col;
    Movement move;
};

class SingleNobleBoard
{
public:
    explicit SingleNobleBoard(bool extend = false): chessman_num_(32);

    void Move(ChessmanStep& step);
    void Back(ChessmanStep& step);

    void CopyHistory(std::vector<ChessmanStep>* steps) const;
    void GetValidSteps(std::vector<ChessmanStep>* steps) const;
    
    size_t chessman_num() { return chessman_num_; }

private:
    size_t chessman_num_;
    std::vector<std::vector<BoardState>> state_;
    std::vector<ChessmanStep> history_;
};
}  // namespace ghk

#endif  // TWO_C_SINGLE_NOBLE_BOARD_H_

