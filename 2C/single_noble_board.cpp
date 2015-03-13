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


SingleNobleBoard::SingleNobleBoard(bool extend):
    extend_(extend), chessman_num_(32)
{
    InitBoard();
}

void SingleNobleBoard::Move(ChessmanStep& step)
{
    size_t index = step.index;
    int next = next_index_[index][step.move];
    int move = move_index_[index][step.move];
    if ((state_[index] == BoardState::Occupied) && (next >= 0) && (move >= 0)
            && (state_[next] == BoardState::Occupied)
            && (state_[move] == BoardState::Empty))
    {
        history_.push_back(step);
        state_[index] = BoardState::Empty;
        state_[next] = BoardState::Empty;
        state_[move] = BoardState::Occupied;
    }
}

void SingleNobleBoard::Back()
{
    ChessmanStep step = history_.back();
    size_t index = step.index;
    int next = next_index_[index][step.move];
    int move = move_index_[index][step.move];

    state_[index] = BoardState::Occupied;
    state_[next] = BoardState::Occupied;
    state_[move] = BoardState::Empty;
    
    history_.pop_back();
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
    if (steps == nullptr)
    {
        return;
    }

    *steps = std::move(vector<ChessmanStep>());
    for (size_t i = 0; i < state_.size(); ++i)
    {
        if (state_[i] == BoardState::Occupied)
        {
            for (int j = 0; j < 4; ++j)
            {
                int next = next_index_[i][j];
                int move = move_index_[i][j];
                if ((next >= 0) && (move >= 0))
                {
                    if ((state_[next] == BoardState::Occupied)
                            && (state_[move] == BoardState::Empty))
                    {
                        ChessmanStep step{i, static_cast<Movement>(j)};
                        steps->push_back(step);
                    }
                }
            }
        }
    }
}

void SingleNobleBoard::GetTransformedState(vector<vector<BoardState>> *states) const
{
    if (states == nullptr)
    {
        return;
    }
    *states = std::move(vector<vector<BoardState>>());

    for (size_t i = 0; i < transform_index_.size(); ++i)
    {
        vector<BoardState> state;
        for (size_t j = 0; j < state_.size(); ++j)
        {
            state.push_back(state_[transform_index_[i][j]]);
        }

        bool flag = true;
        if (i > 0)
        {
            for (size_t j = 0; j < states->size(); ++j)
            {
                if ((*states)[j] == state)
                {
                    flag = false;
                    break;
                }
            }
        }
        if (flag)
        {
            states->push_back(state);
        }
    }
}

void SingleNobleBoard::PrintBoard() const
{
    vector<vector<BoardState>> board;
    StateToBoard(&board);

    for (size_t i = 0; i < board_size_; ++i)
    {
        for (size_t j = 0; j < board_size_; ++j)
        {
            char ch;
            switch (board[i][j])
            {
                case BoardState::Empty:
                    ch = '.';
                    break;
                case BoardState::Occupied:
                    ch = 'o';
                    break;
                case BoardState::Invalid:
                    ch = ' ';
                    break;
                default:
                    ch = ' ';
                    break;
            }
            printf("%c", ch);
        }
        printf("\n");
    }
}

