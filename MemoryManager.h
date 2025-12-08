#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <vector>
#include <queue>
#include <map>
#include <iostream>
#include <iomanip>
using namespace std;

// Page Table Entry
struct PageTableEntry{
    int frame_number;
    bool valid;      
    bool dirty;            
    bool referenced;       
    
    PageTableEntry() : frame_number(-1), valid(false), dirty(false), referenced(false) {}
};

// Page Table for a process
struct PageTable{
    int pid;
    vector<PageTableEntry> entries;
    int num_pages;
    
    PageTable() : pid(0), num_pages(0) {}
    
    PageTable(int process_id, int pages) : pid(process_id), num_pages(pages){
        entries.resize(pages);
    }
};

class MemoryManager{
    private:
        static const int PAGE_SIZE = 4;           // 4 KB per page
        static const int PHYSICAL_MEMORY = 1024;  // 1024 KB total physical memory
        static const int NUM_FRAMES = PHYSICAL_MEMORY / PAGE_SIZE; // 256 frames
        
        vector<bool> frame_table;                 // Track free/occupied frames
        map<int, PageTable> page_tables;          // Page table for each process
        queue<pair<int, int>> fifo_queue;         // For FIFO replacement (pid, page_num)
        
        int page_faults;
        int page_hits;
        int total_memory_accesses;
        
        // Find a free frame
        int findFreeFrame(){
            for (int i = 0; i < NUM_FRAMES; i++){
                if (!frame_table[i]){
                    return i;
                }
            }
            return -1; // No free frame
        }
        
        // FIFO Page Replacement
        int replacePage(){
            if (fifo_queue.empty()){
                return -1;
            }
            
            // Get oldest page
            auto oldest = fifo_queue.front();
            fifo_queue.pop();
            
            int victim_pid = oldest.first;
            int victim_page = oldest.second;
            
            // Get the frame number from victim page
            int frame = page_tables[victim_pid].entries[victim_page].frame_number;
            
            // Mark page as invalid
            page_tables[victim_pid].entries[victim_page].valid = false;
            page_tables[victim_pid].entries[victim_page].frame_number = -1;
            
            cout << "  [Page Replacement] Evicted Page " << victim_page 
                 << " of Process " << victim_pid << " from Frame " << frame << "\n";
            
            return frame;
        }
        
    public:
        MemoryManager() : page_faults(0), page_hits(0), total_memory_accesses(0){
            frame_table.resize(NUM_FRAMES, false);
        }
        
        // Allocate pages for a process
        bool allocateProcess(int pid, int memory_required_kb){
            int pages_needed = (memory_required_kb + PAGE_SIZE - 1) / PAGE_SIZE;
            
            cout << "  Process " << pid << " requires " << memory_required_kb 
                 << " KB (" << pages_needed << " pages)\n";
            
            // Create page table
            page_tables[pid] = PageTable(pid, pages_needed);
            
            cout << "  Created page table with " << pages_needed << " entries for Process " << pid << "\n";
            return true;
        }
        
        // Simulate memory access (handles page faults)
        bool accessMemory(int pid, int page_num){
            total_memory_accesses++;
            
            // Check if process exists
            if (page_tables.find(pid) == page_tables.end()){
                cout << "  ERROR: Process " << pid << " not found!\n";
                return false;
            }
            
            PageTable& pt = page_tables[pid];
            
            // Check if page number is valid
            if (page_num >= pt.num_pages){
                cout << "  ERROR: Invalid page number " << page_num << " for Process " << pid << "\n";
                return false;
            }
            
            // Check if page is in memory (valid bit)
            if (pt.entries[page_num].valid){
                // Page Hit
                page_hits++;
                pt.entries[page_num].referenced = true;
                return true;
            }
            
            // Page Fault - need to load page into memory
            page_faults++;
            cout << "  [Page Fault] Process " << pid << ", Page " << page_num << "\n";
            
            // Try to find free frame
            int frame = findFreeFrame();
            
            // If no free frame, use page replacement
            if (frame == -1){
                cout << "  [No Free Frames] Performing page replacement...\n";
                frame = replacePage();
            }
            
            if (frame == -1){
                cout << "  ERROR: Could not allocate frame!\n";
                return false;
            }
            
            // Load page into frame
            frame_table[frame] = true;
            pt.entries[page_num].frame_number = frame;
            pt.entries[page_num].valid = true;
            pt.entries[page_num].referenced = true;
            
            // Add to FIFO queue
            fifo_queue.push({pid, page_num});
            
            cout << "  [Page Loaded] Page " << page_num << " loaded into Frame " << frame << "\n";
            
            return true;
        }
        
