#pragma once
#include <vector>
#include <map>

//-Virtual Memory Constants-
const int PAGE_SIZE = 4096; // 4 KB Page Size
const int OFFSET_BITS = 12; // 12 bits for Offset (2^12 = 4096)
const int TLB_SIZE = 4;     // Small TLB Cache Size

struct TLBEntry {
    int vpn;    // Virtual Page Number
    int pfn;    // Physical Frame Number
    bool valid; // Is entry valid?
};

class MMU {
private:
    //-The TLB (Fast Cache)-
    std::vector<TLBEntry> tlb;

    //-The Page Table (Slow Map)-
    // Key: Virtual Page Number (VPN), Value: Physical Frame Number (PFN)
    std::map<int, int> pageTable;

    //-Free Frame Counter (Simulating Physical RAM allocation)-
    int nextFreeFrame;

public:
    //-Constructor-
    MMU();

    //-The Main Job: Translates a 32-bit Virtual Address to a Physical Address-
    // Returns -1 if the translation fails (Page Fault)
    int translate(int virtualAddress);

    //-Helper: Maps a Virtual Page to a Physical Frame (Simulates loading from disk)-
    void mapPage(int vpn);
};
