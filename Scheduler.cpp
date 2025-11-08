#include "Scheduler.h"

//-Constructor-
Scheduler::Scheduler() {
    currentTime = 0;
}

//-Method to add a process to the internal list
void Scheduler::addProcess(Process p) {
    processes.push_back(p);
}

//-This method contains the FCFS simulation logic-
void Scheduler::runFCFS() {
    std::cout << "--- First-Come, First-Served (FCFS) ---" << std::endl;


    // 1. Sorts the processes by arrival time.
    std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        if (a.getArrivalTime() != b.getArrivalTime()) {
            return a.getArrivalTime() < b.getArrivalTime();
        }
        return a.getPid() < b.getPid(); 
    });

    // 2. Runs the simulation
    currentTime = 0;
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
    printResults();
}

//-This method will calculates and prints the final statistics table-
void Scheduler::printResults() {
    int total_turnaround_time = 0;
    int total_waiting_time = 0;

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