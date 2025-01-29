/* 
  Mohammad Omar Zahir - zahirm1
  Mahad Aziz - azizm17
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>

//Constants that are used to memory map the binary file and copy the requests from the file to the requests and sortedrequests array
#define INT_SIZE 4
#define INT_COUNT 20
#define MEMORY_SIZE INT_SIZE*INT_COUNT

//Signed char variable that is used to memory the binary file
signed char *mmapfptr;
//Int array that is used to hold the requests
int requests[INT_COUNT];
//Int array that is used to hold the requests but they are sorted
int sortedrequests[INT_COUNT];

/* This function is responsible for sorting the requests in the sortedrequests array using bubble sort. */
void bubbleSort() {
    //For loop to iterate over the array except for the last element
    for (int i = 0; i < INT_COUNT - 1; i++) {
        //For loop to iterate over the elements in the array that come after the current index
        for (int j = 0; j < INT_COUNT - i - 1; j++) {
            //If statement to compare adjacent elements and switch them if they are not in ascending order
            if (sortedrequests[j] > sortedrequests[j + 1]) {
                int temp = sortedrequests[j];
                sortedrequests[j] = sortedrequests[j + 1];
                sortedrequests[j + 1] = temp;
            }
        }
    }
}

/* This function is responsible for scheduling the requests using FCFS and tracking the total head movement. */
int fcfs(int head){
    //Keeps track of total head movements
    int total = 0;
    printf("\nFCFS DISK SCHEDULING ALGORITHM\n\n");
    //For loop to iterate over the requests array and print each request
    for (int i = 0; i < INT_COUNT; i++)
    {
        printf("%d",requests[i]);
        if (i != INT_COUNT-1)
        {
            printf(", ");
        }        
        //Total adds the distance between the request and the current head and then the head gets updated to the current request
        total += abs(requests[i] - head);
        head = requests[i];
    }
    //Print the total head movements
    printf("\n\nFCFS - Total head movements = %d\n",total);
    //Exit Function
    return -1;
}

/* This function is responsible for scheduling the requests using SSTF and tracking the total head movement. */
int sstf(int head){
    //Keeps track of total head movements
    int total = 0;
    //Keeps track of shortest distance
    int sd = 300;
    //Keeps track of index traversed by head
    int index;
    printf("\nSSTF DISK SCHEDULING ALGORITHM\n\n");
    //Created a temp array that holds the requests so we can modify the requests
    int temp[INT_COUNT];
    for (int i = 0; i < INT_COUNT; i++)
    {
        temp[i] = requests[i];
    }
    //For loop to iterate over the requests array and print each request
    for (int i = 0; i < INT_COUNT; i++)
    {
        //For loop to iterate over the requests array and determine which request has the shortest distance to the head
        for (int j = 0; j < INT_COUNT; j++)
        {
            if (abs(head - temp[j]) < sd)
            {
                //Keep track of the index that has the shortest distance and also keep track of the actual distance
                index = j;
                sd = abs(head - temp[j]);
            }
        }
        printf("%d",temp[index]);
        if (i != INT_COUNT-1)
        {
            printf(", ");
        }    
        //Total adds the distance between the request and the current head and then the head gets updated to the current request    
        total += abs(temp[index] - head);
        head = temp[index];
        // Shortest distance is reset and the index of the temp array is set to 999 so it can no longer be used as a shortest distance
        sd = 300;
        temp[index] = 999;
    }
    //Print the total head movements
    printf("\n\nSSTF - Total head movements = %d\n",total);
    //Exit Function
    return -1;
}

