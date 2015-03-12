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
    std::size_t index;
    Movement move;
};

struct Position
{
    std::size_t row;
    std::size_t col;
};

class SingleNobleBoard
{
public:
    explicit SingleNobleBoard(bool extend = false);

    void Move(ChessmanStep &step);
    void Back();

    void CopyHistory(std::vector<ChessmanStep> *steps) const;
    void GetValidSteps(std::vector<ChessmanStep> *steps) const;
    
    void PrintBoard() const;
    void PrintStep(ChessmanStep &step) const;

    std::size_t chessman_num() { return chessman_num_; }
    std::vector<BoardState>& state() { return state_; }

private:
    virtual void InitBoard();
    void StateToBoard(std::vector<std::vector<BoardState>> *board) const;

    bool extend_;
    std::size_t board_size_;
    std::size_t chessman_num_;

    // Put all the possible position into one vector,
    // and index all the possible movement
    std::vector<BoardState> state_;
    std::vector<Position> pos_;
    std::vector<std::vector<int>> next_index_;
    std::vector<std::vector<int>> move_index_;
    std::vector<ChessmanStep> history_;  // Save the movement history
};
}  // namespace ghk

#endif  // TWO_C_SINGLE_NOBLE_BOARD_H_

