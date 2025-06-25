#include <stdio.h>
#include <string.h>

#define MAX_FILES 100
#define MAX_FILENAME 20
#define MAX_BLOCKS 100

// Structure for Linked File Allocation
typedef struct {
    char filename[MAX_FILENAME];
    int start_block;
    int length;
    int blocks[MAX_BLOCKS];
} LinkedFile;

int main() {
    int n, i, j;
    LinkedFile files[MAX_FILES];
    int block_map[MAX_BLOCKS] = {0};

    printf("Enter number of files: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        printf("\nEnter name of file%d: ", i + 1);
        scanf("%s", files[i].filename);

        printf("Enter starting block of file%d: ", i + 1);
        scanf("%d", &files[i].start_block);

        if (files[i].start_block >= MAX_BLOCKS || block_map[files[i].start_block] == 1) {
            printf("❌ Error: Starting block already in use or invalid. Try again.\n");
            i--;
            continue;
        }

        printf("Enter number of blocks occupied by file%d: ", i + 1);
        scanf("%d", &files[i].length);

        printf("Enter %d block numbers: ", files[i].length);
        int overlap = 0;
        for (j = 0; j < files[i].length; j++) {
            scanf("%d", &files[i].blocks[j]);
            if (files[i].blocks[j] >= MAX_BLOCKS || block_map[files[i].blocks[j]] == 1) {
                overlap = 1;
            }
        }

        if (overlap) {
            printf("❌ Error: One or more blocks are already used or invalid. Try again.\n");
            i--;
            continue;
        }

        // Mark blocks used
        block_map[files[i].start_block] = 1;
        for (j = 0; j < files[i].length; j++) {
            block_map[files[i].blocks[j]] = 1;
        }

        printf("✅ File '%s' allocated successfully. Block chain: %d -> ", files[i].filename, files[i].start_block);
        for (j = 0; j < files[i].length; j++) {
            printf("%d", files[i].blocks[j]);
            if (j < files[i].length - 1) printf(" -> ");
        }
        printf("\n");
    }

    // Display allocation table
    printf("\n---------------------------------------------\n");
    printf("| Filename | Start | Length | Block Chain   |\n");
    printf("---------------------------------------------\n");
    for (i = 0; i < n; i++) {
        printf("| %-8s | %-5d | %-6d | ", files[i].filename, files[i].start_block, files[i].length);
        for (j = 0; j < files[i].length; j++) {
            printf("%d", files[i].blocks[j]);
            if (j < files[i].length - 1) printf(" -> ");
        }
        printf("\n");
    }
    printf("---------------------------------------------\n");

    return 0;
}
