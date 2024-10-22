
//preprocessor header files

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

//global variables so we have access to them throughout the code(added a few more defines)

#define EXIT 0
#define INVALID -1
#define CPU 1
#define PID 5
#define RACE 2
#define BANKER 3
#define PROCESS 5
#define RESOURCE 3
#define TRUE 1
#define FALSE 0
#define MEMORY 4
#define FIRST 0
#define BEST 1
#define WORST 2
#define NEXT 3
#define PAGE 5
#define FIFO 0
#define LRU 1
#define FRAMES 4
#define DISK 6
#define FCFS 0
#define SSTF 1
#define REQUEST 8

//calling all our functions (added bankers algorithm and systemState)

int displayMenu();
void cpuScheduling();
void fcfs(int process[], int at[], int bt[]);
void sjf(int process[], int at[], int bt[]);
void displaySchedule();
void raceCondition();
void *threadFuncOne();
void *threadFuncTwo();
void bankersAlgorithm();
void systemState(int feasible[], int safe[]);
void memoryManagement();
void displayProcess(int allocation[], int processes, int processSize[]);
void nextFit(int blockSize[], int blocks, int processSize[], int processes);
void firstFit(int blockSize[], int blocks, int processSize[], int processes);
void bestFit(int blockSize[], int blocks, int processSize[], int processes);
void worstFit(int blockSize[], int blocks, int processSize[], int processes);
void pageReplacement();
void lru();
int findLRU(int time[]);
void fifo();
void displayPages(int page, int allocation[]);
void diskScheduling();
void diskFcfs(int requests[], int head);
void diskSstf(int requests[], int head);

int resource = 5;

int main(){
    //declaring a variable called choice
    int choice = INVALID;
    //creating an array to store process IDS
    int process[PID] = {1,2,3,4,5};
    //our loop that will keep displaying the menu until case exit is entered by the user
    while(choice != EXIT){
        choice = displayMenu();
        switch(choice)
        {
            //case CPU that calls cpuScheduling function
            case CPU:
                cpuScheduling();
                break;
                //added the RACE condition case
            case RACE:
                raceCondition();
                break;
            //added banker case
            case BANKER:
                bankersAlgorithm();
                break;
            case MEMORY:
                memoryManagement();
                break;
            //added PAGE case so we can call the pageReplacement function in the correct circumstance
            case PAGE:
                pageReplacement();
                break;
            case DISK:
                diskScheduling();
            case EXIT:
                exit(EXIT);
        }
    }
    //must return an int variable
    return EXIT;
}
//creating our display menu
int displayMenu(){
    //initialzing choice to -1
    int choice = INVALID;
    //a loop that keeps going until valid choice is entered
    while(choice == INVALID){
        //displaying our menu
        printf("\n\t\t\t********** Operating System Management MENU **********\n");
        printf("1. CPU Scheduling\n");
        printf("2. Race Condition\n");
        //added bankers algorithm in the menu choice
        printf("3. Banker's Algorithm\n");
        printf("4. Memory Management\n");
        //added #5 selection for the menu 
        printf("5. Page Replacement\n");
        printf("6. Disk Scheduling\n"); //option for disk scheduling
        printf("0. Exit\n");
        printf("Select a choice from the menu: ");
        scanf("%d", &choice);
        //not valid input
        if(choice < 0 || choice > DISK){
            //therefore loop will keep going because choice is invalid
          choice = INVALID;
        }
    }
    return choice;
}

void diskScheduling(){
    printf("\n******************* Disk Scheduling *******************\n");

    // declaring array and initializing values to the array
    int requests[REQUEST] = {146, 89, 24, 70, 102, 13, 51, 134};
    //declaring variable head and making it equal to 50
    int head = 50;
    int algorithm;

    //creating looping consruct  to loop for each for each of the two disk scheduling algorithms 
    for (int algorithm = FCFS; algorithm <= SSTF; algorithm++) {
        //decision making logic based on value of the looping variable 
        switch (algorithm) {
            case FCFS:
                printf("\n******************* FCFS*******************\n");
                diskFcfs(requests, head);
                break;
            case SSTF:
                printf("\n******************* SSTF *******************\n");
                diskSstf(requests, head);
                break;
        }
    }
}
//creating diskFcfs function
void diskFcfs(int requests[], int head){
    //creating varaibles 
    int seek = 0;
    int track = 0;
    int distance = 0;
    //start always is at the head
    int start = head;
    //iterating through number of requests 
    for(int i = 0; i < REQUEST; i++){
        //setting track equal to the current element in array requests
        track = requests[i];
        //figuring out the distance between request locations
        distance = abs(head - track);
        //incrementing seek by adding the distance to seek 
        seek = seek + distance;
        //head location is equal to current track location
        head = track;
    }
    //Using a looping construct, display to the console the seek
    //sequence stored in array requests and total seek time
    printf("Starting head: %d\n", start);
    printf("Seek sequence:\n");
    for (int i = 0; i < REQUEST; i++) {
        printf("%d ", requests[i]);
    }
    printf("\nTotal: %d\n", seek);
}

