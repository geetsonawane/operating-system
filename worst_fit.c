#include <stdio.h>

int main() {
    int blockCount, processCount;
    printf("Enter the no. of blocks :- ");
    scanf("%d", &blockCount);

    int blockSize[blockCount];
    printf("Enter the size of block:\n");
    for (int i = 0; i < blockCount; i++) {
        printf("%d :- ", i);
        scanf("%d", &blockSize[i]);
    }

    printf("Enter the no. of processes :- ");
    scanf("%d", &processCount);

    int processSize[processCount], allocation[processCount], fragment[processCount], blockUsed[blockCount];

    for (int i = 0; i < processCount; i++) {
        printf("%d :- ", i);
        scanf("%d", &processSize[i]);
        allocation[i] = -1;
        fragment[i] = 0;
    }

    for (int i = 0; i < blockCount; i++) blockUsed[i] = 0;

    for (int i = 0; i < processCount; i++) {
        int worstIdx = -1;
        for (int j = 0; j < blockCount; j++) {
            if (!blockUsed[j] && blockSize[j] >= processSize[i]) {
                if (worstIdx == -1 || blockSize[j] > blockSize[worstIdx])
                    worstIdx = j;
            }
        }
        if (worstIdx != -1) {
            allocation[i] = worstIdx;
            blockUsed[worstIdx] = 1;
            fragment[i] = blockSize[worstIdx] - processSize[i];
        }
    }

    printf("\nWorst Fit Allocation:\n");
    printf("-----------------------------------------------------------\n");
    printf("| Process No. | Process Size | Block Allocated | Fragment |\n");
    printf("-----------------------------------------------------------\n");

    for (int i = 0; i < processCount; i++) {
        printf("| %-11d | %-12d | ", i, processSize[i]);
        if (allocation[i] != -1) {
            char buffer[30];
            printf("%-14s | %-8d |\n",
                   (sprintf(buffer, "%d (%02d)", blockSize[allocation[i]], allocation[i]), buffer),
                   fragment[i]);
        } else {
            printf("%-14s | %-8s |\n", "NA", "NA");
        }
        printf("-----------------------------------------------------------\n");
    }

    return 0;
}
