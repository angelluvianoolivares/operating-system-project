#include <string>
#include <iostream>
#include "auth.h"
#include "Process.h" 
#include "Scheduler.h" //(3)

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

        // 1. Create a Scheduler instance
        Scheduler myScheduler;

        // 2. Create the processes (this is our test input)
        Process p1(1, 0, 5);
        Process p2(2, 2, 3);
        Process p3(3, 4, 1);
        Process p4(4, 5, 4);

        // 3. Add processes to the scheduler
        myScheduler.addProcess(p1);
        myScheduler.addProcess(p2);
        myScheduler.addProcess(p3);
        myScheduler.addProcess(p4);

        // 4. Run the FCFS simulation and print results
        myScheduler.runFCFS();
        
        cout << "===================================" << endl;
    }

    return 0;
}       