void diskSstf(int requests[], int head) {
    int sequence[REQUEST]; //One-dimensional array to store the service request sequence 
    int distance[REQUEST]; //One-dimensional array to store the distance between service requests
    int seek = 0; //Variable to store the number of seek operations
    int start = head; //ariable to store the starting head location
    int minVal, minValIdx, seqIdx;//minimum distance value, minimum distance index,  sequence index

    for (int i = 0; i < REQUEST; i++) { //looping construct, iterate through the number of REQUESTS
        for(int j = 0; j < REQUEST; j++){ //Calculate the distance between requests, using a looping construct, iterate through the number of REQUESTS
            distance[j] = abs(head - requests[j]); //Update array distance
        }
        minVal = distance[0]; //Update variable minVal
        minValIdx = 0; //Update variable minValIdx
        for (int j = 1; j < REQUEST; j++) { //terate through the number of REQUESTS to find the minimum value
            if (minVal > distance[j]) { //If minVal is greater than the value in array distance, at index of the loop control variable
                minVal = distance[j]; //Update variable minVal
                minValIdx = j; //Update variable minValIdx
            }
        }
        //Update the sequence array
        sequence[seqIdx] = minVal; 
        seqIdx++; //increment 
        head = requests[minValIdx]; //update head 
        requests[minValIdx] = 999; //this is so the request is not processed again
        //calc head movements
        seek += abs(start - sequence[seqIdx - 1]);
        start = sequence[seqIdx - 1];
        //print statements 
        printf("Starting head: %d\n", head);
        printf("Seek Sequence:\n");
    for (int i = 0; i < REQUEST; i++) {
        printf("%d ", sequence[i]);
    }
    printf("\nTotal: %d\n", seek);
    }
}
//creating page replacement function 
void pageReplacement(){
    //for loop to see which function should be called 
    for(int algorithm = FIFO; algorithm <= LRU; algorithm++){
        if(algorithm == FIFO){
            fifo();
        }
        else if (algorithm == LRU){
            lru();
        }
    }
}

//creating first in first out function 
void fifo(){
    printf("\n********** FIFO Page Replacement **********\n\n");
    //creating array 
    int pageRequests[] = {2, 3, 8, 4, 5, 6, 5, 7, 1, 8, 3, 1, 4, 2, 6};
    //setting counter
    int pageFaults = 0;
    //creating another array
    int allocation[FRAMES];
    //initiliazing the allocation array 
    memset(allocation, INVALID, sizeof(allocation));
    //setting another counter to 0
    int present = 0;
    int pages = sizeof(pageRequests) / sizeof(pageRequests[0]);
    //creating nested for loop to iterate through number of pages and frames
    for(int i = 0; i < pages; i++){
        present = 0;
        for(int j = 0; j < FRAMES; j++){
            //if current page request is equal to current frame
            if(allocation[j] == pageRequests[i]){
                present = 1;
                pageFaults--;
                break;
            }
        }
        //if num of page faults is less than or equal to the frames and variable present is equal to 0
        if(pageFaults <= FRAMES && present == 0){
            allocation[pageFaults] = pageRequests[i];
            pageFaults++;
        }
        //if present is 0
        else if(present == 0){
            allocation[(pageFaults - 1) % FRAMES] = pageRequests[i];
        }
        displayPages(pageRequests[i], allocation);
    }
    //printing the total page faults from the counter we incremented 
    printf("\nTotal Page Faults: %d\n", pageFaults);
}

