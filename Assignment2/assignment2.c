/* 
  Mohammad Omar Zahir - zahirm1
  Mahad Aziz - azizm17
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>

// Constants that are used to calculate the page number, page offset, physical address, and to initalize the page table array
#define OFFSET_MASK 255
#define PAGES 256
#define OFFSET_BITS 8
#define PAGE_SIZE 256
// Constant that is used to define the size of the TLb
#define TLB_SIZE 16
// Constants that are used to memory map the binary file and copy the frame from the file to the physical memory array
#define INT_SIZE 256
#define MEMORY_SIZE 65536

// Signed char variable that is used to memory the binary file
signed char *mmapfptr;
// Signed char array that is used to simulate the physical memory
signed char phys_memory[32768];

// This is the struct that is used to simulate the TLB using a 2D integer array
typedef struct TLB{
    int table[16][2];
}TLBentry;

/* This function is responsible for searching the TLB given a page number. */
int search_TLB(void *tlb, int page){
    TLBentry* t = (TLBentry *)tlb;
    // For loop to iterate over the TLB
    for (int i = 0; i < TLB_SIZE; i++)
    {
        // If page number is equal to the page number at the current index of the TLB then return the frame number
        if (t->table[i][0] == page)
        {
            return t->table[i][1];
        }
    }
    // Otherwise return -1
    return -1;
}

/* This function is responsible for adding a page number and a frame number to the TLB. */
int TLB_Add(void *tlb, int page, int frame){
    TLBentry* t = (TLBentry *)tlb;
    // For loop to iterate over the TLB
    for (int i = 0; i < TLB_SIZE; i++)
    {
        // If the page number at the current index of the TLB is -1, then we insert the page number and frame number into that index and return
        if (t->table[i][0] == -1)
        {
            t->table[i][0] = page;
            t->table[i][1] = frame;
            return 0;
        }
    }
    // For loop to iterate over the TLB if any of the page numbers in the TLB do not equal -1
    for (int i = 1; i < TLB_SIZE; i++)
    {
        // Copy the page number and frame number at the current index to the previous index
        t->table[i-1][0] = t->table[i][0];
        t->table[i-1][1] = t->table[i][1];
    }
    // Assign the new page number and frame number to the final index of the TLB
    t->table[TLB_SIZE-1][0] = page;
    t->table[TLB_SIZE-1][1] = frame;
    return 0;
}

/* This function is responsible for updating the page number and a frame number to the TLB given an existing page number in the table. */
int TLB_Update(void *tlb, int initialpage, int page, int frame){
    TLBentry* t = (TLBentry *)tlb;
    // For loop to iterate over the TLB
    for (int i = 0; i < TLB_SIZE; i++)
    {
        // If the page number exists in the TLB then replace the initial page with the new page number and its corresponding frame number
        if (t->table[i][0] == initialpage)
        {
            t->table[i][0] = page;
            t->table[i][1] = frame;
            return 0;
        }
    }
    return 0;
}

int main(){
    // Allocates data to store and represent the TLB
    TLBentry *data = (TLBentry *) malloc (sizeof(TLBentry));
    // Variables for storing values related to the output
    int addr;
    int pnumber;
    int poffset;
    int paddr;
    int frame;
    // Initialize page table array
    int page_table[PAGES];
    /* Acts as a pointer to the circular array of the TLB, keeping track of how many entries are in it and 
    the current iteration (Simulates Circular Array) */
    int count = 0;
    // Acts as a boolean flag signalling when a certain value in the TLB has changed before
    int flag = 0;
    // Used to store total hits
    int hits = 0;
    // Used to store total faults
    int faults = 0;
    // Used to store total iterations
    int total = 0;
    // Initializes Page Table
    for (int i = 0; i < PAGES; i++)
    {
        page_table[i] = -1;
    }
    // Initializes TLB
    for (int i = 0; i < TLB_SIZE; i++)
    {
        data->table[i][0] = -1;
        data->table[i][1] = -1;
    }
    // Open the text file to read virtual addresses and initialize buffer
    FILE *fptr = fopen("addresses.txt", "r");
    char buff[10];
    // Open the binary file to read frame numbers when a page fault occurs
    int mmapfile_fd = open("BACKING_STORE.bin", O_RDONLY);
    mmapfptr = mmap(0, MEMORY_SIZE, PROT_READ, MAP_PRIVATE, mmapfile_fd, 0);
    // Main loops iterating over each virtual address
    while(fgets(buff, 10, fptr) != NULL){
        addr = atoi(buff);
        // Determines the page number and offset from the virtual address and offset bits/mask
        pnumber = addr >> OFFSET_BITS;
        poffset = addr & OFFSET_MASK;
        // Checks if TLB has current page number present
        if (search_TLB(data,pnumber) == -1)
        {
            // Checks if page table has current page number present
            if (page_table[pnumber] == -1)
            {
                // terates over all the elemnts in the page table
                for (int i = 0; i < PAGES; i++)
                {
                    // Checks if any values in the page table have the current frame number
                    if (page_table[i] == count)
                    {
                        // Remove the frame number from the old page number that it was associated with
                        page_table[i] = -1;
                        // Update the TLB with the new page number and frame number at the same index of the old page number
                        TLB_Update(data,i,pnumber,count);
                        flag = 1;
                    }
                }
                // If the frame number was not previously associated with any page number then add the page number and frame number to the TLB
                if (flag == 0)
                {
                    TLB_Add(data,pnumber,count);
                }
                // Assigns corresponding frame number to the page table according to the counter
                page_table[pnumber] = count;
                count++;
                faults++;
                // Copies the frame from the binary file to the physical memory from the corresponding page number
                memcpy(phys_memory+256*page_table[pnumber], mmapfptr+256*pnumber, INT_SIZE);
            }
            frame = page_table[pnumber];
        }
        // If TLB does contain current page number entry, retrieve it
        else{
            hits++;
            frame = search_TLB(data,pnumber);
        }
        // Determines the phyical address from the page number
        paddr = (frame << OFFSET_BITS) | poffset;
        // Resets count once it has reached the maximum of 128 (Simulates circular array)
        if (count == 128)
        {
            count = 0;
        }
        total++;
        // Prints the output and required values for each iteration
        printf("Virtual address: %d Physical address = %d Value=%d \n",addr,paddr,phys_memory[paddr]);
    }
    // Prints the correpsonding required values once at the end of the program
    printf("Total addresses = %d \n",total);
    printf("Page_faults = %d \n",faults);
    printf("TLB Hits = %d ",hits);
    // Unmaps the memory used for the Backing store
    munmap(mmapfptr, MEMORY_SIZE);
    fclose(fptr);
}