/* This function is responsible for scheduling the requests using SCAN and tracking the total head movement. */
int scan(int head, int dir){
    //Keeps track of total head movements
    int total = 0;
    //Keeps track of index traversed by head
    int index;
    printf("\nSCAN DISK SCHEDULING ALGORITHM\n\n");
    //Creates temporary array for holding the sorted requests
    int temp[INT_COUNT];
    for (int i = 0; i < INT_COUNT; i++)
    {
        temp[i] = sortedrequests[i];
    }
    //If direction is left
    if (dir == 0)
    {
        //If the value at the starting index is greater than the largest value of the requests
        if (head >= temp[19])
        {
            //Traverse the requests from the end to the start while printing them
            for (int i = 19; i >= 0; i--)
            {
                printf("%d",temp[i]);
                //Logic for printing commas
                if (i != 0)
                {
                    printf(", ");
                }
            }
            //Calculate the value of the total head movement by subtracting original head value from the first sorted request
            total += abs(head-temp[0]);
        }
        //If the value is not greater than the largest request value
        else{
            //Check if the value at the starting index is smaller than the smallest value of the requests
            if (head < temp[0])
            {
                //Set the index to 0 if so
                index = 0;
            }
            //If the value at the starting index is not smaller than the smallest value of the requests
            else {
                //Traverse the sorted requests from the start to the end to determine the closest request to our starting head value
                for (int i = 0; i < INT_COUNT-1; i++)
                {
                    //The closest request will be the left of the request if the head is between two requests
                    if (head >= temp[i] && head < temp[i+1])
                    {
                        index = i;
                    }
                }
            }
            //Traverses all of the requests of the requests array from the determined index to the first index
            for (int i = index; i >= 0; i--)
            {
                //Print all of these indexes out
                printf("%d",temp[i]);
                //Logic for printing commas
                if (i != INT_COUNT-1)
                {
                    printf(", ");
                }
                //Store a value of 300 to ensure this index does not get traversed again
                temp[i] = 300;
            }
            //Add the current value of the head to the total
            total += head;
            //Traverse right from the first index to the last index of the requests array
            for (int i = 0; i < INT_COUNT; i++)
            {
                //Avoid the requests that have already been printed
                if (temp[i] != 300)
                {
                    printf("%d",temp[i]);
                    if (i != INT_COUNT-1)
                    {
                        printf(", ");
                    }
                    //Set the value of head to the every index we traverse in sorted requests
                    head = temp[i];
                    temp[i] = 300;
                }
                
            }
            //Add the value of the current head to total
            total += head;
        }
    }
    //If direction is right
    else{
        //If the value at the starting index is less than the smallest value of the requests
        if (head <= temp[0])
        {
            //Traverse the requests from the start to the end while printing them
            for (int i = 0; i < INT_COUNT; i++)
            {
                printf("%d",temp[i]);
                if (i != INT_COUNT-1)
                {
                    printf(", ");
                }
            }
            //Calculate the value of the total head movement by subtracting original head value from the last sorted request
            total += abs(head-temp[19]);
        }
        //If the value is greater than the smallest request value
        else{
            //Check if the value at the starting index is greater than or equal the larger value of the requests
            if (head >= temp[19])
            {
                //Set the index to 19 if so
                index = 19;
            }
            //If the value at the starting index is smaller than the largest value of the requests
            else {
                //Traverse the sorted requests from the start to the end to determine the closest request to our starting head value
                for (int i = 0; i < INT_COUNT-1; i++)
                {
                    //Check if our head value is exactly the value of a request
                    if (head == temp[i])
                    {
                        //Set it index to that request
                        index = i;
                    }
                    //The closest request will be the right of the request if the head is between two requests
                    else if (head > temp[i] && head < temp[i+1])
                    {
                        index = i+1;
                    }
                }
            }
            //Traverse right from the current index to the last index of the requests array
            for (int i = index; i < INT_COUNT; i++)
            {
                printf("%d",temp[i]);
                if (i != 0)
                {
                    printf(", ");
                } 
                //Store a value of 300 to ensure this index does not get traversed again
                temp[i] = 300;
            }
            //Add the difference from our starting heads and 299 to total
            total += abs(head - 299);
            //Traverse all of the indexes from the second last request to the last request
            for (int i = INT_COUNT-1; i >= 0; i--)
            {
                //Ensure the index has not been traverses yet
                if (temp[i] != 300)
                {
                    printf("%d",temp[i]);
                    if (i != 0)
                    {
                        printf(", ");
                    }
                    //Set the value of head to the every index we traverse in sorted requests
                    head = temp[i];
                    temp[i] = 300;
                }
            }
            //Add the difference from 299 and the first request in our sorted requests to total
            total += abs(299 - sortedrequests[0]);
        }
    }
    //Print the total head movements
    printf("\n\nSCAN - Total head movements = %d\n",total);
    //Exit Function
    return -1;
}

