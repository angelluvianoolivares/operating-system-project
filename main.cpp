#include <string>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include "auth.h"
#include "Process.h"
using namespace std;

static void runProcessDemo() {
    cout << "\n Process Class Demo \n");
    
    Process p1(1, 0, 5, 2, 256);
    Process p2(2, 1, 3, 1, 128, true);
    Process p3(3, 2, 4, 3, 512);

    vector<Process*> processes = { &p1, &p2, &p3 };

    cout << "Initial state: NEW (on creation)\n";

    cout << "\n READY Queue \n";
    for (auto* p : processes) {
        p->updateState(ProcessState::READY);
        p->display();
    }

    cout << "\n CPU Execution \n");
    for (auto* p : processes) {
        p->updateState(ProcessState::RUNNING);
        cout << "\nRunning PID " << p->getPID() << "...\n";
        while (p->getRemainingTime() > 0) {
            p->executeOneUnit();
            p->display();
            this_thread::sleep_for(chrono::milliseconds(200));
        }
    }

    cout << "\n Summary \n");
    for (auto* p : processes) {
        p->display();
    }
    cout << "\n All processes completed. \n";
}

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
    }

    runProcessDemo();

    return 0;
}
