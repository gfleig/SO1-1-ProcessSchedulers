#ifndef PROCESS_H
#define PROCESS_H

class Process
{
public:

    Process(int id, int startTime, int durationTime);

    ~Process();

    int id,
        startTime,
        durationTime,
        originalDurationTime;
        
    int returnTime      = 0;
    int responseTime    = 0;
    int waitTime        = 0;
    int processedTime   = 0;
};

#endif