#include <string>
#include <iostream>
#include <vector>
#include <limits>
#include <iomanip>
#include "Auth.h"
#include "Process.h"
#include "Scheduler.h"
#include "MemoryManager.h"
using namespace std;

void displayMenu(){
    cout << "\n========== CPU SCHEDULING MENU ==========\n";
    cout << "1. Use Default Process Set\n";
    cout << "2. Enter Custom Process\n";
    cout << "3. Run all Algorithms (Comparison)\n";
    cout << "4. Run FCFS Only\n";
    cout << "5. Run SJF Only\n";
    cout << "6. Run Round Robin Only\n";
    cout << "7. View Page Table for Process\n";
    cout << "8. Simulate Memory Access for Process\n";
    cout << "9. View Memory Statistics\n";
    cout << "10. Exit\n";
    cout << "=========================================\n";
    cout << "Enter your Choice: ";
}

vector<Process> getDefaultProcesses(){
    vector<Process> processes;

    processes.push_back(Process(1, 0, 5, 2, 256));
    processes.push_back(Process(2, 1, 3, 1, 128));
    processes.push_back(Process(3, 2, 8, 3, 512));
    processes.push_back(Process(4, 3, 6, 3, 256));
    processes.push_back(Process(5, 4, 4, 1, 128));

    return processes;
}

vector<Process> getCustomProcesses(){
    int n;
    cout << "Enter Number of Processes: ";
    cin >> n;

    if (cin.fail() || n <= 0){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        cout << "Invalid Input. Using Default Processes.\n";
        return getDefaultProcesses();
    }

    vector<Process> processes;

    cout << "\nEnter Process Details:\n";
    for (int i = 0; i < n; i++){
        int pid, arrival, burst, priority, memory;

        cout << "Process " << (i + 1) << ":\n";
        cout << "Process ID: ";
        cin >> pid;
        cout << "Arrival Time: ";
        cin >> arrival;
        cout << "Burst Time: ";
        cin >> burst;
        cout << "Priority: ";
        cin >> priority;
        cout << "Memory (KB): ";
        cin >> memory;

        if (cin.fail() || burst <= 0 || memory <= 0){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
            cout << "Invalid Input. Skipping this Process.\n";
            continue;
        }

        processes.push_back(Process(pid, arrival, burst, priority, memory));
    }

    if (processes.empty()){
        cout << "No Valid Processes Entered. Using Default Processes.\n";
        return getDefaultProcesses();
    }

    return processes;
}

void displayProcesses(const vector<Process>& processes){
    cout << "\n========== PROCESS DETAILS ==========\n";
    cout << left << setw(8) << "PID"
         << setw(12) << "Arrival"
         << setw(12) << "Burst"
         << setw(12) << "Priority"
         << setw(12) << "Memory(KB)" << "\n";
    cout << "-------------------------------------------------------\n";

    for (const auto& p : processes){
        cout << left << setw(8) << p.getPID()
             << setw(12) << p.getArrivalTime()
             << setw(12) << p.getBurstTime()
             << setw(12) << p.getPriority()
             << setw(12) << p.getMemoryRequired() << "\n";
    }
    cout << "=====================================\n\n";
}

void allocateMemoryForProcesses(MemoryManager& memMgr, const vector<Process>& processes){
    cout << "\n========== MEMORY ALLOCATION ==========\n";
    for (const auto& p : processes){
        memMgr.allocateProcess(p.getPID(), p.getMemoryRequired());
    }
    cout << "=======================================\n";
}

void simulateSchedulingWithMemory(MemoryManager& memMgr, const vector<Process>& processes){
    cout << "\nSimulating Memory Access During Process Execution...\n";
    for (const auto& p : processes){
        cout << "\n--- Process " << p.getPID() << " Executing ---\n";
        memMgr.simulateProcessMemoryAccess(p.getPID(), 3);
    }
}

