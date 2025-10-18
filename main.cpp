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

        cout << "---------------------------------------------" << endl;
        cout << "-----Process Class Test Cases-------" << endl;

        // 1. Create processes using our new constructor
        Process p1(1, 0, 5); // pid=1, arrival=0, burst=5
        //Process p2(2, 1, 3); // pid=2, arrival=1, burst=3
        
        cout << "Created Process (PID: " << p1.getPid() << ")" << endl;
        cout << "Initial State: " << stateToString(p1.getState()) << endl; // Should be NEW (0)
        cout << "Initial Remaining Time: " << p1.getRemainingTime() << endl; // Should be 5

        /*
        cout << "Created Process (PID: " << p2.getPid() << ")" << endl;
        cout << "Initial State: " << stateToString(p2.getState()) << endl; // Should be NEW (0)
        cout << "Initial Remaining Time: " << p2.getRemainingTime() << endl; // Should be 3
        */

        // 2. Demonstrating updating the process state
        cout << "Changing state from NEW to READY." << endl;
        p1.setState(STATE_READY);
        cout << "Current State: " << stateToString(p1.getState()) << endl; // Should be READY (1)

        cout << "Changing state from READY to RUNNING." << endl;
        p1.setState(STATE_RUNNING);
        cout << "Current State: " << stateToString(p1.getState()) << endl; // Should be RUNNING (2)

        /*
        cout << "Changing state from NEW to READY." << endl;
        p2.setState(STATE_READY);
        cout << "Current State: " << stateToString(p1.getState()) << endl; 

        cout << "Changing state from READY to RUNNING." << endl;
        p2.setState(STATE_RUNNING);
        cout << "Current State: " << stateToString(p1.getState()) << endl; 
        */

        // 3. Demonstrates the decrementing execution time
        cout << "Simulating run for 3 time units." << endl;
        p1.runFor(3);
        cout << "Remaining Time: " << p1.getRemainingTime() << endl; // Should be 2
        cout << "Is process finished? " << (p1.isFinished() ? "Yes" : "No") << endl;

        cout << "Simulating run for 2 more time units..." << endl;
        p1.runFor(2);
        cout << "Remaining Time: " << p1.getRemainingTime() << endl; // Should be 0
        cout << "Is process finished? " << (p1.isFinished() ? "Yes" : "No") << endl;

        /*
        cout << "Simulating run for 3 time units." << endl;
        p2.runFor(4);
        cout << "Remaining Time: " << p2.getRemainingTime() << endl;
        cout << "Is process finished? " << (p2.isFinished() ? "Yes" : "No") << endl;

        cout << "Simulating run for 2 more time units..." << endl;
        p2.runFor(3);
        cout << "Remaining Time: " << p2.getRemainingTime() << endl; 
        cout << "Is process finished? " << (p2.isFinished() ? "Yes" : "No") << endl;
        */

        // 4. Demonstrates final state change
        cout << "Setting state to TERMINATED." << endl;
        p1.setState(STATE_TERMINATED);
        cout << "Final State: " << stateToString(p1.getState()) << endl; // Should be TERMINATED (4)

        //p2.setState(STATE_TERMINATED);
        
        cout << "---------------------------------------------------" << endl;
    }

    return 0;
}