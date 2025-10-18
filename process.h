#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <iostream>

enum class ProcessState { NEW, READY, RUNNING, WAITING, TERMINATED };

class Process {
private:
    int pid;
    int arrival_time;
    int burst_time;
    int priority;
    ProcessState state;
    int remaining_time;
    int waiting_time;
    int turnaround_time;
    int memory_required;
    bool io_operations;

public:
    Process(int pid, int arrival, int burst, int priority, int memory, bool io_ops = false)
        : pid(pid), arrival_time(arrival), burst_time(burst), priority(priority), state(ProcessState::NEW), remaining_time(burst), waiting_time(0),
          turnaround_time(0), memory_required(memory), io_operations(io_ops) {}

    void updateState(ProcessState new_state) {
        state = new_state;
    }

    void executeOneUnit() {
        if (remaining_time > 0) {
            remaining_time--;
            if (remaining_time == 0)
                state = ProcessState::TERMINATED;
        }
    }

    int getPID() const { return pid; }
    int getArrivalTime() const { return arrival_time; }
    int getBurstTime() const { return burst_time; }
    int getRemainingTime() const { return remaining_time; }
    ProcessState getState() const { return state; }
    void setWaitingTime(int w) { waiting_time = w; }
    void setTurnaroundTime(int t) { turnaround_time = t; }

    void display() const {
        std::string state_str;
        switch (state) {
            case ProcessState::NEW: state_str = "NEW"; break;
            case ProcessState::READY: state_str = "READY"; break;
            case ProcessState::RUNNING: state_str = "RUNNING"; break;
            case ProcessState::WAITING: state_str = "WAITING"; break;
            case ProcessState::TERMINATED: state_str = "TERMINATED"; break;
        }

        std::cout << "PID: " << pid
                  << " | Arrival: " << arrival_time
                  << " | Burst: " << burst_time
                  << " | Remaining: " << remaining_time
                  << " | Priority: " << priority
                  << " | State: " << state_str
                  << std::endl;
    }
};

#endif
