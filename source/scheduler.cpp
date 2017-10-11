#include "scheduler.hpp"

Scheduler::Scheduler()
{
    this->averageReturnTime     = 0;
    this->averageResponseTime   = 0;
    this->averageWaitTime       = 0;
}

Scheduler::~Scheduler()
{
    ;
}

/*local function to be used in std::sort*/
bool compareStartTime(Process p1, Process p2)
{
    return (p1.startTime < p2.startTime);
}

bool compareDurationTime(Process p1, Process p2)
{
    return (p1.durationTime < p2.durationTime);
}
/*end of local functions*/

void Scheduler::FCFS(std::vector <Process> processList)
{
    int totalReturnTime     = 0;
    int totalResponseTime   = 0;
    int totalWaitTime       = 0;
    int executionTime       = 0;                    //total time of "CPU" execution
    const int numberOfProcesses = processList.size();

    std::vector <Process> activeProcessList = processList;

    std::sort(activeProcessList.begin(), activeProcessList.end(), compareStartTime);

    //for each process
    for(unsigned int i = 0; i < activeProcessList.size(); ++i)
    {
        Process process = activeProcessList[i];     //gets the current process to spare me typing activeProcessList so many fucking times

        /*
        if there's a gap between the current execution time and the start of the next process,
        set the execution time to the start of the next process.
        */
        if(process.startTime > executionTime)       
        {
            executionTime = process.startTime;
        }

        totalReturnTime += executionTime + process.durationTime - process.startTime;

        totalResponseTime += executionTime - process.startTime;

        totalWaitTime += executionTime - process.startTime;

        executionTime += process.durationTime;
    }

    this->averageReturnTime = (float) totalReturnTime / (float) numberOfProcesses;
    this->averageResponseTime = (float) totalResponseTime / (float) numberOfProcesses;
    this->averageWaitTime = (float) totalWaitTime / (float) numberOfProcesses;

    std::cout << "FCFS " << this->averageReturnTime << " " << this->averageResponseTime << " " << this->averageWaitTime << std::endl;
}

void Scheduler::SJF(std::vector <Process> processList)
{
    int totalReturnTime     = 0;
    int totalResponseTime   = 0;
    int totalWaitTime       = 0;
    int executionTime       = 0;                                                           //total time of "CPU" execution
    const int numberOfProcesses = processList.size();    

    std::vector <Process> activeProcessList = processList;

    std::vector <Process> queuedProcessList;                                               //list of processes awating to be executed
    
    std::sort(activeProcessList.begin(), activeProcessList.end(), compareStartTime);       //sort firstly by start time

    //for each process
    while(activeProcessList.size() != 0)
    {           
        for(unsigned int j = 0; j < activeProcessList.size(); ++j)      //for each process 
        {
            if(activeProcessList[j].startTime <= executionTime)         //wich start time is less or equal to the current execution time
            {                
                queuedProcessList.push_back(activeProcessList[j]);      //put that process in queue(list of processes waiting execution
            } 
        }

        std::sort(queuedProcessList.begin(), queuedProcessList.end(), compareDurationTime);                                 //sorts queued processes by duration
        std::sort(activeProcessList.begin(), activeProcessList.begin() + queuedProcessList.size(), compareDurationTime);    //sorts processes waiting execution by duration        

        Process process = queuedProcessList[0];                                                                             //gets the current process
        activeProcessList.erase(activeProcessList.begin());                                                                 //remove first process from list- it's executiong
        
        if(process.startTime > executionTime)                           //if there's a gap between the current execution time and the start of the next process,
        {
            executionTime = process.startTime;                          //set the execution time to the start of the next process.
        }

        totalReturnTime += executionTime + process.durationTime - process.startTime;

        totalResponseTime += executionTime - process.startTime;

        totalWaitTime += executionTime - process.startTime;

        executionTime += process.durationTime;

        queuedProcessList.clear();                                      //clears queue of processes waiting execution       
    }

    this->averageReturnTime = (float) totalReturnTime / (float) numberOfProcesses;
    this->averageResponseTime = (float) totalResponseTime / (float) numberOfProcesses;
    this->averageWaitTime = (float) totalWaitTime / (float) numberOfProcesses;

    std::cout << "SJF " << this->averageReturnTime << " " << this->averageResponseTime << " " << this->averageWaitTime << std::endl;
}

void Scheduler::RR(std::vector <Process> processList)
{
    int totalReturnTime     = 0;
    int totalResponseTime   = 0;
    int totalWaitTime       = 0;
    int executionTime       = 0;                                                //total time of "CPU" execution
    const int numberOfProcesses = processList.size();
    const int quantum = 2;     
    
    std::vector <bool> wasExecuted (numberOfProcesses, false);                  

    std::vector <int> firstArrivalTime (numberOfProcesses);

    std::vector <Process> activeProcessList = processList;
    std::vector <Process> queuedProcessList;                                    //list/queue of processes ready for execution
    
    std::sort(activeProcessList.begin(), activeProcessList.end(), compareStartTime);

    while(activeProcessList.size() || queuedProcessList.size())
    {
        while(  activeProcessList.size()                                        //first construction of the ready queue
                && activeProcessList.front().startTime <= executionTime)
        {
            queuedProcessList.push_back(activeProcessList.front());
            activeProcessList.erase(activeProcessList.begin());
        }    
        
        if(queuedProcessList.size())
        {
    
            Process process = queuedProcessList.front();                        //gets first process in list        
            queuedProcessList.erase(queuedProcessList.begin());                 //and deletes it

            //totalWaitTime += executionTime - process.startTime - process.originalDurationTime;
            totalWaitTime += executionTime - process.startTime;

            if(wasExecuted[process.id] == false)                                //checks if the current process has already been executed 
            {   
                firstArrivalTime[process.id] = process.startTime;               //and sotres the original start time
                totalResponseTime += executionTime - process.startTime;
                wasExecuted[process.id] = true;
            }                    

            if(process.durationTime > quantum)
            {
                executionTime += quantum;
                process.durationTime -= quantum;                  
            }
            else
            {
                executionTime += process.durationTime;
                process.durationTime = 0;                
            }
            
            while(  activeProcessList.size()                                    //puts new processes in the back of the queue,
            && activeProcessList.front().startTime <= executionTime)            //after the current one "ended execution"
            {
                queuedProcessList.push_back(activeProcessList.front());
                activeProcessList.erase(activeProcessList.begin());
            }

            if(process.durationTime > 0)
            {
                process.startTime = executionTime;

                queuedProcessList.push_back(process);                           //puts current process in the end of the queue, to be the last to be executed
            }
            else
            {
                totalReturnTime += executionTime - firstArrivalTime[process.id] /*- process.startTime - process.durationTime*/;
            }            
        }
        else
        {           
            executionTime = queuedProcessList.front().startTime;                //in case the first process doesnt begin at time 0
        }
    }
    

    this->averageReturnTime = (float) totalReturnTime / (float) numberOfProcesses;
    this->averageResponseTime = (float) totalResponseTime / (float) numberOfProcesses;
    this->averageWaitTime = (float) totalWaitTime / (float) numberOfProcesses;

    std::cout << "RR " << this->averageReturnTime << " " << this->averageResponseTime << " " << this->averageWaitTime << std::endl;
}
