/*************************************************************************
    > File Name: philosopher.cpp
    > Author: Guo Hengkai
    > Description: Base philosopher class implementation for homework 1D
    > Created Time: Sat 07 Mar 2015 02:32:45 PM CST
 ************************************************************************/
#include "philosopher.h"
#include <cstdio>
#include <string>
#include <sstream>
#include <vector>
#include "fork.h"

using std::string;
using std::vector;

namespace ghk
{
void Philosopher::Run()
{
    if (!KeepState())
    {
        TryChangeState();
    }
}

void Philosopher::StartEating()
{
    state_ = PersonState::Eating;
    UpdateRemainTime();
}

const string Philosopher::forks_list() const
{
    string result = "";
    for (size_t i = 0; i < forks_.size(); ++i)
    {
        if (has_fork(forks_[i]))
        {
            std::stringstream ss;
            ss << forks_[i]->fork_num();
            string temp;
            ss >> temp;
            result += temp + " ";
        }
    }

    return result;
}

int Philosopher::forks_num() const
{
    int result = 0;
    for (size_t i = 0; i < forks_.size(); ++i)
    {
        if (has_fork(forks_[i]))
        {
            result++;
        }
    }

    return result;
}

bool Philosopher::KeepState()
{
    if (remain_time_ > 0)
    {
        --remain_time_;
        if (state_ == PersonState::Eating)
        {
            ++total_eating_time_;
        }
        return true;
    }
    else
    {
        return false;
    }
}

void Philosopher::TryChangeState()
{
    switch (state_)
    {
        case PersonState::Thinking:
            state_ = PersonState::Hungry;
            remain_time_ = 0;
            break;
        case PersonState::Hungry:
            RequestAllForks();
            break;
        case PersonState::Eating:
            ReleaseAllForks();
            state_ = PersonState::Thinking;
            UpdateRemainTime();
            break;
        default:
            break;
    }
}


void Philosopher::RequestAllForks()
{
    for (size_t i = 0; i < forks_.size(); ++i)
    {
        if (!has_fork(forks_[i]))
        {
            forks_[i]->Request(this);
        }
    }
}

void Philosopher::ReleaseAllForks()
{
    for (size_t i = 0; i < forks_.size(); ++i)
    {
        if (has_fork(forks_[i]))
        {
            forks_[i]->Release();
        }
    }
}
}  // namespace ghk
