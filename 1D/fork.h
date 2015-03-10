/*************************************************************************
    > File Name: fork.h
    > Author: Guo Hengkai
    > Description: Fork class definition for homework 1D
    > Created Time: Sat 07 Mar 2015 03:34:52 PM CST
 ************************************************************************/
#ifndef ONE_D_FORK_H_
#define ONE_D_FORK_H_

#include <vector>

namespace ghk
{
class Philosopher;  // Forward declaration   
class Fork
{
public:
    explicit Fork(int fork_num): fork_num_(fork_num), owner_ptr_(nullptr) {}

    void Request(Philosopher* person_ptr);
    void Release();
    void ProcessList();

    int fork_num() const { return fork_num_; }
    Philosopher* owner_ptr() const { return owner_ptr_; }
    bool is_used() const { return owner_ptr_ != nullptr; }

private:
    int fork_num_;

    std::vector<Philosopher*> waiting_list_;
    Philosopher* owner_ptr_;
};
}  // namespace ghk

#endif  // ONE_D_FORK_H_

