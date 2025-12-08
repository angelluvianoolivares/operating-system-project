# Operating System Project - CPU Scheduling & Virtual Memory

## Group Members
- Rodolfo Mendoza
- Angel Olivares

## Project Overview
This Operating System Simulator implements:
1. **User Authentication** - Login system with predetermined credentials
2. **CPU Scheduling Algorithms** - FCFS, SJF, and Round Robin
3. **Virtual Memory Management** - Paging system with page fault handling

---

## Features

### Part 1: Authentication System
- Implemented login page with authentication function
- Checks for predetermined user credentials
- Username: `admin`
- Password: `password123`

### Part 2: CPU Scheduling
Implements three scheduling algorithms:
- **FCFS (First Come First Serve)** - Non-preemptive scheduling
- **SJF (Shortest Job First)** - Non-preemptive scheduling
- **Round Robin** - Preemptive scheduling with time quantum

Features:
- Custom or default process sets
- Gantt chart visualization
- Performance metrics (waiting time, turnaround time)
- Algorithm comparison

### Part 3: Virtual Memory Management (NEW!)
Implements a **paging-based virtual memory system** with:

#### Key Components:
- **Page Size**: 4 KB
- **Physical Memory**: 1024 KB (256 frames)
- **Page Tables**: One per process
- **Page Replacement**: FIFO (First-In-First-Out) algorithm

#### Features:
1. **Automatic Memory Allocation**
   - Processes are allocated page tables based on memory requirements
   - Pages are loaded on-demand (demand paging)

2. **Page Fault Handling**
   - Simulates page faults when accessing non-resident pages
   - Automatically loads pages into available frames
   - Implements FIFO page replacement when memory is full

3. **Memory Statistics**
   - Total memory accesses
   - Page hits and page faults
   - Hit rate and fault rate percentages
   - Frame utilization

4. **Interactive Features**
   - View page tables for any process
   - Simulate memory accesses
   - Real-time memory statistics

---

## How It Works

### Virtual Memory Implementation

#### 1. Page Tables
Each process gets a page table with entries containing:
- **Frame Number**: Physical frame where the page is loaded
- **Valid Bit**: Whether the page is in physical memory
- **Dirty Bit**: Whether the page has been modified
- **Referenced Bit**: Recently accessed flag

#### 2. Memory Access Process
```
User requests memory access
    ↓
Check page table (valid bit)
    ↓
├─ Valid = 1 → PAGE HIT (access frame directly)
│
└─ Valid = 0 → PAGE FAULT
       ↓
   Find free frame
       ↓
   ├─ Frame available → Load page into frame
   │
   └─ No frame → FIFO Page Replacement
          ↓
      Evict oldest page → Load new page
```

#### 3. FIFO Page Replacement
- Maintains a queue of pages in memory
- When memory is full, evicts the oldest page
- Simple and fair, but may not be optimal

---

## Compilation Instructions

### Method 1: Using g++ directly
```bash
g++ -o os_simulator Main.cpp Auth.cpp -std=c++11
```

### Method 2: Using Makefile (if provided)
```bash
make
```

---

## Running the Program

1. **Start the program:**
   ```bash
   ./os_simulator
   ```

2. **Login:**
   - Username: `admin`
   - Password: `password123`

3. **Main Menu Options:**
   ```
   1. Use Default Process Set - Load 5 predefined processes
   2. Enter Custom Process - Define your own processes
   3. Run all Algorithms (Comparison) - Compare FCFS, SJF, and RR
   4. Run FCFS Only - Execute First Come First Serve
   5. Run SJF Only - Execute Shortest Job First
   6. Run Round Robin Only - Execute Round Robin (with quantum)
   7. View Page Table for Process - See page table details
   8. Simulate Memory Access for Process - Test page faults
   9. View Memory Statistics - Display memory performance
   10. Exit - Close the simulator
   ```

---

## Example Usage

### Basic Workflow:
1. Login with credentials
2. Select option `1` to load default processes
   - Automatically allocates memory for each process
   - Creates page tables
3. Select option `3` to run all algorithms
   - Simulates memory access during execution
   - Shows page faults and hits
   - Displays scheduling results
   - Shows memory statistics
4. Select option `7` to view a process's page table
5. Select option `9` to see overall memory performance

### Understanding Output:

#### Memory Allocation:
```
========== MEMORY ALLOCATION ==========
  Process 1 requires 256 KB (64 pages)
  Created page table with 64 entries for Process 1
  ...
```

#### Page Fault Example:
```
  [Page Fault] Process 1, Page 0
  [Page Loaded] Page 0 loaded into Frame 5
```

#### Memory Statistics:
```
========== VIRTUAL MEMORY STATISTICS ==========
Physical Memory: 1024 KB (256 frames)
Page Size: 4 KB
Total Memory Accesses: 150
Page Hits: 120
Page Faults: 30
Hit Rate: 80.00%
Page Fault Rate: 20.00%
Frames Used: 45 / 256
Free Frames: 211
===============================================
```

---

## File Structure
```
.
├── Auth.h              - Authentication header
├── Auth.cpp            - Authentication implementation
├── Process.h           - Process class definition
├── Scheduler.h         - CPU scheduling algorithms
├── MemoryManager.h     - Virtual memory management (NEW!)
├── Main.cpp            - Main program with menu system
└── README.md           - This file
```

---

## Technical Details

### Default Processes:
| PID | Arrival | Burst | Priority | Memory (KB) |
|-----|---------|-------|----------|-------------|
| 1   | 0       | 5     | 2        | 256         |
| 2   | 1       | 3     | 1        | 128         |
| 3   | 2       | 8     | 3        | 512         |
| 4   | 3       | 6     | 3        | 256         |
| 5   | 4       | 4     | 1        | 128         |

### Memory Calculations:
- Process with 256 KB needs: 256 / 4 = **64 pages**
- Process with 128 KB needs: 128 / 4 = **32 pages**
- Process with 512 KB needs: 512 / 4 = **128 pages**

---

## Learning Objectives
This project demonstrates:
- **Process Management**: Process states, PCB management
- **CPU Scheduling**: Different scheduling strategies and trade-offs
- **Virtual Memory**: Paging, page tables, address translation
- **Page Replacement**: FIFO algorithm implementation
- **Performance Metrics**: Hit rates, fault rates, efficiency analysis

---

## Future Enhancements (Optional)
- Implement other page replacement algorithms (LRU, Optimal)
- Add Translation Lookaside Buffer (TLB) simulation
- Implement segmentation
- Add memory compaction/defragmentation
- Support for variable page sizes
- Multi-level page tables

---

## Notes
- The program uses simulated time units for scheduling
- Memory access is simplified for educational purposes
- Page replacement occurs when physical memory is full
- All processes are allocated memory upon loading

---

## Troubleshooting

**Issue**: Program won't compile
- Make sure you have g++ installed with C++11 support
- Check that all header files are in the same directory

**Issue**: Login fails
- Use exactly: `admin` / `password123` (case-sensitive)

**Issue**: Page faults seem high
- This is normal on first access of each page (demand paging)
- Second access to the same page should result in page hits

---

## Credits
Developed for Operating Systems Class
Date: December 2024
