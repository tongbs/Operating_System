//0413359
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

int main() {
    const int FRAMES = 256;
    const int FRAME_SIZE = 256;
    const int TLB_SIZE = 16;

    int temp;
    int pageNum;
    int offset;
    int value;
    int physAddress;

    int pageTable[FRAMES][2];
    char buff[FRAME_SIZE];
    unsigned int mem[FRAMES][FRAME_SIZE];
    bool inTLB = false;
    bool inPageTable = false;

    int frameNum;
    int tlbHit = 0;
    int pageFault = 0;
    int numAddresses = 0;
    int memCount = 0;
    int tableCount = 0;
    int addressNum;

    for(int i = 0; i<FRAMES; i++)
    {
        pageTable[i][0] = -1;
        pageTable[i][1] = -1;
    }

    //vector<int> f;

    // *** Open Logical Addresses File ***
    ifstream file;
    file.open("address.txt");

    // *** Open Output File **
    ofstream output("result.txt");
    vector<int> tlb[2];

    // *** While Loop For handling Addresses ***
    file >> addressNum;
    while(file >> temp) {

        // Reset Flags
        inTLB = false;
        inPageTable = false;
        numAddresses++;

        // Get offset and page number
        pageNum = temp & 0xFF00;
        offset = temp & 0xFF;
        pageNum = pageNum >> 8;

        // Search For Frame in TLB
        for (int i = 0; i < tlb[0].size(); i++)
        {
            if (pageNum == tlb[0].at(i))
            {
                tlbHit++;
                inTLB = true;
                frameNum = tlb[1].at(i);
                tlb[0].erase(tlb[0].begin()+i);
                tlb[1].erase(tlb[1].begin()+i);

                tlb[0].push_back(pageNum);
                tlb[1].push_back(frameNum);
                break;
            }
        }

        // Search in Page Table
        if(!inTLB)
        {
            for(int i = 0; i < FRAMES; i++)
            {
                if(pageNum == pageTable[i][0])
                {

                    frameNum = pageTable[i][1];
                    inPageTable = true;
                    if (tlb[0].size() == 16)
                    {
                        tlb[0].erase(tlb[0].begin());
                        tlb[1].erase(tlb[1].begin());
                    }
                    tlb[0].push_back(pageNum);
                    tlb[1].push_back(frameNum);
                }
            }
            if(!inPageTable)
            {
                // *** Open Backing Store ***
                ifstream backingStore;
                backingStore.open("BACKING_STORE.bin", ios::binary | ios::in);

                pageFault++;
                // Find the page
                backingStore.seekg((pageNum * FRAME_SIZE), backingStore.beg);
                backingStore.read(buff, FRAME_SIZE);
                // Save page
                for(int i = 0; i < sizeof(buff); i++)
                {
                    mem[memCount][i] = buff[i];
                }
				// Close BACKING_STORE
                backingStore.close();
                //Find Requested Byte
                pageTable[memCount][0] = pageNum;
                pageTable[memCount][1] = memCount;
                frameNum = memCount;
                memCount = (memCount + 1) % FRAMES;
                // Save Page in TLB
                if(tlb[0].size() == 16)
                {
                    tlb[0].erase(tlb[0].begin());
                    tlb[1].erase(tlb[1].begin());
                }
                tlb[0].push_back(pageNum);
                tlb[1].push_back(frameNum);
            }
            //tableCount = (tableCount + 1) % TLB_SIZE;
        }

        // Get value and Physical Addresss from memory
        value = mem[frameNum][offset];
        physAddress = (frameNum * FRAME_SIZE) + offset;

        //cout << "page : " << pageNum << endl << "TLB : " << endl;
        /*for(int k = 0; k<tlb[0].size();k++)
        {
            cout << tlb[0][k] << "  " << tlb[1][k] << "  " <<endl;
        }*/
        //cout << endl;
        // Output to File
        output << physAddress << "     " << value << endl;
    }

    // Record Statistics
    output << "TLB Hits: " << tlbHit << endl;
    output << "Page Faults: " << pageFault << endl;
    // Close Files
    file.close();
    output.close();
    return 0;
}
