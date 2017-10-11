#include <iostream>
#include <fstream>
#include <iomanip>

#include "process.hpp"
#include "scheduler.hpp"

/*gets process list from file, returns vector of all processes,
in order of appearence on file*/
std::vector <Process> getProcessList(const char* filename)
{
    std::ifstream file;
    file.open(filename, std::ios::in);

    int id = 0;                                 //each process will have an id, according to its position in file
    std::vector <Process> processList;

    while(file.good())
    {
        int processStartTime, processDurationtime;

        file >> processStartTime;
        file >> processDurationtime;

        Process newProcess(id, processStartTime, processDurationtime);
        processList.push_back(newProcess);

        ++id;
    }

    file.close();

    return processList;
}

int main()
{
    std::cout << std::fixed << std::setprecision(1);     //set decimal prcision to 1 case

    std::vector <Process> processList = getProcessList("processos.txt");

    Scheduler fcfs, sjf, rr;

    fcfs.FCFS(processList);
    sjf.SJF(processList);
    rr.RR(processList);

    return 0;
}
