#include <stdio.h>
#include <stdbool.h>

#define MAX 10

void calculateNeed(int n, int m, int max[][MAX], int alloc[][MAX], int need[][MAX]) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            need[i][j] = max[i][j] - alloc[i][j];
}

void calculateAvailable(int n, int m, int total[], int alloc[][MAX], int avail[]) {
    for (int j = 0; j < m; j++) {
        int sum = 0;
        for (int i = 0; i < n; i++)
            sum += alloc[i][j];
        avail[j] = total[j] - sum;
        if (avail[j] < 0) avail[j] = 0;
    }
}

void printState(int n, int m, int avail[], int max[][MAX], int alloc[][MAX], int need[][MAX]) {
    printf("\nAvailable:\n");
    for (int j = 0; j < m; j++)
        printf("R%d: %d  ", j, avail[j]);

    printf("\n\nMax Need:\n");
    for (int i = 0; i < n; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < m; j++)
            printf("%d ", max[i][j]);
        printf("\n");
    }

    printf("\nAllocation:\n");
    for (int i = 0; i < n; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < m; j++)
            printf("%d ", alloc[i][j]);
        printf("\n");
    }

    printf("\nRemaining Need:\n");
    for (int i = 0; i < n; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < m; j++)
            printf("%d ", need[i][j]);
        printf("\n");
    }
    printf("\n");
}

void findSafeSequence(int n, int m, int avail[], int alloc[][MAX], int need[][MAX]) {
    int work[MAX], safeSeq[MAX], count = 0;
    bool finish[MAX] = {0};
    for (int j = 0; j < m; j++) work[j] = avail[j];

    while (count < n) {
        bool found = false;
        for (int i = 0; i < n; i++) {
            if (!finish[i]) {
                bool canRun = true;
                for (int j = 0; j < m; j++) {
                    if (need[i][j] > work[j]) {
                        canRun = false;
                        break;
                    }
                }
                if (canRun) {
                    for (int j = 0; j < m; j++)
                        work[j] += alloc[i][j];
                    safeSeq[count++] = i;
                    finish[i] = true;
                    found = true;
                }
            }
        }
        if (!found) {
            printf("System is not in safe state.\n");
            return;
        }
    }

    printf("System is in safe state.\nSafe sequence: ");
    for (int i = 0; i < n; i++)
        printf("P%d%s", safeSeq[i], i < n - 1 ? " -> " : "\n");
}

int main() {
    int n, m, input;
    int max[MAX][MAX], alloc[MAX][MAX], need[MAX][MAX], total[MAX], avail[MAX];

    printf("Enter number of processes: "); scanf("%d", &n);
    printf("Enter number of resources: "); scanf("%d", &m);

    printf("\nSelect input type:\n1. Available\n2. Total\nEnter choice: ");
    scanf("%d", &input);

    printf("\nEnter Max Need:\n");
    for (int i = 0; i < n; i++) {
        printf("For process P%d: ", i);
        for (int j = 0; j < m; j++) scanf("%d", &max[i][j]);
    }

    printf("\nEnter Allocation:\n");
    for (int i = 0; i < n; i++) {
        printf("For process P%d: ", i);
        for (int j = 0; j < m; j++) {
            scanf("%d", &alloc[i][j]);
            if (alloc[i][j] > max[i][j]) {
                printf("Error: Allocation exceeds maximum.\n");
                i--;
                break;
            }
        }
    }

    if (input == 1) {
        printf("\nEnter Available:\n");
        for (int j = 0; j < m; j++) {
            printf("R%d: ", j);
            scanf("%d", &avail[j]);
        }
    } else if (input == 2) {
        printf("\nEnter Total:\n");
        for (int j = 0; j < m; j++) {
            printf("R%d: ", j);
            scanf("%d", &total[j]);
        }
        calculateAvailable(n, m, total, alloc, avail);
        printf("\nCalculated Available:\n");
        for (int j = 0; j < m; j++)
            printf("R%d: %d  ", j, avail[j]);
        printf("\n");
    } else {
        printf("Invalid input method.\n");
        return 1;
    }

    calculateNeed(n, m, max, alloc, need);
    printState(n, m, avail, max, alloc, need);
    findSafeSequence(n, m, avail, alloc, need);
    return 0;
}

