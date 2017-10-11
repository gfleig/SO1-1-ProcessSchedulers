#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include <algorithm>
#include <iostream>
#include "process.hpp"

/*class variables are used to store the values from the specific function.
  multiple instances of Scheduler are needed to execute different types of schedulers*/
class Scheduler
{
public:
    float averageReturnTime,      //time until process is finished
        averageResponseTime,    //time until first response
        averageWaitTime;        //time not being executed

    Scheduler();                //constructor won't be used

    ~Scheduler();

    void FCFS(std::vector <Process> processList);     //functions to execute the scheduling method that will update the local time variables
    void SJF(std::vector <Process> processList);
    void RR(std::vector <Process> processList);
};

#endif