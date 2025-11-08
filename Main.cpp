#include <string>
#include <iostream>
#include <vector>
#include <limits>
#include <iomanip>
#include "Auth.h"
#include "Process.h"
#include "Scheduler.h"
using namespace std;

void displayMenu(){
    cout << "CPU SCHEDULING MENU\n";
    cout << "1. Use Default Process Set\n";
    cout << "2. Enter Custome Process\n";
    cout << "3. Run all Algorithms (Comparison)\n";
    cout << "4. Run FSFS Only\n";
    cout << "5. Run SJF Only\n";
    cout << "6. Run Round Robin Only\n";
    cout << "7. Exit\n";
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

        if (cin.fail() || n <= 0){
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
    cout << "PROCESS DETAILS\n";
    cout << left << setw(8) << "PID"
         << setw(12) << "Arrival"
         << setw(12) << "Burst"
         << setw(12) << "Priority" << "\n";
    cout << "--------------------------------------------\n";

    for (const auto& p : processes){
        cout << left << setw(8) << p.getPID()
             << setw(12) << p.getArrivalTime()
             << setw(12) << p.getBurstTime()
             << setw(12) << p.getPriority() << "\n";
    }
    cout << "\n";
}

int main(){
    cout << "OPERATING SYSTEM SIMULATOR\n";
    cout << "OS is Booting Up....." << endl;
    cout << "Please Wait!" << endl << endl;

    bool isAuthenticated = authenticateUser();
    if (!isAuthenticated){
        cout << "\nIncorrect Information. Login Denied.\n";
        return 1;
    }

    cout << "\nLogin Successful!\n";

    vector<Process> processes;
    bool useDefault = true;
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
                useDefault = true;
                displayProcesses(processes);
                cout << "Default Process Set Loaded Successfully!\n";
                break;
            case 2:
                processes = getCustomProcesses();
                useDefault = false;
                displayProcesses(processes);
                break;
            case 3:
                if (processes.empty()){
                    cout << "No Process Loaded. Loading Default Processes...\n";
                    processes = getDefaultProcesses();
                    displayProcesses(processes);
                }

                {
                    Scheduler scheduler(processes);
                    int quantum;

                    cout << "Enter Time Quantum for Round Robin: ";
                    cin >> quantum;
                    if (cin.fail() || quantum <= 0){
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
                        cout << "Invalid Input. Using Default Quantum.\n";
                    }
                    scheduler.compareAlgorithms(quantum);
                }
                break;
            case 4:
                if (processes.empty()){
                    cout << "No Process Loaded. Loading Default Processes...\n";
                    processes = getDefaultProcesses();
                    displayProcesses(processes);
                }
                {
                    Scheduler scheduler(processes);
                    scheduler.scheduleFCFS();
                }
                break;
            case 5:
                if (processes.empty()){
                    cout << "No Process Loaded. Loading Default Processes...\n";
                    processes = getDefaultProcesses();
                    displayProcesses(processes);
                }
                {
                    Scheduler scheduler(processes);
                    scheduler.scheduleSJF();
                }
                break;
            case 6:
                if (processes.empty()){
                    cout << "No Process Loaded. Loading Default Processes...\n";
                    processes = getDefaultProcesses();
                    displayProcesses(processes);
                }
                {
                    Scheduler scheduler(processes);
                    int quantum;

                    cout << "Enter Time Quantum for Round Robin: ";
                    cin >> quantum;
                    if (cin.fail() || quantum <= 0){
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
                        cout << "Invalid Input. Using Default Quantum.\n";
                    }
                    scheduler.scheduleRoundRobin(quantum);
                }
                break;
            case 7:
                return 0;
            default:
                cout << "Invalid Choice. Please Try Again.\n";
        }
    }
    
    return 0;
}