//creating lru function 
void lru() {
    printf("\n********** LRU Page Replacement **********\n\n");
    int pageRequests[] = {2, 3, 8, 4, 5, 6, 5, 7, 1, 8, 3, 1, 4, 2, 6};
    int pageFaults = 0;
    int allocation[FRAMES];
    int pages = sizeof(pageRequests) / sizeof(pageRequests[0]);
    memset(allocation, INVALID, sizeof(allocation));
    int counter = 0;
    int time[10];
    memset(time, INVALID, sizeof(time));
    int flag1 = 0, flag2 = 0, position = 0;
    //creating nested for loop to iterate through pages and frames
    for (int i = 0; i < pages; i++) {
        flag1 = 0, flag2 = 0;
        for (int j = 0; j < FRAMES; j++) {
            if (allocation[j] == pageRequests[i]) {
                counter++;
                time[j] = counter;
                flag1 = 1;
                flag2 = 1;
                break;
            }
        }
        //if flag 1 = 0 iterate through frames
        if (flag1 == 0) {
            for (int j = 0; j < FRAMES; j++) {
                //if index in allocation array is invalid
                if (allocation[j] == INVALID) {
                    counter++;
                    pageFaults++;
                    allocation[j] = pageRequests[i];
                    time[j] = counter;
                    flag2 = 1;
                    break;
                }
            }
        }
        //if flag 2 is 0
        if (flag2 == 0) {
            position = findLRU(time);
            counter++;
            pageFaults++;
            allocation[position] = pageRequests[i];
            time[position] = counter;
        }
        //calling display function 
        displayPages(pageRequests[i], allocation);
    }
    //print the counter for total page faults
    printf("\nTotal Page Faults: %d\n", pageFaults);
}

int findLRU(int time[]){
    int position = 0;
    int minimum = time[0];
    for(int i = 1; i < FRAMES; i++){
        if(time[i] < minimum){
            minimum = time[i];
            position = i;
        }
    }
    return position;
}

void displayPages(int page, int allocation[]) {
    printf("\nPage Number: %d\n", page);
    printf("Memory Block Assigned:\n");
    for (int i = 0; i < FRAMES; i++) {
        if (allocation[i] == INVALID) {
            printf("-");
        } else {
            printf("%d", allocation[i]);
        }
        printf("\t");
    }
    printf("\n");
}

//created banker algorithm function
void bankersAlgorithm(){
//we had to create a 2d array called allocation
    int allocation[PROCESS][RESOURCE] = {{0, 0, 2},
                                         {3, 0, 2},
                                         {0, 1, 0},
                                         {2, 1, 1},
                                         {2, 0, 0}};
//and another 2d array called max demand
    int maxDemand[PROCESS][RESOURCE] = {{4, 3, 3},
                                        {9, 0, 2},
                                        {7, 5, 3},
                                        {2, 2, 2},
                                        {3, 2, 2}};
//and a 1d array called avaialable
    int available[RESOURCE] = {2, 4, 6};
//created a 2d array for space and 2 1d arrays for our for loops
    int need[PROCESS][RESOURCE];
    int feasible[PROCESS];
    int safe[PROCESS];
//initiliazed index to 0
    int safeIdx = 0;
//iterating through number of processess
    for (int p = 0; p < PROCESS; p++) {
        feasible[p] = FALSE;
    }
//calculating needed resources
    for (int p = 0; p < PROCESS; p++) {
//iterating for number of resources
        for (int r = 0; r < RESOURCE; r++) {
            need[p][r] = maxDemand[p][r] - allocation[p][r];
        }
    }
//this is beginning of the algorithm we iterate through number of processess var i
    for (int i = 0; i < PROCESS; i++) {
        //iterate through number of processes var p
        for (int p = 0; p < PROCESS; p++) {
            //if statement for current element in array
            if (feasible[p] == FALSE) {
                int isUnsafe = FALSE;
                //iterate number of resources var r
                for (int r = 0; r < RESOURCE; r++) {
                    //if statement that if this is met unsafe is true
                    if (need[p][r] > available[r]) {
                        isUnsafe = TRUE;
                        break;
                    }
                }
//otherwise increment safe idx
                if (isUnsafe == FALSE) {
                    safe[safeIdx++] = p;
                    for (int r = 0; r < RESOURCE; r++) {
                        available[r] += allocation[p][r];
                    }
                    feasible[p] = TRUE;
                }
            }
        }
    }

    systemState(feasible, safe);
}

void memoryManagement(){


    // Loop through memory management algorithms
    for (int algorithm = FIRST; algorithm <= NEXT; algorithm++) {
        // Declare and initialize arrays
        int blockSize[] = {70, 20, 45, 65, 40, 80};
        int processSize[] = {15, 35, 25, 45, 60, 20};
        int blocks = sizeof(blockSize) / sizeof(blockSize[0]);
        int processes = sizeof(processSize) / sizeof(processSize[0]);

        switch(algorithm) {
            case FIRST:
                printf("\n  ********** First Fit **********\n\n");
                firstFit(blockSize, blocks, processSize, processes);
                break;
            case BEST:
                printf("\n  ********** Best Fit **********\n\n");
                bestFit(blockSize, blocks, processSize, processes);
                break;
            case WORST:
                printf("\n  ********** Worst Fit **********\n\n");
                worstFit(blockSize, blocks, processSize, processes);
                break;
            case NEXT:
                printf("\n  ********** Next Fit **********\n\n");
                nextFit(blockSize, blocks, processSize, processes);
                break;
        }
    }
}

