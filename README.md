# operating-system-project
OS Project for Operating Systems Class

Group members: Rodolfo Mendoza and Angel Olivares 

Part 1 of Project Description: 
Implemented a Login page which uses a auntentication function to check for predetermined user login info.


Intructions: 
1.Compile the main.cpp file 

2.It will ask to enter username and password 

3.Use predeterminated User: 

Admin Password: password123! 

4.If username does not match, the program terminates. 


Part 2: Process Class
This class serves as the basic data structure for a process. 
It holds the core attributes and provides simple methods to manipulate its state and execution time.


Attributes:

'pid': A unique integer to ID the process.
'arrival_time': What the simulation time will be when the process arrives (starts).
'burst_time': The total bust (CPU) time required by the process.
'state': An integer (0,1,2,3,4) representing the current state (e.g. NEW, READY, RUNNING)
'remaining_time': The amount of CPU time left for the process to complete.

Methods:

'Process(pid, arrival, burst)': The constructor to initialize the core attributes. It sets the initial state to 'STATE_NEW'.
'setState(newState)': Updates the process's current state.
'runFor(time)': Decrements the 'remaining_time'. It includes a check to ensure the time never becomes negative.
getters: A set of getter methods to read the private attributes (e.g., 'getPid()', 'getState()').
'isFinished()': A helper method that returns true if 'remaining_time' is 0.