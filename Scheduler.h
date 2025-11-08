#pragma once

#include <vector>       // To hold a list of processes
#include <iostream>     // For printing
#include <iomanip>      // For formatting the output table
#include <algorithm>    // For sorting the processes
#include "Process.h"    // Our 'Process' class

class Scheduler {
public:
    // -Constructor- (initializes our scheduler)
    Scheduler();

    //-Method to add a new process to our list-
    void addProcess(Process p);

    //-Runs the FCFS simulation-
    void runFCFS();

private:
    //-A list to hold all the processes for the simulation-
    std::vector<Process> processes;

    //-To keep track of simulation time we use a clock-
    int currentTime;

    //-A helper method to print the results table-
    void printResults();
};