void displayProcess(int allocation[], int processes, int processSize[])
{
    printf("\nProcess No. \tProcess Size\tBlock no.\n");
    for(int i = 0; i < processes; i++)
    {
        printf(" %d\t\t %d\t\t", (i + 1), processSize[i]);

        if(allocation[i] == INVALID)
        {
           printf("Not allocated\n");
        }
        else
        {
            printf("%d\n", (allocation[i] + 1));
        }

    }
    printf("\n");
}


void nextFit(int blockSize[], int blocks, int processSize[], int processes) {
    int allocation[processes];
    memset(allocation, INVALID, sizeof(allocation));

    int id = 0; // Block id for allocation

    for (int i = 0; i < processes; i++) {
        while (id < blocks) {
            if (blockSize[id] >= processSize[i]) {
                allocation[i] = id;
                blockSize[id] -= processSize[i];
                break;
            }
            id = (id + 1) % blocks;
        }
    }

    // Display allocated memory blocks for each process
    displayProcess(allocation, processes, processSize);
}

void firstFit(int blockSize[], int blocks, int processSize[], int processes) {
    int allocation[processes];
    memset(allocation, INVALID, sizeof(allocation));

    for (int i = 0; i < processes; i++) {
        for (int j = 0; j < blocks; j++) {
            if (blockSize[j] >= processSize[i]) {
                allocation[i] = j;
                blockSize[j] -= processSize[i];
                break;
            }
        }
    }

    // Display allocated memory blocks for each process
    displayProcess(allocation, processes, processSize);
}

void bestFit(int blockSize[], int blocks, int processSize[], int processes) {
    int allocation[processes];
    memset(allocation, INVALID, sizeof(allocation));

    for (int i = 0; i < processes; i++) {
        int bestIdx = INVALID;

        for (int j = 0; j < blocks; j++) {
            if (blockSize[j] >= processSize[i]) {
                if (bestIdx == INVALID)
                {
                    bestIdx = j;
                }
                else if (blockSize[bestIdx] >= blockSize[j])
//                else if(blockSize[j] < blockSize[bestIdx])
                {
                    bestIdx = j;
                }
            }
        }

        if (bestIdx != INVALID) {
            allocation[i] = bestIdx;
            blockSize[bestIdx] -= processSize[i];
        }
    }

    // Display allocated memory blocks for each process
    displayProcess(allocation, processes, processSize);
}

void worstFit(int blockSize[], int blocks, int processSize[], int processes) {
    int allocation[processes];
    memset(allocation, INVALID, sizeof(allocation));

    for (int i = 0; i < processes; i++) {
        int worstIdx = INVALID;

        for (int j = 0; j < blocks; j++) {
            if (blockSize[j] >= processSize[i]) {
                if (worstIdx == INVALID || blockSize[j] > blockSize[worstIdx]) {
                    worstIdx = j;
                }
            }
        }

        if (worstIdx != INVALID) {
            allocation[i] = worstIdx;
            blockSize[worstIdx] -= processSize[i];
        }
    }

    // Display allocated memory blocks for each process
    displayProcess(allocation, processes, processSize);
}

void systemState(int feasible[], int safe[]) {
    int isSafe = TRUE;
//iterate for number of processess
    for (int i = 0; i < PROCESS; i++) {
        //if value is stored in array feasible, idx of loop controll is equal to 0
        if (feasible[i] == FALSE) {
            //not safe
            isSafe = FALSE;
            //prints system is not safe
            printf("The system is not safe.\n");
            break;
        }
    }
//if safe
    if (isSafe) {
        printf("Safe resource allocation sequence:\n");
        for (int i = 0; i < PROCESS; i++) {
            //prints out the algorithm
            printf("P%d-> ", safe[i]);
        }
        printf("\n");
    }
}

//created a function that is return type void
void raceCondition(){
    //declared two variables with data type pthread_t
    pthread_t threadOne, threadTwo;
    //calling pthread create functions with the appropriate paramaters
    pthread_create(&threadOne, NULL, threadFuncOne, NULL);
    pthread_create(&threadTwo, NULL, threadFuncTwo, NULL);
    //calling pthread join functions with appropriate parameters
    pthread_join(threadOne, NULL);
    pthread_join(threadTwo, NULL);
    //printing out the final value
    printf("The Final Value is: %d\n", resource);

}