/*
Output:
bhupesh-suryawanshi@bhupesh-suryawanshi:/media/bhupesh-suryawanshi/New Volume/IT/TY/SEM 6/OS/Practical Exam$ gcc bankers.c
bhupesh-suryawanshi@bhupesh-suryawanshi:/media/bhupesh-suryawanshi/New Volume/IT/TY/SEM 6/OS/Practical Exam$ ./a.out
Enter number of processes: 5
Enter number of resources: 3

Select input type:
1. Available
2. Total
Enter choice: 2

Enter Max Need:
For process P0: 7 5 3
For process P1: 3 2 2
For process P2: 9 0 2
For process P3: 4 2 2
For process P4: 5 3 3

Enter Allocation:
For process P0: 0 1 0
For process P1: 2 0 0
For process P2: 3 0 2
For process P3: 2 1 1
For process P4: 0 0 2

Enter Total:
R0: 10
R1: 5
R2: 7

Calculated Available:
R0: 3  R1: 3  R2: 2  

Available:
R0: 3  R1: 3  R2: 2  

Max Need:
P0: 7 5 3 
P1: 3 2 2 
P2: 9 0 2 
P3: 4 2 2 
P4: 5 3 3 

Allocation:
P0: 0 1 0 
P1: 2 0 0 
P2: 3 0 2 
P3: 2 1 1 
P4: 0 0 2 

Remaining Need:
P0: 7 4 3 
P1: 1 2 2 
P2: 6 0 0 
P3: 2 1 1 
P4: 5 3 1 

System is in safe state.
Safe sequence: P1 -> P3 -> P4 -> P0 -> P2

bhupesh-suryawanshi@bhupesh-suryawanshi:/media/bhupesh-suryawanshi/New Volume/IT/TY/SEM 6/OS/Practical Exam$ ./a.out
Enter number of processes: 5
Enter number of resources: 3

Select input type:
1. Available
2. Total
Enter choice: 1

Enter Max Need:
For process P0: 7 5 3
For process P1: 3 2 2
For process P2: 9 0 2
For process P3: 4 2 2
For process P4: 5 3 3

Enter Allocation:
For process P0: 0 1 0
For process P1: 2 0 0
For process P2: 3 0 2
For process P3: 2 1 1
For process P4: 0 0 2

Enter Available:
R0: 3   
R1: 3
R2: 2

Available:
R0: 3  R1: 3  R2: 2  

Max Need:
P0: 7 5 3 
P1: 3 2 2 
P2: 9 0 2 
P3: 4 2 2 
P4: 5 3 3 

Allocation:
P0: 0 1 0 
P1: 2 0 0 
P2: 3 0 2 
P3: 2 1 1 
P4: 0 0 2 

Remaining Need:
P0: 7 4 3 
P1: 1 2 2 
P2: 6 0 0 
P3: 2 1 1 
P4: 5 3 1 

System is in safe state.
Safe sequence: P1 -> P3 -> P4 -> P0 -> P2

bhupesh-suryawanshi@bhupesh-suryawanshi:/media/bhupesh-suryawanshi/New Volume/IT/TY/SEM 6/OS/Practical Exam$ ./a.out
Enter number of processes: 4
Enter number of resources: 3

Select input type:
1. Available
2. Total
Enter choice: 2

Enter Max Need:
For process P0: 2 1 1
For process P1: 3 4 4
For process P2: 3 1 1
For process P3: 1 1 1

Enter Allocation:
For process P0: 1 0 1
For process P1: 2 1 2
For process P2: 3 0 0
For process P3: 1 0 1

Enter Total:
R0: 9
R1: 2
R2: 5

Calculated Available:
R0: 2  R1: 1  R2: 1  

Available:
R0: 2  R1: 1  R2: 1  

Max Need:
P0: 2 1 1 
P1: 3 4 4 
P2: 3 1 1 
P3: 1 1 1 

Allocation:
P0: 1 0 1 
P1: 2 1 2 
P2: 3 0 0 
P3: 1 0 1 

Remaining Need:
P0: 1 1 0 
P1: 1 3 2 
P2: 0 1 1 
P3: 0 1 0 

System is not in safe state.
bhupesh-suryawanshi@bhupesh-suryawanshi:/media/bhupesh-suryawanshi/New Volume/IT/TY/SEM 6/OS/Practical Exam$ 

*/
