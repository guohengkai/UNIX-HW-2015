/*************************************************************************
    > File Name: main.cpp
    > Author: Guo Hengkai
    > Description: Main program of pouring water for homework 4B 
    > Created Time: Fri 13 Mar 2015 11:24:44 PM CST
 ************************************************************************/
#include <cstdio>
#include <vector>

using std::vector;

struct CupState
{
    int v[3];
    int pre_index;
};

bool GetValidState(CupState &state, int index, int be, int en,
                   int v[3], CupState *new_state)
{
    int delta = std::min(state.v[be], v[en] - state.v[en]);
    if (delta == 0)
    {
        return false;
    }
    else
    {
        if (new_state == nullptr)
        {
            return false;
        }

        *new_state = state;
        new_state->v[be] -= delta;
        new_state->v[en] += delta;
        new_state->pre_index = index;
        return true;
    }
}

bool IsTarget(CupState state, int target)
{
    return ((state.v[0] == target) || (state.v[1] == target)
            || (state.v[2] == target)) && ((state.v[0] == 0)
            || (state.v[1] == 0) || (state.v[2] == 0));
}

bool BreadthFirstSearch(vector<CupState> &state_queue, int v[3])
{
    int target = v[0] / 2;

    CupState init_state{{v[0], 0, 0}, -1};
    state_queue.push_back(init_state);

    int head = 0;
    int tail = 1;
    while (head < tail)
    {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
            {
                if (i == j)
                {
                    continue;
                }

                CupState state;
                if (GetValidState(state_queue[head], head, i, j, v, &state))
                {
                    state_queue.push_back(state);
                    ++tail;
                    if (IsTarget(state, target))
                    {
                        return true;
                    }
                }
            }
        ++head;
    }

    return false;
}

int main()
{
    int v[3];
    while (true)
    {
        printf("Please input the volumn of two cups (sum must be even): \n");
        scanf("%d %d", v + 1, v + 2);
        if ((v[1] + v[2]) % 2 == 0)
        {
            break;
        }
    }

    v[0] = v[1] + v[2];
    vector<CupState> state_queue;
    if (BreadthFirstSearch(state_queue, v))
    {
        vector<CupState> result;
        int current_index = state_queue.size() - 1;
        while (current_index >= 0)
        {
            result.push_back(state_queue[current_index]);
            current_index = state_queue[current_index].pre_index;
        }

        printf("For cups of (%d, %d, %d), the number of steps is %zu:\n",
                v[0], v[1], v[2], result.size());
        for (auto iter = result.rbegin();
                iter != result.rend(); ++iter)
        {
            printf("(%d, %d, %d)\n", iter->v[0], iter->v[1], iter->v[2]);
        }
    }
    else
    {
        printf("No solution found.\n");
    }
}
