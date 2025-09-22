#include <string>
#include <iostream>
#include "auth.h"

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

    }

    return 0;
}