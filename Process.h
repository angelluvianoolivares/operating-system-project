#pragma once
#include <string>
#include <vector>   //(3)

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
    int state;
    // NEW: Instead of one integer, we have a list of cycles
    std::vector<int> burst_cycles;  // Pattern: [CPU, I/O, CPU, I/O, ...] 
    int current_burst_index;    // NEW: Tracks which burst we are currently on
    int current_burst_remaining; // NEW: Time remaining for the CURRENT burst only


    // -Statistics Attributes- (3)
    int completion_time;
    int turnaround_time;
    int waiting_time;

public:
    //-Methods-
    
    // 1. Constructor with parameters
    // Updated Constructor: Accepts a vector of bursts
    Process(int p_pid, int p_arrival_time, std::vector<int> p_burst_cycles);

    // 2. A method to update process state
    void setState(int newState);

    // 3. A method to decrement remaining execution time
    // Returns TRUE if the current burst finished, FALSE otherwise
    bool runFor(int time);

    // 4. Moves to the next burst in the list
    void moveToNextBurst();

    //-Getters-

    int getPid() const;
    int getArrivalTime() const;
    int getState() const;

    // Helper to get total CPU time (for stats)
    int getTotalCpuDuration() const;

    // Returns the remaining time for the CURRENT burst (needed for SRTF)
    int getCurrentBurstRemaining() const;

    // Returns the full duration of the CURRENT burst (needed for SJF)
    int getCurrentBurstDuration() const;

    // Checks if the current burst is a CPU burst (even index: 0, 2, 4...)
    bool isCpuBurst() const;

    // Checks if the process is completely finished (no more bursts)
    bool isFinished() const;

    // Stats Getters
    int getTurnaroundTime() const;
    int getWaitingTime() const;
    int getCompletionTime() const;

    //-Setters for new attributes- (3)
    void setCompletionTime(int time); 
    void setTurnaroundTime(int time); 
    void setWaitingTime(int time); 
};

// Helper function declaration for printing states
string stateToString(int state);
