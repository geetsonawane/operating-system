#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PAGES 100
#define MAX_FRAMES 10

void display_table_header(int n) {
    printf("+");
    for (int i = 0; i < n; i++) printf("-------+");
    printf("\n");
}

void display_table_rows(int frameStates[MAX_PAGES][MAX_FRAMES], int pageCount, int frameCount) {
    for (int i = frameCount - 1; i >= 0; i--) {
        printf("|");
        for (int j = 0; j < pageCount; j++) {
            if (frameStates[j][i] != -1)
                printf("%5d  |", frameStates[j][i]);
            else
                printf("       |");
        }
        printf("\n");
    }
}

void display_hit_miss_row(int hitStatus[MAX_PAGES], int n) {
    display_table_header(n);
    printf("|");
    for (int i = 0; i < n; i++) {
        printf("%5s  |", hitStatus[i] ? "Hit" : "Miss");
    }
    printf("\n");
    display_table_header(n);
}

void lruPageReplacement(int pages[], int pageCount, int frameCount) {
    int frames[MAX_FRAMES];
    int lastUsed[MAX_FRAMES];
    int frameStates[MAX_PAGES][MAX_FRAMES];
    int hitStatus[MAX_PAGES];
    int faults = 0, hits = 0;

    for (int i = 0; i < frameCount; i++) {
        frames[i] = -1;
        lastUsed[i] = -1;
    }

    for (int i = 0; i < pageCount; i++) {
        int page = pages[i], isHit = 0;

        for (int j = 0; j < frameCount; j++) {
            if (frames[j] == page) {
                isHit = 1;
                lastUsed[j] = i;
                break;
            }
        }

        if (!isHit) {
            int empty = -1;
            for (int j = 0; j < frameCount; j++) {
                if (frames[j] == -1) {
                    empty = j;
                    break;
                }
            }

            if (empty != -1) {
                frames[empty] = page;
                lastUsed[empty] = i;
            } else {
                int lruIndex = 0;
                for (int j = 1; j < frameCount; j++) {
                    if (lastUsed[j] < lastUsed[lruIndex]) {
                        lruIndex = j;
                    }
                }
                frames[lruIndex] = page;
                lastUsed[lruIndex] = i;
            }
            faults++;
        } else {
            hits++;
        }

        for (int j = 0; j < frameCount; j++) {
            frameStates[i][j] = frames[j];
        }
        hitStatus[i] = isHit;
    }

    display_table_header(pageCount);
    display_table_rows(frameStates, pageCount, frameCount);
    display_hit_miss_row(hitStatus, pageCount);

    printf("\nTotal Page Faults: %d\n", faults);
    printf("Total Page Hits: %d\n", hits);
    printf("Page Fault Ratio: %.2f%%\n", (faults * 100.0) / pageCount);
    printf("Page Hit Ratio: %.2f%%\n", (hits * 100.0) / pageCount);
}

int main() {
    int pages[MAX_PAGES], pageCount = 0, frameCount;
    char input[500];

    printf("Enter the reference string (space-separated): ");
    fgets(input, sizeof(input), stdin);

    char *token = strtok(input, " ");
    while (token != NULL && pageCount < MAX_PAGES) {
        pages[pageCount++] = atoi(token);
        token = strtok(NULL, " ");
    }

    printf("Enter number of frames: ");
    scanf("%d", &frameCount);

    lruPageReplacement(pages, pageCount, frameCount);
    return 0;
}
