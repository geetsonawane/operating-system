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

int findOptimalIndex(int frames[], int frameCount, int pages[], int currentIndex, int pageCount) {
    int farthestIndex = -1, farthestDist = -1;
    for (int i = 0; i < frameCount; i++) {
        int j;
        for (j = currentIndex + 1; j < pageCount; j++) {
            if (frames[i] == pages[j]) {
                if (j > farthestDist) {
                    farthestDist = j;
                    farthestIndex = i;
                }
                break;
            }
        }
        if (j == pageCount) return i; // Not used again
    }
    return farthestIndex == -1 ? 0 : farthestIndex;
}

void optimalPageReplacement(int pages[], int pageCount, int frameCount) {
    int frames[MAX_FRAMES];
    int frameStates[MAX_PAGES][MAX_FRAMES];
    int hitStatus[MAX_PAGES];
    int faults = 0, hits = 0;

    for (int i = 0; i < frameCount; i++) frames[i] = -1;

    for (int i = 0; i < pageCount; i++) {
        int page = pages[i], isHit = 0;

        for (int j = 0; j < frameCount; j++) {
            if (frames[j] == page) {
                isHit = 1;
                break;
            }
        }

        if (!isHit) {
            int replaced = 0;
            for (int j = 0; j < frameCount; j++) {
                if (frames[j] == -1) {
                    frames[j] = page;
                    replaced = 1;
                    break;
                }
            }
            if (!replaced) {
                int indexToReplace = findOptimalIndex(frames, frameCount, pages, i, pageCount);
                frames[indexToReplace] = page;
            }
            faults++;
        } else hits++;

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

    optimalPageReplacement(pages, pageCount, frameCount);
    return 0;
}
