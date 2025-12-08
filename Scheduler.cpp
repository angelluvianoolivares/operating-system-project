#include "Scheduler.h"
#include <cstdlib> // For random address generation

//-Constructor-
Scheduler::Scheduler() {}


//-Helper: Handles the Waiting Queue (I/O)-
// This method iterates through processes in the waiting queue and runs their I/O bursts (3)
void Scheduler::handleWaitingQueue(std::vector<Process*>& waitingQueue, std::vector<Process*>& readyQueue) {

    // 1. Goes through every process in the waiting queue
    for (int i = 0; i < waitingQueue.size(); i++) {
        Process* p = waitingQueue[i];

        // 2. Runs I/O for 1 tick
        // runFor returns TRUE if the burst finished
        bool burstFinished = p->runFor(1);

        if (burstFinished) {
            p->moveToNextBurst();

            // 3. Checks if process is fully finished or just the I/O burst
            if (p->isFinished()) {
                p->setState(STATE_TERMINATED);
                // Removes from waiting queue
                waitingQueue.erase(waitingQueue.begin() + i);
                i--; // Adjusts index since we removed an element
            }
            else {
                // I/O done, moves back to Ready Queue for next CPU burst
                p->setState(STATE_READY);
                readyQueue.push_back(p);
                // Removes from waiting queue
                waitingQueue.erase(waitingQueue.begin() + i);
                i--; // Adjust index
            }
        }
    }
}


//-This method contains the FCFS simulation logic-
// Updated to support I/O concurrency and Waiting Queues (3)
void Scheduler::runFCFS(std::vector<Process> processes) {
    int currentTime = 0;
    int completed = 0;

    //-Queues use pointers to the main 'processes' vector-
    std::vector<Process*> readyQueue;
    std::vector<Process*> waitingQueue;
    Process* cpuProcess = nullptr; // The process currently on CPU

    // 1. Sorts the processes by arrival time initially
    std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.getArrivalTime() < b.getArrivalTime();
    });

    // 2. Main Simulation Loop (Tick-by-Tick)
    while (completed != processes.size()) {

        //-Check for new arrivals-
        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].getArrivalTime() == currentTime) {
                processes[i].setState(STATE_READY);
                readyQueue.push_back(&processes[i]);
            }
        }

        //-Handle I/O (Waiting Queue)-
        handleWaitingQueue(waitingQueue, readyQueue);

        //-Handle CPU Execution-
        if (cpuProcess != nullptr) {
            // Run CPU for 1 tick
            bool burstFinished = cpuProcess->runFor(1);

            // --- VIRTUAL MEMORY SIMULATION (NEW) ---
            // Randomly simulate a memory instruction (approx 25% of the time)
            if (rand() % 4 == 0) {
                int virtualAddr = rand() % (1024 * 1024); // Random 0 to 1MB
                int physicalAddr = mmu.translate(virtualAddr);
                
                if (physicalAddr != -1) {
                     std::cout << "[MMU] Process " << cpuProcess->getPid() << ": " 
                               << "Virt: " << virtualAddr << " -> Phys: " << physicalAddr << std::endl;
                } else {
                     // Simulate Page Fault Handling
                     // std::cout << "[MMU] Page Fault! Mapping page..." << std::endl;
                     int vpn = virtualAddr >> 12; // 12 is OFFSET_BITS
                     mmu.mapPage(vpn);
                }
            }
            // ---------------------------------------

            if (burstFinished) {
                cpuProcess->moveToNextBurst();

                if (cpuProcess->isFinished()) {
                    cpuProcess->setCompletionTime(currentTime);
                    cpuProcess->setState(STATE_TERMINATED);
                    completed++;
                    cpuProcess = nullptr; // CPU is free
                }
                else {
                    // Burst done, but process not finished -> Must be I/O next
                    cpuProcess->setState(STATE_WAITING);
                    waitingQueue.push_back(cpuProcess);
                    cpuProcess = nullptr; // CPU is free
                }
            }
        }

        //-Scheduler Decision (FCFS)-
        // If CPU is free, pick next process from Ready Queue
        if (cpuProcess == nullptr && !readyQueue.empty()) {
            cpuProcess = readyQueue.front(); // Pick first in line
            readyQueue.erase(readyQueue.begin()); // Remove from line
            cpuProcess->setState(STATE_RUNNING);
        }

        currentTime++;
    }

    printResults(processes, "First-Come, First-Served (FCFS)");
}


