#include "process.hpp"

Process::Process(int id, int startTime, int durationTime)
{
    this->id = id; 
    this->startTime = startTime;
    this->durationTime = durationTime;
    this->originalDurationTime = durationTime;
}

Process::~Process()
{
    ;
}
