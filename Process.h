#pragma once
#include <string>

using namespace std;  

// Define integer constants for the process states [cite: 1773-1776]
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
};

// Helper function declaration for printing states
string stateToString(int state);