//-This method contains the SJF simulation logic-
// Updated to support I/O concurrency (3)
void Scheduler::runSJF(std::vector<Process> processes) {
    int currentTime = 0;
    int completed = 0;

    std::vector<Process*> readyQueue;
    std::vector<Process*> waitingQueue;
    Process* cpuProcess = nullptr;

    // 1. Sorts by arrival time initially
    std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.getArrivalTime() < b.getArrivalTime();
    });

    // 2. Main Simulation Loop
    while (completed != processes.size()) {

        //-Check for new arrivals-
        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].getArrivalTime() == currentTime) {
                processes[i].setState(STATE_READY);
                readyQueue.push_back(&processes[i]);
            }
        }

        //-Handle I/O (Waiting Queue)-
        handleWaitingQueue(waitingQueue, readyQueue);

        //-Handle CPU Execution-
        if (cpuProcess != nullptr) {
            bool burstFinished = cpuProcess->runFor(1);

            // --- VIRTUAL MEMORY SIMULATION (NEW) ---
            if (rand() % 4 == 0) {
                int virtualAddr = rand() % (1024 * 1024); 
                int physicalAddr = mmu.translate(virtualAddr);
                
                if (physicalAddr == -1) {
                     int vpn = virtualAddr >> 12;
                     mmu.mapPage(vpn);
                }
            }
            // ---------------------------------------

            if (burstFinished) {
                cpuProcess->moveToNextBurst();

                if (cpuProcess->isFinished()) {
                    cpuProcess->setCompletionTime(currentTime);
                    cpuProcess->setState(STATE_TERMINATED);
                    completed++;
                    cpuProcess = nullptr;
                }
                else {
                    cpuProcess->setState(STATE_WAITING);
                    waitingQueue.push_back(cpuProcess);
                    cpuProcess = nullptr;
                }
            }
        }

        //-Scheduler Decision (SJF)-
        // If CPU is free, find process with shortest CURRENT burst
        if (cpuProcess == nullptr && !readyQueue.empty()) {
            int shortestIndex = -1;
            int shortestBurst = 99999;

            for (int i = 0; i < readyQueue.size(); i++) {
                if (readyQueue[i]->getCurrentBurstDuration() < shortestBurst) {
                    shortestBurst = readyQueue[i]->getCurrentBurstDuration();
                    shortestIndex = i;
                }
            }

            cpuProcess = readyQueue[shortestIndex];
            readyQueue.erase(readyQueue.begin() + shortestIndex);
            cpuProcess->setState(STATE_RUNNING);
        }

        currentTime++;
    }

    printResults(processes, "Shortest Job First (SJF)");
}


