#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <iostream>
using namespace std;

enum class ProcessState{ NEW, READY, RUNNING, WAITING, TERMINATED };

class Process{
    private:
        int pid;
        int arrival_time;
        int burst_time;
        int priority;
        int remaining_time;
        int waiting_time;
        int turnaround_time;
        int completion_time;
        int memory_required;
        bool io_operations;
        ProcessState state;
    
    public:
        Process(int pid, int arrival, int burst, int priority, int memory, bool io_ops = false)
            : pid(pid), arrival_time(arrival), burst_time(burst), priority(priority), remaining_time(burst),
              waiting_time(0), turnaround_time(0), completion_time(0), memory_required(memory), io_operations(io_ops),
              state(ProcessState::NEW) {}
        
        void updateState(ProcessState new_state){
            state = new_state;
        }

        void executeOneUnit(){
            if (remaining_time > 0){
                remaining_time--;
                if (remaining_time == 0){
                    state = ProcessState::TERMINATED;
                }
            }
        }

        //Getters
        int getPID() const { return pid; }
        int getArrivalTime() const { return arrival_time; }
        int getBurstTime() const { return burst_time; }
        int getRemainingTime() const { return remaining_time; }
        int getPriority() const { return priority; }
        int getWaitingTime() const { return waiting_time; }
        int getTurnaroundTime() const { return turnaround_time; }
        int getCompletionTime() const { return completion_time; }
        ProcessState getState() const { return state; }

        //Setters
        void setWaitingTime(int w) { waiting_time = w; }
        void setTurnaroundTime(int t) { turnaround_time = t; }
        void setCompletionTime(int c) { completion_time = c; }
        void setRemainingTime(int r) { remaining_time = r; }

        void display() const{
            string state_str;
            
            switch(state){
                case ProcessState::NEW:
                    state_str = "NEW";
                    break;
                case ProcessState::READY:
                    state_str = "READY";
                    break;
                case ProcessState::RUNNING:
                    state_str = "RUNNING";
                    break;
                case ProcessState::WAITING:
                    state_str = "WAITING";
                    break;
                case ProcessState::TERMINATED:
                    state_str = "TERMINATED";
                    break;
            }

            cout << "PID: " << pid
                 << "Arrival: " << arrival_time
                 << "Burst: " << burst_time
                 << "Remaining: " << remaining_time
                 << "Priority: " << priority
                 << "State: " << state_str;
            
            if (state == ProcessState::TERMINATED){
                cout << "Waiting Time: " << waiting_time
                     << "Turnaround Time: " << turnaround_time
                     << "Completion Time: " << completion_time;
            }
            cout << endl;
        }
};

#endif