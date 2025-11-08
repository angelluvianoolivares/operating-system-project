#include "Auth.h"
#include <iostream>
#include <string>
using namespace std;

bool authenticateUser(){
    string username, password;

    cout << "Enter Username: ";
    cin >> username;
    cout << "Enter Password: ";
    cin >> password;

    if (username == "admin" && password == "password123"){
        return true;
    } else {
        return false;
    }
}