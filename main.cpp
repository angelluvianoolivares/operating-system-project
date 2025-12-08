#include <string>
#include <iostream>
#include <vector>
#include <cstdlib> // For random numbers
#include <ctime>   // For time seed
#include "auth.h"
#include "Process.h" 
#include "Scheduler.h"

using namespace std;

//-Helper to generate random processes as per assignment instructions-
vector<Process> generateRandomProcesses(int count) {
    vector<Process> processes;
    int currentID = 1;

    for (int i = 0; i < count; i++) {
        // Random arrival time between 0 and 20
        int arrival = rand() % 20;

        // Generate a random burst sequence: CPU -> IO -> CPU
        // We ensure it has 3 parts so we can test the Waiting Queue logic
        int cpu1 = 1 + (rand() % 10); // Random CPU burst 1-10
        int io = 1 + (rand() % 5);  // Random IO burst 1-5
        int cpu2 = 1 + (rand() % 10); // Random CPU burst 1-10

        // Create the sequence {CPU, IO, CPU}
        vector<int> bursts = { cpu1, io, cpu2 };

        processes.push_back(Process(currentID, arrival, bursts));
        currentID++;
    }
    return processes;
}

int main()
{
    // Seed the random generator
    srand(time(0));

    cout << "OS is booting up....." << endl;
    cout << "Please wait! " << endl;

    bool isAuthenticated = authenticateUser();

    if (!isAuthenticated)
    {
        cout << "Incorrect Information. Login Denied" << endl;
    }
    else
    {
        cout << "Login Succesful!" << endl;

        //-ASSIGNMENT PART 3: SCHEDULING WITH I/O-
        cout << "\n===================================" << endl;
        cout << "--- Running Process Scheduler ---" << endl;

        Scheduler myScheduler;

        // 1. Generate Random Processes (as required)
        cout << "\nGenerating 5 random processes with {CPU, IO, CPU} bursts..." << endl;
        vector<Process> myProcesses = generateRandomProcesses(5);

        // Print what was generated so you can verify the results
        for (const auto& p : myProcesses) {
            cout << "Created P" << p.getPid() << " Arrival: " << p.getArrivalTime() << endl;
        }

        // 2. Run the Algorithms
        // The Scheduler will now handle the I/O bursts using the Waiting Queue
        myScheduler.runFCFS(myProcesses);
        myScheduler.runSJF(myProcesses);
        myScheduler.runSRTF(myProcesses);

        cout << "===================================" << endl;
    }

    return 0;
}