//-This method contains the SRTF simulation logic-
// Updated to support I/O concurrency and Preemption (3)
void Scheduler::runSRTF(std::vector<Process> processes) {
    int currentTime = 0;
    int completed = 0;

    std::vector<Process*> readyQueue;
    std::vector<Process*> waitingQueue;
    Process* cpuProcess = nullptr;

    // 1. Sorts by arrival time initially
    std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.getArrivalTime() < b.getArrivalTime();
    });

    // 2. Main Simulation Loop
    while (completed != processes.size()) {

        //-Check for new arrivals-
        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].getArrivalTime() == currentTime) {
                processes[i].setState(STATE_READY);
                readyQueue.push_back(&processes[i]);
            }
        }

        //-Handle I/O (Waiting Queue)-
        handleWaitingQueue(waitingQueue, readyQueue);

        //-Preemption Logic (SRTF Specific)-
        // If CPU is busy, check if anyone in Ready Queue is shorter
        if (cpuProcess != nullptr) {
            for (int i = 0; i < readyQueue.size(); i++) {
                if (readyQueue[i]->getCurrentBurstRemaining() < cpuProcess->getCurrentBurstRemaining()) {

                    // Preempt!
                    cpuProcess->setState(STATE_READY);
                    readyQueue.push_back(cpuProcess); // Put back in ready queue

                    cpuProcess = readyQueue[i]; // Take the new shorter one
                    readyQueue.erase(readyQueue.begin() + i);
                    cpuProcess->setState(STATE_RUNNING);
                    break; // Only switch once per tick
                }
            }
        }

        //-Handle CPU Execution-
        if (cpuProcess != nullptr) {
            bool burstFinished = cpuProcess->runFor(1);

            // --- VIRTUAL MEMORY SIMULATION (NEW) ---
            if (rand() % 4 == 0) {
                int virtualAddr = rand() % (1024 * 1024); 
                int physicalAddr = mmu.translate(virtualAddr);
                
                if (physicalAddr == -1) {
                     int vpn = virtualAddr >> 12;
                     mmu.mapPage(vpn);
                }
            }
            // ---------------------------------------

            if (burstFinished) {
                cpuProcess->moveToNextBurst();

                if (cpuProcess->isFinished()) {
                    cpuProcess->setCompletionTime(currentTime);
                    cpuProcess->setState(STATE_TERMINATED);
                    completed++;
                    cpuProcess = nullptr;
                }
                else {
                    cpuProcess->setState(STATE_WAITING);
                    waitingQueue.push_back(cpuProcess);
                    cpuProcess = nullptr;
                }
            }
        }

        //-Scheduler Decision (If CPU is free)-
        if (cpuProcess == nullptr && !readyQueue.empty()) {
            int shortestIndex = -1;
            int shortestTime = 99999;

            for (int i = 0; i < readyQueue.size(); i++) {
                if (readyQueue[i]->getCurrentBurstRemaining() < shortestTime) {
                    shortestTime = readyQueue[i]->getCurrentBurstRemaining();
                    shortestIndex = i;
                }
            }

            cpuProcess = readyQueue[shortestIndex];
            readyQueue.erase(readyQueue.begin() + shortestIndex);
            cpuProcess->setState(STATE_RUNNING);
        }

        currentTime++;
    }

    printResults(processes, "Shortest Remaining Time First (SRTF)");
}


//-This method calculates and prints the final statistics table-
void Scheduler::printResults(std::vector<Process>& processes, std::string algorithmName) {
    std::cout << "\n--- " << algorithmName << " ---" << std::endl;

    int total_turnaround_time = 0;
    int total_waiting_time = 0;

    //-Prints the table header-
    std::cout << "| PID | Arrival | Total CPU | Complete | Turnaround | Waiting |\n";
    std::cout << "|-----|---------|-----------|----------|------------|---------|\n";

    // 1. Sort by PID for a clean report
    std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.getPid() < b.getPid();
    });

    // 2. Loop through processes to calculate and print stats
    for (int i = 0; i < processes.size(); i++) {
        int tat = processes[i].getCompletionTime() - processes[i].getArrivalTime();
        int total_cpu = processes[i].getTotalCpuDuration();
        int wt = tat - total_cpu;

        // Ensure non-negative (safety check)
        if (wt < 0) wt = 0;

        processes[i].setTurnaroundTime(tat);
        processes[i].setWaitingTime(wt);

        total_turnaround_time += tat;
        total_waiting_time += wt;

        std::cout << "| " << std::setw(3) << processes[i].getPid()
            << " | " << std::setw(7) << processes[i].getArrivalTime()
            << " | " << std::setw(9) << total_cpu
            << " | " << std::setw(8) << processes[i].getCompletionTime()
            << " | " << std::setw(10) << processes[i].getTurnaroundTime()
            << " | " << std::setw(7) << processes[i].getWaitingTime() << " |\n";
    }

    //-Calculate and Print Averages-
    double avg_tat = (double)total_turnaround_time / processes.size();
    double avg_wt = (double)total_waiting_time / processes.size();

    std::cout << "\nAverage Turnaround Time: " << std::fixed << std::setprecision(2) << avg_tat << std::endl;
    std::cout << "Average Waiting Time:    " << avg_wt << std::endl;
    std::cout << "---------------------------------------------------------" << std::endl;
}