/* This function is responsible for scheduling the requests using C-SCAN and tracking the total head movement. */
int cscan(int head, int dir){
    //Keeps track of total head movements
    int total = 0;
    //Keeps track of index traversed by head
    int index;
    printf("\nC-SCAN DISK SCHEDULING ALGORITHM\n\n");
    //Creates temporary array for holding the sorted requests
    int temp[INT_COUNT];
    for (int i = 0; i < INT_COUNT; i++)
    {
        temp[i] = sortedrequests[i];
    }
    //If direction is left
    if (dir == 0)
    {
        //If the value at the starting index is greater than the largest value of the requests
        if (head >= temp[19])
        {
            //Traverse the requests from the end to the start while printing them
            for (int i = 19; i >= 0; i--)
            {
                printf("%d",temp[i]);
                //Logic for printing commas
                if (i != 0)
                {
                    printf(", ");
                }
            }
            //Calculate the value of the total head movement by subtracting original head value from the first sorted request
            total += abs(head-temp[0]);
        }
        //If the value at the starting index is not greater than the largest value of the requests
        else{
            //Check if the value at the head is smaller than the first request
            if (head < temp[0])
            {
                //Set the index to -1
                index = -1;
            }
            //If the value of the head is not smaller than the first request
            else {
                //The closest request will be the left of the request if the head is between two requests
                for (int i = 0; i < INT_COUNT-1; i++)
                {
                    if (head >= temp[i] && head < temp[i+1])
                    {
                        index = i;
                    }
                }
            }
            //Traverses all of the requests of the requests array from the determined index to the first index
            for (int i = index; i >= 0; i--)
            {
                //Print all of these indexes out
                printf("%d",temp[i]);
                //Logic for printing commas
                if (i != INT_COUNT-1)
                {
                    printf(", ");
                }
                //Store a value of 300 to ensure this index does not get traversed again
                temp[i] = 300;
            }
            //Add the current value of the head to the total
            total += head;
            //Traverse all of the indexes from the second last request to the last request
            for (int i = INT_COUNT-1; i >= 0; i--)
            {
                //Ensure the index has not been traverses yet
                if (temp[i] != 300)
                {
                    printf("%d",temp[i]);
                    if (i != index+1)
                    {
                        printf(", ");
                    }
                    //Set the value of head to the every index we traverse in sorted requests
                    head = temp[i];
                    temp[i] = 300;
                }
            }
            total += 299 + abs(299 - sortedrequests[index+1]);
        }
    }
    //If direction is right
    else{
        //If the value at the starting index is less than the smallest value of the requests
        if (head <= temp[0])
        {
            //Traverse the requests from the start to the end while printing them
            for (int i = 0; i < INT_COUNT; i++)
            {
                printf("%d",temp[i]);
                if (i != INT_COUNT-1)
                {
                    printf(", ");
                }
            }
            //Calculate the value of the total head movement by subtracting original head value from the last sorted request
            total += abs(head-temp[19]);
        }
        //If the value is greater than the smallest request value
        else{
            //Check if the value at the starting index is greater than the larger value of the requests
            if (head > temp[19])
            {
                //Set the index to 19 if so
                index = 20;
            }
            //If the value at the starting index is smaller than or equal the largest value of the requests
            else {
                //Traverse the sorted requests from the start to the end to determine the closest request to our starting head value
                for (int i = 0; i < INT_COUNT-1; i++)
                {
                    //Check if our head value is exactly the value of a request
                    if (head == temp[i])
                    {
                        //Set it index to that request
                        index = i;
                    }
                    //The closest request will be the right of the request if the head is between two requests
                    else if (head > temp[i] && head < temp[i+1])
                    {
                        index = i+1;
                    }
                }
            }
            //Traverse right from the current index to the last index of the requests array
            for (int i = index; i < INT_COUNT; i++)
            {
                printf("%d",temp[i]);
                if (i != 0)
                {
                    printf(", ");
                } 
                //Store a value of 300 to ensure this index does not get traversed again
                temp[i] = 300;
            }
            //Add the difference from our starting head and 299 to total
            total += abs(head - 299);
            //Traverse all of the requests from the first index to the second last index
            for (int i = 0; i <= INT_COUNT-1; i++)
            {
                //Ensure that the index has not been traversed before
                if (temp[i] != 300)
                {
                    printf("%d",temp[i]);
                    //Logic for printing commas
                    if (i != index-1)
                    {
                        printf(", ");
                    }
                    //Set the value of head to the every index we traverse in sorted requests
                    head = temp[i];
                    temp[i] = 300;
                }
            }
            //Add the sum of 299 and the difference of 0 and the second last request to total
            total += 299 + abs(0 - sortedrequests[index-1]);
        }
    }
    //Print the total head movements
    printf("\n\nC-SCAN - Total head movements = %d\n",total);
    //Exit Function
    return -1;
}

