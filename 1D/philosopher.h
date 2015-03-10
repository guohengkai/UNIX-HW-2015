/*************************************************************************
    > File Name: philosopher.h
    > Author: Guo Hengkai
    > Description: Base philosopher class definition for homework 1D
    > Created Time: Fri 06 Mar 2015 01:01:26 PM CST
 ************************************************************************/
#ifndef ONE_D_PHILOSOPHER_H_
#define ONE_D_PHILOSOPHER_H_

#include <string>
#include <vector>
#include "fork.h"

namespace ghk
{
enum PersonState
{
    Thinking = 0,
    Hungry,
    Eating
};

class Philosopher
{
public:
    Philosopher(int person_num, int max_time, Fork& left_fork,
            Fork& right_fork): state_(PersonState::Thinking),
            max_time_(max_time), total_eating_time_(0),
            person_num_(person_num)
    {
        UpdateRemainTime();
        forks_.push_back(&left_fork);
        forks_.push_back(&right_fork);
    }

    virtual void Run();  // Run the philosopher for one time unit
    void StartEating();
    
    int person_num() const { return person_num_; }
    int total_eating_time() const { return total_eating_time_; }

    bool has_fork(const Fork* fork) const { return fork->owner_ptr() == this; }
    const char* state() const
    {
        switch (state_)
        {
            case PersonState::Thinking:
                return "Thinking";
            case PersonState::Hungry:
                return "Hungry";
            case PersonState::Eating:
                return "Eating";
            default:
                return "";
        }
    }
    const std::string forks_list() const;
    int forks_num() const;

protected:
    void UpdateRemainTime() { remain_time_ = random() % max_time_ + 1; }
    bool KeepState();
    virtual void TryChangeState();

    void RequestAllForks();
    void ReleaseAllForks();

    PersonState state_;
    int max_time_;
    int remain_time_;
    int total_eating_time_;
    
    std::vector<Fork*> forks_;

private:
    int person_num_;
};
}  // namespace ghk

#endif  // ONE_D_PHILOSOPHER_H_

