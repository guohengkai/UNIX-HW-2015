/*************************************************************************
    > File Name: number_game.h
    > Author: Guo Hengkai
    > Description: Number game class definition for homework 2A
    > Created Time: Thu 12 Mar 2015 09:18:46 AM CST
 ************************************************************************/
#ifndef TWO_A_NUMBER_GAME_H_
#define TWO_A_NUMBER_GAME_H_

#include <vector>

namespace ghk
{
class NumberGame
{
public:
    explicit NumberGame(int max_number): step_(0), max_number_(max_number),
        state_(std::vector<int>(max_number, 0)) {}

    bool Write(int number);
    void Erase();

    void GetValidNumbers(std::vector<int> *valid_number);
    void PrintState() const;

private:
    int step_;
    int max_number_;
    // 0 for empty, + for written number, - for invalid number
    std::vector<int> state_; 
};
}  // namespace ghk
#endif  // TWO_A_NUMBER_GAME_H_