/* This function is responsible for scheduling the requests using LOOK and tracking the total head movement. */
int look(int head, int dir){
    int total = 0;
    int index;
    printf("\nLOOK DISK SCHEDULING ALGORITHM\n\n");
    int temp[INT_COUNT];
    for (int i = 0; i < INT_COUNT; i++)
    {
        temp[i] = sortedrequests[i];
    }
    if (dir == 0)
    {
        if (head >= temp[19])
        {
            for (int i = 19; i >= 0; i--)
            {
                printf("%d",temp[i]);
                if (i != 0)
                {
                    printf(", ");
                }
            }
            //Calculate the value of the total head movement by subtracting original head value from the first sorted request
            total += abs(head-temp[0]);
        }
        else{
            if (head < temp[0])
            {
                index = 0;
            }
            else {
                for (int i = 0; i < INT_COUNT-1; i++)
                {
                    if (head >= temp[i] && head < temp[i+1])
                    {
                        index = i;
                    }
                }
            }
            for (int i = index; i >= 0; i--)
            {
                printf("%d",temp[i]);
                if (i != INT_COUNT-1)
                {
                    printf(", ");
                } 
                temp[i] = 300;
            }
            //Add the current value of the head minus the first request to the total
            total += abs(head-sortedrequests[0]);
            for (int i = 0; i < INT_COUNT; i++)
            {
                if (temp[i] != 300)
                {
                    printf("%d",temp[i]);
                    if (i != INT_COUNT-1)
                    {
                        printf(", ");
                    } 
                    head = temp[i];
                    temp[i] = 300;
                }
                
            }
            //Add the value of the last request minus the first request to the total
            total += abs(sortedrequests[INT_COUNT-1]-sortedrequests[0]);
        }
    }
    else{
        if (head <= temp[0])
        {
            for (int i = 0; i < INT_COUNT; i++)
            {
                printf("%d",temp[i]);
                if (i != INT_COUNT-1)
                {
                    printf(", ");
                }
            }
            //Calculate the value of the total head movement by subtracting original head value from the last sorted request
            total += abs(head-temp[19]);
        }
        else{
            if (head >= temp[19])
            {
                index = 19;
            }
            else {
                for (int i = 0; i < INT_COUNT-1; i++)
                {
                    if (head == temp[i])
                    {
                        index = i;
                    }
                    else if (head > temp[i] && head < temp[i+1])
                    {
                        index = i+1;
                    }
                }
            }
            for (int i = index; i < INT_COUNT; i++)
            {
                printf("%d",temp[i]);
                if (i != 0)
                {
                    printf(", ");
                } 
                temp[i] = 300;
            }
            //Add the difference from our starting heads and the last request in sortedrequests to the total
            total += abs(head - sortedrequests[INT_COUNT-1]);
            for (int i = INT_COUNT-1; i >= 0; i--)
            {
                if (temp[i] != 300)
                {
                    printf("%d",temp[i]);
                    if (i != 0)
                    {
                        printf(", ");
                    } 
                    head = temp[i];
                    temp[i] = 300;
                }
            }
            //Add the difference from the last and the first request in our sorted requests to the total
            total += abs(sortedrequests[INT_COUNT-1]-sortedrequests[0]);
        }
    }
    printf("\n\nLOOK - Total head movements = %d\n",total);
    return -1;
}


