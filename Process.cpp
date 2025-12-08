#include "Process.h"

using namespace std;

// 1. Constructor implementation 
Process::Process(int p_pid, int p_arrival_time, std::vector<int> p_burst_cycles) {
    pid = p_pid;
    arrival_time = p_arrival_time;
    burst_cycles = p_burst_cycles; // Stores the list

    state = STATE_NEW;

    // Starts at the first burst (Index 0)
    current_burst_index = 0;

    // Sets remaining time to the full duration of the first burst
    if (!burst_cycles.empty()) {
        current_burst_remaining = burst_cycles[0];
    }
    else {
        current_burst_remaining = 0;
    }

    // Initialize new stats to 0 (3)
    completion_time = 0;
    turnaround_time = 0;
    waiting_time = 0;
}

// 2. Method to update the process state
void Process::setState(int newState) {
    state = newState;
}

// 3. Updated runFor method (3)
// Returns true if the burst finished, false if it still has time left
bool Process::runFor(int time) {
    if (time >= current_burst_remaining) {
        current_burst_remaining = 0;
        return true; // Burst Finished
    }
    else {
        current_burst_remaining -= time;
        return false; // Still running
    }
}

// 4. Moves to the next burst 
void Process::moveToNextBurst() {
    current_burst_index++; // Go to next index

    if (current_burst_index < burst_cycles.size()) {
        // Loads the time for the new burst
        current_burst_remaining = burst_cycles[current_burst_index];
    }
    else {
        // No more bursts left
        current_burst_remaining = 0;
    }
}

//-Getter implementations-
int Process::getPid() const { return pid; }
int Process::getArrivalTime() const { return arrival_time; }
int Process::getState() const { return state; }
int Process::getCurrentBurstRemaining() const { return current_burst_remaining; }

int Process::getCurrentBurstDuration() const {
    if (current_burst_index < burst_cycles.size()) {
        return burst_cycles[current_burst_index];
    }
    return 0;
}

// Sums up all the CPU bursts (even indices)
int Process::getTotalCpuDuration() const {
    int total = 0;
    for (size_t i = 0; i < burst_cycles.size(); i++) {
        // Even indices (0, 2, 4...) are CPU bursts
        if (i % 2 == 0) {
            total += burst_cycles[i];
        }
    }
    return total;
}

// Even indices (0, 2, 4...) are CPU. Odd indices (1, 3, 5...) are I/O.
bool Process::isCpuBurst() const {
    return (current_burst_index % 2 == 0);
}

bool Process::isFinished() const {
    return (current_burst_index >= burst_cycles.size());
}

// Stats Getters/Setters (3)
int Process::getTurnaroundTime() const { return turnaround_time; }  //(3)
int Process::getWaitingTime() const { return waiting_time; }    //(3)
int Process::getCompletionTime() const { return completion_time; }  //(3)
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
