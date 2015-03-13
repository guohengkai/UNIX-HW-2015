/*************************************************************************
    > File Name: number_game.cpp
    > Author: Guo Hengkai
    > Description: Number game class implementation for homework 2A
    > Created Time: Thu 12 Mar 2015 09:32:47 AM CST
 ************************************************************************/
#include "number_game.h"
#include <cmath>
#include <cstdio>
#include <vector>

using std::vector;

namespace ghk
{
bool NumberGame::Write(int number)
{
    if (state_[number - 1] != 0)
    {
        printf("Invalid operation.\n");
        return false;
    }

    ++step_;
    state_[number - 1] = step_;
    for (int i = 1; i < max_number_ - number; ++i)
    {
       if ((state_[i] != 0) && (state_[i + number] == 0)) 
       {
           state_[i + number] = -step_;
       }
    }
    return true;
}

void NumberGame::Erase()
{
    for (int i = 1; i < max_number_; ++i)
    {
        if (std::abs(state_[i]) == step_)
        {
            state_[i] = 0;
        }
    }
    --step_;
}

void NumberGame::GetValidNumbers(vector<int> *valid_number)
{
    if (valid_number == nullptr)
    {
        return;
    }
    *valid_number = std::move(vector<int>());
    for (int i = 1; i < max_number_; ++i)
    {
        if (state_[i] == 0)
        {
            valid_number->push_back(i + 1);
        }
    }
}

void NumberGame::PrintState() const
{
    for (int i = 0; i < max_number_; ++i)
    {
        printf("%2d ", i + 1);
    }
    printf("\n");

    for (int i = 0; i < max_number_; ++i)
    {
        char ch;
        if (state_[i] > 0)
        {
            ch = '*';
        }
        else if (state_[i] < 0)
        {
            ch = 'x';
        }
        else
        {
            ch = ' ';
        }
        printf(" %c ", ch);
    }
    printf("\n");
}
}  // namespace ghk

