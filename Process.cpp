#include "Process.h"

using namespace std;

// 1. Constructor implementation 
Process::Process(int p_pid, int p_arrival_time, int p_burst_time) {
    pid = p_pid;
    arrival_time = p_arrival_time;
    burst_time = p_burst_time;
    state = STATE_NEW;
    remaining_time = burst_time;

    // Initialize new stats to 0 (3)
    completion_time = 0;
    turnaround_time = 0;
    waiting_time = 0;
}

// 2. Method to update the process state
void Process::setState(int newState) {
    state = newState;
}

// 3. Method to decrement remaining time
void Process::runFor(int time) {
    if (time > remaining_time) {
        remaining_time = 0;
    }
    else {
        remaining_time -= time;
    }
}

//-Getter implementations-
int Process::getPid() const { return pid; }
int Process::getArrivalTime() const { return arrival_time; }
int Process::getState() const { return state; }
int Process::getRemainingTime() const { return remaining_time; }
bool Process::isFinished() const { return (remaining_time == 0); }

int Process::getBurstTime() const { return burst_time; } //(3)

//-NEW Setter implementations- (3)
void Process::setCompletionTime(int time) { completion_time = time; }
void Process::setTurnaroundTime(int time) { turnaround_time = time; }
void Process::setWaitingTime(int time) { waiting_time = time; }

//-Helper function implementation-

string stateToString(int state) {
    switch (state) {
    case STATE_NEW: return "NEW";
    case STATE_READY: return "READY";
    case STATE_RUNNING: return "RUNNING";
    case STATE_WAITING: return "WAITING";
    case STATE_TERMINATED: return "TERMINATED";
    default: return "UNKNOWN";
    }
}