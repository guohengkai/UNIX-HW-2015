/*************************************************************************
    > File Name: fork.cpp
    > Author: Guo Hengkai
    > Description: Fork class implementation for homework 1D
    > Created Time: Sat 07 Mar 2015 04:01:37 PM CST
 ************************************************************************/
#include "fork.h"
#include <algorithm>
#include <vector>
#include "philosopher.h"

namespace ghk
{
void Fork::Request(Philosopher* person_ptr)
{
    waiting_list_.push_back(person_ptr);
}

void Fork::Release()
{
    owner_ptr_ = nullptr;
}

void Fork::ProcessList()
{
    if ((is_used()) || (waiting_list_.empty()))
    {
        return;
    }

    // Random select one person in the list
    std::random_shuffle(waiting_list_.begin(), waiting_list_.end());
    owner_ptr_ = waiting_list_[0];
    waiting_list_.clear();
    owner_ptr_->StartEating();
}
}  // namespace ghk