int main(){
    cout << "======================================\n";
    cout << "   OPERATING SYSTEM SIMULATOR\n";
    cout << "======================================\n";
    cout << "OS is Booting Up....." << endl;
    cout << "Please Wait!" << endl << endl;

    bool isAuthenticated = authenticateUser();
    if (!isAuthenticated){
        cout << "\nIncorrect Information. Login Denied.\n";
        return 1;
    }

    cout << "\n✓ Login Successful!\n";
    cout << "======================================\n";

    // Create Memory Manager
    MemoryManager memoryManager;

    vector<Process> processes;
    bool memoryAllocated = false;
    int choice;

    while (true){
        displayMenu();
        cin >> choice;

        if (cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
            cout << "Invalid Input. Please Enter a Number.\n";
            continue;
        }

        switch (choice){
            case 1:
                processes = getDefaultProcesses();
                displayProcesses(processes);
                allocateMemoryForProcesses(memoryManager, processes);
                memoryAllocated = true;
                cout << "✓ Default Process Set Loaded Successfully!\n";
                break;
                
            case 2:
                processes = getCustomProcesses();
                displayProcesses(processes);
                allocateMemoryForProcesses(memoryManager, processes);
                memoryAllocated = true;
                break;
                
            case 3:
                if (processes.empty()){
                    cout << "No Process Loaded. Loading Default Processes...\n";
                    processes = getDefaultProcesses();
                    displayProcesses(processes);
                    allocateMemoryForProcesses(memoryManager, processes);
                    memoryAllocated = true;
                }

                {
                    Scheduler scheduler(processes);
                    int quantum;

                    cout << "Enter Time Quantum for Round Robin: ";
                    cin >> quantum;
                    if (cin.fail() || quantum <= 0){
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        quantum = 2;
                        cout << "Invalid Input. Using Default Quantum = 2.\n";
                    }
                    
                    simulateSchedulingWithMemory(memoryManager, processes);
                    scheduler.compareAlgorithms(quantum);
                    memoryManager.displayMemoryStats();
                }
                break;
                
            case 4:
                if (processes.empty()){
                    cout << "No Process Loaded. Loading Default Processes...\n";
                    processes = getDefaultProcesses();
                    displayProcesses(processes);
                    allocateMemoryForProcesses(memoryManager, processes);
                    memoryAllocated = true;
                }
                {
                    Scheduler scheduler(processes);
                    simulateSchedulingWithMemory(memoryManager, processes);
                    scheduler.scheduleFCFS();
                    memoryManager.displayMemoryStats();
                }
                break;
                
            case 5:
                if (processes.empty()){
                    cout << "No Process Loaded. Loading Default Processes...\n";
                    processes = getDefaultProcesses();
                    displayProcesses(processes);
                    allocateMemoryForProcesses(memoryManager, processes);
                    memoryAllocated = true;
                }
                {
                    Scheduler scheduler(processes);
                    simulateSchedulingWithMemory(memoryManager, processes);
                    scheduler.scheduleSJF();
                    memoryManager.displayMemoryStats();
                }
                break;
                
            case 6:
                if (processes.empty()){
                    cout << "No Process Loaded. Loading Default Processes...\n";
                    processes = getDefaultProcesses();
                    displayProcesses(processes);
                    allocateMemoryForProcesses(memoryManager, processes);
                    memoryAllocated = true;
                }
                {
                    Scheduler scheduler(processes);
                    int quantum;

                    cout << "Enter Time Quantum for Round Robin: ";
                    cin >> quantum;
                    if (cin.fail() || quantum <= 0){
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        quantum = 2;
                        cout << "Invalid Input. Using Default Quantum = 2.\n";
                    }
                    
                    simulateSchedulingWithMemory(memoryManager, processes);
                    scheduler.scheduleRoundRobin(quantum);
                    memoryManager.displayMemoryStats();
                }
                break;
                
            case 7:
                {
                    int pid;
                    cout << "Enter Process ID to view page table: ";
                    cin >> pid;
                    
                    if (cin.fail()){
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid Input.\n";
                    } else {
                        memoryManager.displayPageTable(pid);
                    }
                }
                break;
                
            case 8:
                {
                    int pid, num_accesses;
                    cout << "Enter Process ID: ";
                    cin >> pid;
                    cout << "Enter Number of Memory Accesses to Simulate: ";
                    cin >> num_accesses;
                    
                    if (cin.fail() || num_accesses <= 0){
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid Input.\n";
                    } else {
                        memoryManager.simulateProcessMemoryAccess(pid, num_accesses);
                    }
                }
                break;
                
            case 9:
                memoryManager.displayMemoryStats();
                break;
                
            case 10:
                cout << "\nShutting down OS Simulator...\n";
                cout << "Goodbye!\n";
                return 0;
                
            default:
                cout << "Invalid Choice. Please Try Again.\n";
        }
    }
    
    return 0;
}
