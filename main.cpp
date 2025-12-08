#include <string>
#include <iostream>
#include "auth.h"
#include "Process.h" 
#include "Scheduler.h" //(3)
#include <vector> //Needed to create a list (4)

using namespace std; 

int main()
{
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

        //-ASSIGNMENT PART 3: SCHEDULING TEST-
        cout << "\n===================================" << endl;
        cout << "--- Running Process Scheduler ---" << endl;

        // 1. Create ONE Scheduler instance
        Scheduler myScheduler;

        // 2. Create ONE "master list" of processes
        std::vector<Process> myProcesses;
        myProcesses.push_back(Process(1, 0, 8));
        myProcesses.push_back(Process(2, 1, 4));
        myProcesses.push_back(Process(3, 2, 9));
        myProcesses.push_back(Process(4, 3, 5));

        // 3. Add processes to the scheduler
        myProcesses.clear(); // Clear the list
        myProcesses.push_back(Process(1, 2, 3));
        myProcesses.push_back(Process(2, 4, 2));
        myProcesses.push_back(Process(3, 5, 1));
        myProcesses.push_back(Process(4, 7, 4));
        myProcesses.push_back(Process(5, 9, 2));
        myProcesses.push_back(Process(6, 15, 6));
        myProcesses.push_back(Process(7, 16, 8));

        // 4. Run the Algorithms and print results
        myScheduler.runFCFS(myProcesses);
        myScheduler.runSJF(myProcesses);
        myScheduler.runSRTF(myProcesses);
        cout << "===================================" << endl;
    }

    return 0;
}       