void SingleNobleBoard::PrintStep(ChessmanStep &step) const
{
    printf("(%zu, %zu) ", pos_[step.index].row, pos_[step.index].col);
    switch (step.move)
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
void SingleNobleBoard::InitBoard()
{
    size_t chess_step;
    if (extend_)
    {
        board_size_ = 11;
        chess_step = 2;
    }
    else
    {
        board_size_ = 9;
        chess_step = 1;
    }

    // Generate the whole board
    vector<vector<BoardState>> board(board_size_);
    for (size_t i = 0; i < board_size_; ++i)
    {
        board[i] = vector<BoardState>(board_size_, BoardState::Invalid);
    }

    for (size_t i = 3; i < board_size_ - 3; i += chess_step)
    {
        for (size_t j = 1; j < 3; ++j)
        {
            board[i][j] = BoardState::Occupied;
            board[i][board_size_ - j - 1] = BoardState::Occupied;
            board[j][i] = BoardState::Occupied;
            board[board_size_ - j - 1][i] = BoardState::Occupied;
        }
        for (size_t j = 3; j < board_size_ - 3; j += chess_step)
        {
            board[i][j] = BoardState::Occupied;
        }

    }
    board[board_size_ / 2][board_size_ / 2] = BoardState::Empty;

    if (extend_)
    {
        for (size_t i = 4; i < 8; i += 2)
            for (size_t j = 1; j < board_size_ - 1; ++j)
            {
                board[i][j] = BoardState::Empty;
                board[j][i] = BoardState::Empty;
            }
    }

    // Generate the state and its corresponding vectors
    vector<vector<int>> board_num(board_size_);
    for (size_t i = 0; i < board_size_; ++i)
    {
        board_num[i] = vector<int>(board_size_, -1);
    }

    state_.clear();
    for (size_t i = 0; i < board_size_; ++i)
        for (size_t j = 0; j < board_size_; ++j)
        {
            if (board[i][j] != BoardState::Invalid)
            {
                board_num[i][j] = state_.size();
                state_.push_back(board[i][j]);
                Position pos{i, j};
                pos_.push_back(pos);
            }
        }

    for (size_t i = 0; i < state_.size(); ++i)
    {
        vector<int> next(4, -1);
        vector<int> move(4, -1);
        int x = pos_[i].row;
        int y = pos_[i].col;
        for (int j = 0; j < 4; ++j)
        {
            next[j] = board_num[x + MOVE[j][0]][y + MOVE[j][1]];
            if (next[j] >= 0)
            {
                move[j] = board_num[x + MOVE[j][0] * 2][y + MOVE[j][1] * 2];
            }
        }
        next_index_.push_back(next);
        move_index_.push_back(move);
    }

    // Generate the roated and flip pattern
    vector<size_t> state_num;
    for (size_t i = 0; i < state_.size(); ++i)
    {
        state_num.push_back(i);
    }
    // Rotate the state
    for (int i = 0; i < 4; ++i)
    {
        vector<size_t> transformed_num;
        for (size_t i = 0; i < state_num.size(); ++i)
        {
            transformed_num.push_back(board_num
                    [board_size_ - 1 - pos_[state_num[i]].col]
                    [pos_[state_num[i]].row]);
        }
        state_num = transformed_num;
        transform_index_.push_back(state_num);
        /*
        for (size_t i = 0; i < state_num.size(); ++i)
        {
            printf("%zu,", state_num[i]);
        }
        printf("\n"); */
    }
    // Flip the state
    vector<size_t> fliped_num;
    for (size_t i = 0; i < state_num.size(); ++i)
    {
        fliped_num.push_back(board_num
                [pos_[state_num[i]].row]
                [board_size_ - 1 - pos_[state_num[i]].col]);
    }
    state_num = fliped_num;
    transform_index_.push_back(state_num);
    // Rotate the state again
    for (int i = 0; i < 3; ++i)
    {
        vector<size_t> transformed_num;
        for (size_t i = 0; i < state_num.size(); ++i)
        {
            transformed_num.push_back(board_num
                    [board_size_ - 1 - pos_[state_num[i]].col]
                    [pos_[state_num[i]].row]);
        }
        state_num = transformed_num;
        transform_index_.push_back(state_num);
        /*
        for (size_t i = 0; i < state_num.size(); ++i)
        {
            printf("%zu,", state_num[i]);
        }
        printf("\n"); */
    }

}

void SingleNobleBoard::StateToBoard(vector<vector<BoardState>> *board) const
{
    *board = std::move(vector<vector<BoardState>>(board_size_));
    for (size_t i = 0; i < board_size_; ++i)
    {
        (*board)[i] = vector<BoardState>(board_size_, BoardState::Invalid);
    }

    for (size_t i = 0; i < state_.size(); ++i)
    {
        (*board)[pos_[i].row][pos_[i].col] = state_[i];
    }
}
}  // namespace ghk