        // Deallocate process memory
        void deallocateProcess(int pid){
            if (page_tables.find(pid) == page_tables.end()){
                return;
            }
            
            PageTable& pt = page_tables[pid];
            
            // Free all frames used by this process
            for (auto& entry : pt.entries){
                if (entry.valid){
                    frame_table[entry.frame_number] = false;
                }
            }
            
            // Remove from page table
            page_tables.erase(pid);
            
            cout << "  Deallocated memory for Process " << pid << "\n";
        }
        
        // Display page table for a process
        void displayPageTable(int pid){
            if (page_tables.find(pid) == page_tables.end()){
                cout << "  Process " << pid << " not found!\n";
                return;
            }
            
            PageTable& pt = page_tables[pid];
            
            cout << "\nPage Table for Process " << pid << ":\n";
            cout << left << setw(10) << "Page#"
                 << setw(12) << "Frame#"
                 << setw(10) << "Valid"
                 << setw(12) << "Referenced" << "\n";
            cout << "--------------------------------------------\n";
            
            for (int i = 0; i < pt.num_pages; i++){
                cout << left << setw(10) << i
                     << setw(12) << (pt.entries[i].valid ? to_string(pt.entries[i].frame_number) : "N/A")
                     << setw(10) << (pt.entries[i].valid ? "Yes" : "No")
                     << setw(12) << (pt.entries[i].referenced ? "Yes" : "No") << "\n";
            }
            cout << "\n";
        }
        
        // Display memory statistics
        void displayMemoryStats(){
            cout << "\n========== VIRTUAL MEMORY STATISTICS ==========\n";
            cout << "Physical Memory: " << PHYSICAL_MEMORY << " KB (" << NUM_FRAMES << " frames)\n";
            cout << "Page Size: " << PAGE_SIZE << " KB\n";
            cout << "Total Memory Accesses: " << total_memory_accesses << "\n";
            cout << "Page Hits: " << page_hits << "\n";
            cout << "Page Faults: " << page_faults << "\n";
            
            if (total_memory_accesses > 0){
                double hit_rate = (double)page_hits / total_memory_accesses * 100;
                double fault_rate = (double)page_faults / total_memory_accesses * 100;
                cout << fixed << setprecision(2);
                cout << "Hit Rate: " << hit_rate << "%\n";
                cout << "Page Fault Rate: " << fault_rate << "%\n";
            }
            
            // Count used frames
            int used_frames = 0;
            for (bool occupied : frame_table){
                if (occupied) used_frames++;
            }
            
            cout << "Frames Used: " << used_frames << " / " << NUM_FRAMES << "\n";
            cout << "Free Frames: " << (NUM_FRAMES - used_frames) << "\n";
            cout << "===============================================\n\n";
        }
        
        // Simulate memory accesses for a process during execution
        void simulateProcessMemoryAccess(int pid, int num_accesses = 5){
            if (page_tables.find(pid) == page_tables.end()){
                cout << "  Process " << pid << " not in memory!\n";
                return;
            }
            
            PageTable& pt = page_tables[pid];
            
            cout << "\nSimulating Memory Access for Process " << pid << ":\n";
            
            // Access different pages (simulate realistic access pattern)
            for (int i = 0; i < num_accesses && i < pt.num_pages; i++){
                int page_to_access = i % pt.num_pages;
                cout << "Access " << (i + 1) << ": ";
                accessMemory(pid, page_to_access);
            }
            
            // Access some pages again (simulate locality)
            if (num_accesses > pt.num_pages){
                for (int i = 0; i < (num_accesses - pt.num_pages); i++){
                    int page_to_access = i % pt.num_pages;
                    cout << "Access " << (pt.num_pages + i + 1) << ": ";
                    accessMemory(pid, page_to_access);
                }
            }
        }
        
        void reset(){
            frame_table.assign(NUM_FRAMES, false);
            page_tables.clear();
            while (!fifo_queue.empty()) fifo_queue.pop();
            page_faults = 0;
            page_hits = 0;
            total_memory_accesses = 0;
        }
};

#endif
