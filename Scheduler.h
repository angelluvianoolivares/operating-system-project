#pragma once

#include <vector>       // To hold a list of processes
#include <iostream>     // For printing
#include <iomanip>      // For formatting the output table
#include <algorithm>    // For sorting the processes
#include "Process.h"    // Our 'Process' class
#include <string>       // Added this for string (3)

class Scheduler {
public:
    // -Constructor- (initializes our scheduler)
    Scheduler();

    //-Runs the FCFS simulation on a given list of processes-
    void runFCFS(std::vector<Process> processes);

    //-Runs the SJF simulation on a given list of processes- (3)
    void runSJF(std::vector<Process> processes);

    //-Runs the SRTF simulation on a given list of processes- (3)
    void runSRTF(std::vector<Process> processes); 

private:

    //-A helper method to print the list and tittle-
    void printResults(std::vector<Process>& processes, std::string algorithmName);
};
