#pragma once
#include <string>

using namespace std;  

// Using integers to define the states Ex. Ready = 1
#define STATE_NEW         0
#define STATE_READY       1
#define STATE_RUNNING     2
#define STATE_WAITING     3
#define STATE_TERMINATED  4

class Process {

private:
    //-Core Attributes-
    int pid;
    int arrival_time;
    int burst_time;
    int state;
    int remaining_time;
    // -Additional Attributes- (3)
    int completion_time;
    int turnaround_time;
    int waiting_time;

public:
    //-Methods-
    
    // 1. Constructor with parameters
    Process(int p_pid, int p_arrival_time, int p_burst_time);

    // 2. A method to update process state
    void setState(int newState);

    // 3. A method to decrement remaining execution time
    void runFor(int time);

    //-4. Getters-
    int getPid() const;
    int getArrivalTime() const;
    int getState() const;
    int getRemainingTime() const;
    bool isFinished() const;
    int getBurstTime() const; //(3)

    void setCompletionTime(int time); //(3)
    void setTurnaroundTime(int time); //(3)
    void setWaitingTime(int time); //(3)
};

// Helper function declaration for printing states
string stateToString(int state);