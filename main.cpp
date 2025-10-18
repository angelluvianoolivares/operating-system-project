#include <string>
#include <iostream>
#include "auth.h"
#include "Process.h" 

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

        //-ASSIGNMENT PART 2: SIMPLE TEST CASE-

        cout << "\n===================================" << endl;
        cout << "--- Running Process Class Test Case ---" << endl;

        // 1. Create a process using our new constructor
        Process p1(1, 0, 5); // pid=1, arrival=0, burst=5
        
        cout << "\nCreated Process (PID: " << p1.getPid() << ")" << endl;
        cout << "  Initial State: " << stateToString(p1.getState()) << endl; // Should be NEW (0)
        cout << "  Initial Remaining Time: " << p1.getRemainingTime() << endl; // Should be 5

        // 2. Demonstratng updating the process state
        cout << "\nChanging state from NEW to READY..." << endl;
        p1.setState(STATE_READY);
        cout << "  Current State: " << stateToString(p1.getState()) << endl; // Should be READY (1)

        cout << "Changing state from READY to RUNNING..." << endl;
        p1.setState(STATE_RUNNING);
        cout << "  Current State: " << stateToString(p1.getState()) << endl; // Should be RUNNING (2)

        // 3. Demonstrate decrementing execution time
        cout << "\nSimulating run for 3 time units..." << endl;
        p1.runFor(3);
        cout << "  Remaining Time: " << p1.getRemainingTime() << endl; // Should be 2
        cout << "  Is process finished? " << (p1.isFinished() ? "Yes" : "No") << endl;

        cout << "\nSimulating run for 2 more time units..." << endl;
        p1.runFor(2);
        cout << "  Remaining Time: " << p1.getRemainingTime() << endl; // Should be 0
        cout << "  Is process finished? " << (p1.isFinished() ? "Yes" : "No") << endl;

        // 4. Demonstrate final state change
        cout << "\nSetting state to TERMINATED..." << endl;
        p1.setState(STATE_TERMINATED);
        cout << "  Final State: " << stateToString(p1.getState()) << endl; // Should be TERMINATED (4)
        
        cout << "===================================" << endl;
    }

    return 0;
}