/* This function is responsible for scheduling the requests using C-LOOK and tracking the total head movement. */
int clook(int head, int dir){
    int total = 0;
    int index;
    printf("\nC-LOOK DISK SCHEDULING ALGORITHM\n\n");
    int temp[INT_COUNT];
    for (int i = 0; i < INT_COUNT; i++)
    {
        temp[i] = sortedrequests[i];
    }
    if (dir == 0)
    {
        if (head >= temp[19])
        {
            for (int i = 19; i >= 0; i--)
            {
                printf("%d",temp[i]);
                if (i != 0)
                {
                    printf(", ");
                }
            }
            //Calculate the value of the total head movement by subtracting original head value from the first sorted request
            total += abs(head-temp[0]);
        }
        else{
            if (head < temp[0])
            {
                index = -1;
            }
            else {
                for (int i = 0; i < INT_COUNT-1; i++)
                {
                    if (head >= temp[i] && head < temp[i+1])
                    {
                        index = i;
                    }
                }
            }
            for (int i = index; i >= 0; i--)
            {
                printf("%d",temp[i]);
                if (i != INT_COUNT-1)
                {
                    printf(", ");
                } 
                temp[i] = 300;
            }
            //Add the absolute value of the current value of the head minus the first sorted request to the total
            total += abs(head - sortedrequests[0]);
            for (int i = INT_COUNT-1; i >= 0; i--)
            {
                if (temp[i] != 300)
                {
                    printf("%d",temp[i]);
                    if (i != index+1)
                    {
                        printf(", ");
                    } 
                    head = temp[i];
                    temp[i] = 300;
                }
                
            }
            //Add the absolute value of the difference of the first and last sorted request and the difference of the last sorted request and the request at one index ahead to the total
            total += abs(sortedrequests[INT_COUNT-1]-sortedrequests[0]) + abs(sortedrequests[INT_COUNT-1] - sortedrequests[index+1]);
        }
    }
    else{
        if (head <= temp[0])
        {
            for (int i = 0; i < INT_COUNT; i++)
            {
                printf("%d",temp[i]);
                if (i != INT_COUNT-1)
                {
                    printf(", ");
                }
            }
            total += abs(head-temp[19]);
        }
        else{
            if (head > temp[19])
            {
                index = 20;
            }
            else {
                for (int i = 0; i < INT_COUNT-1; i++)
                {
                    if (head == temp[i])
                    {
                        index = i;
                    }
                    else if (head > temp[i] && head < temp[i+1])
                    {
                        index = i+1;
                    }
                }
            }
            for (int i = index; i < INT_COUNT; i++)
            {
                printf("%d",temp[i]);
                if (i != 0)
                {
                    printf(", ");
                } 
                temp[i] = 300;
            }
            //Add the difference from our starting head and the last sorted request to the total
            total += abs(head - sortedrequests[INT_COUNT-1]);
            for (int i = 0; i <= INT_COUNT-1; i++)
            {
                if (temp[i] != 300)
                {
                    printf("%d",temp[i]);
                    if (i != index-1)
                    {
                        printf(", ");
                    } 
                    head = temp[i];
                    temp[i] = 300;
                }
            }
            //Add the absolute value of the difference of the first and last sorted request and the difference of the last sorted request and the request at one index before to the total
            total += abs(sortedrequests[INT_COUNT-1]-sortedrequests[0]) + abs(sortedrequests[0] - sortedrequests[index-1]);
        }
    }
    printf("\n\nC-LOOK - Total head movements = %d\n",total);
    return -1;
}

/* Function for processing all of the various disk scheduling algorithms*/
int main(int argc, char *argv[]){
    //Stores the direction in the form of a number (0 for left, 1 for right)
    int dir;
    //Stores value at head
    int head;
    //Stores total requests
    int i;
    //Makes sure there are 2 arguments given to the program
    if(argc != 3) {
        printf("Incorrect number of arguments \n");
        //Exits program if not
        return -1;
    }
    //Makes sure the direction argument is exactly LEFT or exactly RIGHT
    if (strcmp(argv[2],"LEFT") != 0 && strcmp(argv[2],"RIGHT")!= 0){
        printf("Direction of the head must be LEFT or RIGHT \n");
        //Exits program if not
        return -1;
    }
    //Ensures that the starting head argument is within acceptable range of 0 and 299
    if (atoi(argv[1]) < 0 || atoi(argv[1]) > 299)
    {
        printf("Head must be between 0 and 299 \n");
        //Exits program if not
        return -1;
    }
    //Sets head to the head argument
    head = atoi(argv[1]);
    //Sets the direction to the direction argument
    if (strcmp(argv[2],"LEFT") == 0)
    {
        dir = 0;
    }
    else
    {
        dir = 1;
    }
    //Reads the Requests from the request.bin file
    int mmapfile_fd = open("request.bin", O_RDONLY);
    //Maps memory for storage
    mmapfptr = mmap(0, MEMORY_SIZE, PROT_READ, MAP_PRIVATE, mmapfile_fd, 0);
    //Stores the requests in the array requests and sortedrequests
    for (i = 0; i < INT_COUNT; i++)
    {
        memcpy(requests + i, mmapfptr + INT_SIZE*i, INT_SIZE);
        memcpy(sortedrequests + i, mmapfptr + INT_SIZE*i, INT_SIZE);
    }
    //Unmaps memory
    munmap(mmapfptr, MEMORY_SIZE);
    //Prints the required information for the program
    printf("Total requests = %d \n",i);
    printf("Initial Head Position: %d \n",atoi(argv[1]));
    printf("Direction of Head: %s \n",argv[2]);
    //Sorts the sortedrequests array
    bubbleSort();
    //Calls the 6 disk scheduling algorithms
    fcfs(head);
    sstf(head);
    scan(head,dir);
    cscan(head,dir);
    look(head,dir);
    clook(head,dir);    
}