//creating thread function one with a pointer
void *threadFuncOne(){
    //decalring threadId variable and initiliazing it to 1
    int threadId = 1;
    //decalring another integer variable and initializing it to be equal to the resource variable
    int fOne = resource;
    //printing out values and incrementing fOne by 1
    printf("Thread %d: Initial Value: %d\n", threadId, fOne);
    fOne++;
    printf("Thread %d: Updated value of local variable: %d\n", threadId, fOne);
    sleep(1);
    resource = fOne;
    printf("Thread %d: Final value is: %d\n", threadId, resource);
    //calling exit function and passing in NULL as the parameter
    pthread_exit(NULL);
}

//creating thread function 2
void *threadFuncTwo(){
    //initializing threadId to 2 in this function
    int threadId = 2;
    int fTwo = resource;
    //printing out values and decrementing
    printf("Thread %d: Initial Value: %d\n", threadId, fTwo);
    fTwo--;
    printf("Thread %d: Updated value of local variable: %d\n", threadId, fTwo);
    sleep(1);
    resource = fTwo;
    printf("Thread %d: Final value is: %d\n", threadId, resource);
    pthread_exit(NULL);
}


//creating out cpuScheduling function
void cpuScheduling(){
    //we created three arrays that will hold IDS and times
    int process[PID] = {1,2,3,4,5};
    int arrvTime[PID] = {0,2,4,6,7};
    int burstTime[PID] = {8,5,10,2,3};
    //we call the two algorithm functions to execute at this point in the code
    fcfs(process,arrvTime,burstTime);
    sjf(process,arrvTime,burstTime);
}
//now we must make the first come first serve algorithm
void fcfs(int process[],int at[], int bt[]){
//first step is to create two arrays that will hold the wait time and the turn around time
    int wt[PID];
    int tat[PID];
    //we inilialize the wait time array to start at 0
    wt[0] = 0;
//easier to display info with the title
	printf("\n************* FCFS *************\n");

    //calculate the wait times for each process
    for(int i = 1; i < PID; i++){
        wt[i] = bt[i-1] + wt[i-1];
    }

    //calculate the turnaround times for each process
    for( int i = 0; i < PID; i++){
        tat[i] = bt[i] + wt[i];
    }
    //then we call the display schedule function to calculate averages and print them out
    displaySchedule(process, at,bt,wt,tat);
    return;
}
//creating shortest job first algorithm
void sjf(int process[], int at[], int bt[]){
//we create two arrays to hold wait times and turnaround times
    int wt[PID];
    int tat[PID];
    //we initialize idx to 0 and we also made a temporary variable to hold info for sorting
    int idx = 0;
    int temp = 0;
//easier to read info with a title
	printf("\n************* SJF *************\n");
//this is our sorting process based on burst time using a selection sort(wow cs1 material .o.)
    for(int i = 0; i < PID; i++) {
//idx is equal to index i
        idx = i;
//nested for loop
        for(int j = i + 1; j < PID; j++){
            if(bt[j] < bt[idx]){
                idx = j;
            }
        }
//swapping burst times
        temp = bt[i];
        bt[i] = bt[idx];
        bt[idx] = temp;
//swapping process ids
        temp = process[i];
        process[i] = process[idx];
        process[idx] = temp;
//swapping arrival times
        temp = at[i];
        at[i] = at[idx];
        at[idx] = temp;

    }
//making the first index of the wait time array equal to 0
    wt[0] = 0;
//calculate wait times after selection sort
    for(int i = 1; i < PID; i++){
        wt[i] = bt[i-1] + wt[i-1];
    }
//calculate turnaround time after selection sort
    for(int i = 0; i < PID; i++){
        tat[i] = bt[i] + wt[i];
    }
//calling display schedule function to calculate averages and print out all info
    displaySchedule(process, at, bt, wt, tat);

}
//creating displaySchedule function
void displaySchedule(int process[], int at[], int bt[], int wt[], int tat[]){
    // initialize local variables to 0 to get rid of memory location garbage
    int totWt = 0;
    int totTat = 0;
    float avgWt = 0.0;
    float avgTat = 0.0;
//display all info for each process
    for(int i = 0; i < PID; i++){

        totWt += wt[i];
        totTat += tat[i];
        printf("Process %d:\nArrival Time: %d\nBurst Time: %d\nWait Time: %d\nTurnaround Time: %d\n\n", process[i], at[i], bt[i], wt[i], tat[i]);

    }
    //calc all the averages
    avgWt = (float)totWt / PID;
    avgTat = (float)totTat / PID;
//print out the averages
    printf("Average wait time is %.2f\n", avgWt);
    printf("Average turnaround time is %.2f\n", avgTat);
}



