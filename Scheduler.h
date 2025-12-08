#pragma once

#include <vector>       // To hold a list of processes
#include <iostream>     // For printing
#include <iomanip>      // For formatting the output table
#include <algorithm>    // For sorting the processes
#include "Process.h"    // Our 'Process' class
#include <string>       // Added this for string

class Scheduler {
public:
    // -Constructor- (initializes our scheduler)
    Scheduler();

    /* Instead of the Scheduler holding on to one list (which gets ruined when using an algorithm),
    *  we make our run function accept a list of processes.)
    void addProcess(Process p);
    */

    //-Runs the FCFS simulation on a given list of processes-
    void runFCFS(std::vector<Process> processes);

    //-Runs the SJF simulation on a given list of processes- (4)
    void runSJF(std::vector<Process> processes);

private:
    /*
    //-A list to hold all the processes for the simulation-
    std::vector<Process> processes;

    //-To keep track of simulation time we use a clock-
    int currentTime;
    */

    //-A helper method to print the list and tittle-
    void printResults(std::vector<Process>& processes, std::string algorithmName);
};
