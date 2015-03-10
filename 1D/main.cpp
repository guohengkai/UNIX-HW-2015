/*************************************************************************
    > File Name: main.cpp
    > Author: Guo Hengkai
    > Description: Main program for homework 1D
    > Created Time: Sat 07 Mar 2015 10:44:27 AM CST
 ************************************************************************/
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "fork.h"
#include "philosopher.h"

using std::vector;
using ghk::Fork;
using ghk::Philosopher;

int current_time;

void Simulate(vector<Philosopher*>& persons, vector<Fork>& forks, int time)
{
    for (int i = 0; i < time; ++i)
    {
        printf("Time %d start......\n", current_time + i);
        for (size_t j = 0; j < persons.size(); ++j)
        {
            persons[j]->Run();
        }

        // Process waiting list for each fork
        for (size_t j = 0; j < forks.size(); ++j)
        {
            forks[j].ProcessList();
        }

        // Print information of fork and state
        for (size_t j = 0; j < persons.size(); ++j)
        {
            printf("Philosopher %d is %s, who has forks: %s\n",
                    persons[j]->person_num(), persons[j]->state(),
                    persons[j]->forks_list().c_str());
        }

        // Print information of total eating time
        printf("Total eating time currently: \n");
        for (size_t j = 0; j < persons.size(); ++j)
        {
            printf("Philosopher %d is %d.\n", persons[j]->person_num(),
                    persons[j]->total_eating_time());
        }

        printf("****************************************************\n");
    }
    current_time += time;
}

void StartSimulation(int person_num)
{
    // Initialize all the people and forks
    printf("The number of philosopher: %d\n", person_num);
    vector<Fork> forks;
    for (int i = 0; i < person_num; ++i)
    {
        Fork fork(i);
        forks.push_back(fork);
    }
    
    vector<Philosopher> phers;
    vector<Philosopher*> pher_ptrs;
    for (int i = 0; i < person_num; ++i)
    {
        if (i == 0)
        {
            Philosopher pher(i, 3, forks[1], forks[4]);
            phers.push_back(pher);
        }
        else if (i == person_num - 1)
        {
            Philosopher pher(i, 3, forks[0], forks[3]);
            phers.push_back(pher);
        }
        else
        {
            Philosopher pher(i, 3, forks[i - 1], forks[i + 1]);
            phers.push_back(pher);
        }
    }
    for (int i = 0; i < person_num; ++i)
    {
        pher_ptrs.push_back(&(phers[i]));
    }

    // Start to simulate
    current_time = 0;
    while (true)
    {
        printf("Input the time you want to simulate: (<=0 for exit) ");
        int time;
        scanf("%d", &time);
        if (time <= 0)
        {
            printf("Exit simulation.\n");
            break;
        }

        Simulate(pher_ptrs, forks, time);
    }
}

int main(int argc, char** argv)
{
    int person_num = 5;
    if (argc > 1)
    {
        person_num = atoi(argv[1]);
    }

    StartSimulation(person_num);

    return 0;
}

