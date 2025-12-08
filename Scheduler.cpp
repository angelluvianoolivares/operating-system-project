#include "Scheduler.h"

//-Constructor-
Scheduler::Scheduler() {}


//-This method contains the FCFS simulation logic-
// Updated to work on the "processes" copy it was given (4)
void Scheduler::runFCFS(std::vector<Process> processes) {

    // 1. Sorts the processes by arrival time.
    std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        if (a.getArrivalTime() != b.getArrivalTime()) {
            return a.getArrivalTime() < b.getArrivalTime();
        }
        return a.getPid() < b.getPid(); 
    });

    // 2. Runs the simulation
    int currentTime = 0;
    for (int i = 0; i < processes.size(); i++) {

        //-Handle idle time- (if CPU is waiting for a process to arrive)
        if (currentTime < processes[i].getArrivalTime()) {
            currentTime = processes[i].getArrivalTime();
        }

        //-Set states- (from our past attemp)
        processes[i].setState(STATE_RUNNING);

        //-Process runs-
        currentTime += processes[i].getBurstTime();

        //-Set completion time-
        processes[i].setCompletionTime(currentTime);

        //-Set final state-
        processes[i].setState(STATE_TERMINATED);
    }

    // 3. Prints the results
    printResults(processes, "First-Come, First-Served (FCFS)");
}

// -NEW METHOD- (4)
// This logic is adapted from your teammate's 'SJF_NonPreemptive' function
void Scheduler::runSJF(std::vector<Process> processes) {
    int n = processes.size();
    int currentTime = 0;
    int completed = 0;

    // This 'isCompleted' vector is a great way to track finished processes
    std::vector<bool> isCompleted(n, false);

    while (completed != n) {
        int shortest_burst = 99999; // A big number
        int shortest_index = -1;

        // Find the shortest job that has arrived and is not yet completed
        for (int i = 0; i < n; i++) {
            if (processes[i].getArrivalTime() <= currentTime && !isCompleted[i]) {
                if (processes[i].getBurstTime() < shortest_burst) {
                    shortest_burst = processes[i].getBurstTime();
                    shortest_index = i;
                }
                // Tie-breaking (by arrival time), just like your teammate's code
                if (processes[i].getBurstTime() == shortest_burst) {
                    if (processes[i].getArrivalTime() < processes[shortest_index].getArrivalTime()) {
                        shortest_index = i;
                    }
                }
            }
        }

        if (shortest_index == -1) {
            // No process is ready, so just move time forward
            currentTime++;
        }
        else {
            // A process is ready to run
            Process& p = processes[shortest_index];

            // Run the process to completion
            currentTime += p.getBurstTime();
            p.setCompletionTime(currentTime);
            p.setState(STATE_TERMINATED);
            isCompleted[shortest_index] = true;
            completed++;
        }
    }

    // Print the results
    printResults(processes, "Shortest Job First (SJF) Non-Preemptive");
}

//-This method will calculates and prints the final statistics table-
// Updated to accept the list of processes and a title (4)
void Scheduler::printResults(std::vector<Process>& processes, std::string algorithmName) {
    std::cout << "\n--- " << algorithmName << " ---" << std::endl;

    int total_turnaround_time = 0;
    int total_waiting_time = 0;


    // Sort by PID for a clean, final report
    std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.getPid() < b.getPid();
    });

    // 1. Print the table header (Make a table since more organized than a list format would look.)
    std::cout << "| PID | Arrival | Burst | Complete | Turnaround | Waiting |\n";


    // 2. Calculaes stats for each process and print its row
    for (int i = 0; i < processes.size(); i++) {
        //-Calculate stats-
        int tat = processes[i].getCompletionTime() - processes[i].getArrivalTime();
        int wt = tat - processes[i].getBurstTime();

        //-Store them back into the process object-
        processes[i].setTurnaroundTime(tat);
        processes[i].setWaitingTime(wt);

        //-Add to totals-
        total_turnaround_time += tat;
        total_waiting_time += wt;

        //-Print the row-
        std::cout << "| " << std::setw(3) << processes[i].getPid()
            << " | " << std::setw(7) << processes[i].getArrivalTime()
            << " | " << std::setw(5) << processes[i].getBurstTime()
            << " | " << std::setw(8) << processes[i].getCompletionTime()
            << " | " << std::setw(10) << processes[i].getTurnaroundTime()
            << " | " << std::setw(7) << processes[i].getWaitingTime() << " |\n";
    }

    // 3. Calculate and print the averages
    double avg_tat = (double)total_turnaround_time / processes.size();
    double avg_wt = (double)total_waiting_time / processes.size();

    std::cout << "\nAverage Turnaround Time: " << std::fixed << std::setprecision(2) << avg_tat << std::endl;
    std::cout << "Average Waiting Time:    " << avg_wt << std::endl;

}
