#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>

#define OFFSET_MASK 255
#define PAGES 256
#define OFFSET_BITS 8
#define PAGE_SIZE 256
#define TLB_SIZE 16
#define INT_SIZE 4
#define INT_COUNT 16384
#define MEMORY_SIZE 65536

int intArray[INT_COUNT];
signed char *mmapfptr;

typedef struct TLB{
    int table[TLB_SIZE][2];
}TLBentry;

int search_TLB(void *tlb, int value){
    TLBentry* t = (TLBentry *)tlb;
    for (int i = 0; i < TLB_SIZE; i++)
    {
        if (t->table[i][0] == value)
        {
            //Return -1 if fnumber does not exist
            return t->table[i][1];
        }
    }
    //Return -2 if pagenumber does not exist
    return -2;
}

int search_TLB2(void *tlb, int value){
    TLBentry* t = (TLBentry *)tlb;
    for (int i = 0; i < TLB_SIZE; i++)
    {
        if (t->table[i][1] == value)
        {
            //Return -1 if fnumber does not exist
            return t->table[i][0];
        }
    }
    //Return -2 if pagenumber does not exist
    return -2;
}

int TLB_Add(void *tlb, int page, int fnumber){
    TLBentry* t = (TLBentry *)tlb;
    for (int i = 0; i < TLB_SIZE; i++)
    {
        if (t->table[i][0] == -1)
        {
            t->table[i][0] = page;
            t->table[i][1] = fnumber;
            return 0;
        }
    }
    for (int i = 1; i < TLB_SIZE; i++)
    {
        t->table[i-1][0] = t->table[i][0];
        t->table[i-1][1] = t->table[i][1];
    }
    t->table[TLB_SIZE-1][0] = page;
    t->table[TLB_SIZE-1][1] = fnumber;
    return 0;
}

int TLB_Update(void *tlb, int initialpage, int page, int fnumber){
    TLBentry* t = (TLBentry *)tlb;
    for (int i = 0; i < TLB_SIZE; i++)
    {
        if (t->table[i][0] == initialpage)
        {
            t->table[i][0] = page;
            t->table[i][1] = fnumber;
            return 0;
        }
    }
}

int main(){
    TLBentry *data = (TLBentry *) malloc (sizeof(TLBentry));
    int addr;
    int pnumber;
    int poffset;
    int paddr;
    int page_table[PAGES];
    int frame_table[128];
    int fnumber = 0;
    int count = 0;
    int flag = 0;
    int omarisfat = 0;
    int moron = 0;
    int faults = 0;
    int idiot = 0;
    int bstore = 0;
    int mmapfile_fd = open("omarisdumb.bin", O_RDONLY);
    mmapfptr = mmap(0, MEMORY_SIZE, PROT_READ, MAP_PRIVATE, mmapfile_fd, 0);
    for (int i = 0; i < PAGES; i++)
    {
        page_table[i] = -1;
    }
    for (int i = 0; i < TLB_SIZE; i++)
    {
        data->table[i][0] = -1;
        data->table[i][1] = -1;
    }
    for (int i = 0; i < 128; i++)
    {
        frame_table[i] = -1;
    }
    
    FILE *fptr = fopen("addresses.txt", "r");
    char buff[10];

    while(fgets(buff, 10, fptr) != NULL){
        addr = atoi(buff);
        pnumber = addr >> OFFSET_BITS;
        poffset = addr & OFFSET_MASK;

        //When fnumber and/or pnumber do not exist
        if (search_TLB(data, pnumber) == -2){    
            //When fnumber is assigned to pnumber (page table)
            if (page_table[pnumber] != -1)
            {
                flag = 1;
            }
            
            //When fnumber is not assigned to a pnumber (page table)
            if (page_table[pnumber] == -1)
            {
                for (int i = 0; i < PAGES; i++)
                {
                    if (page_table[i] == fnumber)
                    {
                        page_table[i] = -1;
                        frame_table[fnumber] == -1;
                        TLB_Update(data, i, pnumber, fnumber);
                        omarisfat = 1;
                    }
                }
                memcpy(frame_table+fnumber, mmapfptr+4*fnumber, INT_SIZE);
                // printf("%d\n",mmapfptr);
                // bstore++;
                page_table[pnumber] = fnumber;
                // frame_table[fnumber] = pnumber;
                if (omarisfat == 0)
                {
                    TLB_Add(data, pnumber, fnumber);
                }
                
                fnumber++;
                faults++;
            }
            //When we have reset fnumber once (count > 0) and pnumber is assigned to an fnumber
            else if (count > 0 && flag == 0)
            {
                for (int i = 0; i < PAGES; i++)
                {
                    if (page_table[i] == fnumber)
                    {
                        page_table[i] = -1;
                        frame_table[fnumber] == -1;
                        TLB_Update(data, i, pnumber, fnumber);
                        omarisfat = 1;
                    }
                }
                page_table[pnumber] = fnumber;
                frame_table[fnumber] = pnumber;
                
                if (search_TLB(data, pnumber) == -2){
                    if (search_TLB2(data,fnumber) == -2)
                    {
                        TLB_Add(data,pnumber,fnumber);
                    }
                    else {
                        TLB_Update(data, search_TLB2(data,fnumber), pnumber, fnumber);
                    }
                }
                else{
                    TLB_Update(data, pnumber, pnumber, fnumber);
                }
                fnumber++;
            }
            else if (flag == 1 && search_TLB(data,pnumber) == -2)
            {
                if (search_TLB2(data,fnumber) == -2)
                {
                    TLB_Add(data,pnumber,fnumber);
                }
                else {
                    TLB_Update(data, search_TLB2(data,fnumber), pnumber, fnumber);
                }
                
            }
            idiot++;
        }
        else{
            moron++;
            // fnumber = search_TLB(data, pnumber);
            // if (data->table[TLB_SIZE-1][0] == pnumber)
            // {
            //     printf("%d\n",addr);
            // }
            printf("%d %d\n",addr,pnumber);
            
            
        }
        omarisfat = 0;
        flag = 0;
        if (fnumber == 128)
        {
            fnumber = 0;
            count++;
        }

        paddr = (search_TLB(data,pnumber) << OFFSET_BITS) | poffset;
        printf("Virtual address: %d Physical address = %d , Test %d %d\n",addr,paddr,pnumber,fnumber);
    }
    munmap(mmapfptr, MEMORY_SIZE);
    printf("%d %d %d",moron,idiot,faults);
    fclose(fptr);
}