#include "MMU.h"
#include <iostream>

//-Constructor-
MMU::MMU() {
    // Start allocating from Frame 0
    nextFreeFrame = 0;
}

//-Helper: Add a mapping to the Page Table-
void MMU::mapPage(int vpn) {
    // Assign the next available physical frame
    pageTable[vpn] = nextFreeFrame;
    nextFreeFrame++;
}

//-The Main Job: Translate Virtual -> Physical-
int MMU::translate(int virtualAddress) {
    // 1. Extract the components using bitwise operators
    //    VPN is the top 20 bits (shift right by 12)
    int vpn = virtualAddress >> OFFSET_BITS;

    //    Offset is the bottom 12 bits (mask with 0xFFF)
    int offset = virtualAddress & (PAGE_SIZE - 1);

    int pfn = -1; // -1 means "Not Found"
    bool tlbHit = false;

    // 2. Check the TLB (Fast Cache) first
    for (const auto& entry : tlb) {
        if (entry.valid && entry.vpn == vpn) {
            pfn = entry.pfn;
            tlbHit = true;
            // (LRU logic would go here, but we keep it simple)
            break;
        }
    }

    // 3. If TLB Miss, Check the Page Table (Slow Memory)
    if (!tlbHit) {
        if (pageTable.count(vpn)) {
            pfn = pageTable[vpn];

            // We found it in memory! Update the TLB.
            // FIFO Replacement: If full, remove the first one.
            if (tlb.size() >= TLB_SIZE) {
                tlb.erase(tlb.begin());
            }

            // Add the new entry
            TLBEntry newEntry;
            newEntry.vpn = vpn;
            newEntry.pfn = pfn;
            newEntry.valid = true;
            tlb.push_back(newEntry);
        }
        else {
            // Page Fault: The page is not in the page table
            return -1;
        }
    }

    // 4. Reconstruct the Physical Address
    //    (Frame Number shifted left) + Offset
    int physicalAddress = (pfn << OFFSET_BITS) | offset;

    return physicalAddress;
}
