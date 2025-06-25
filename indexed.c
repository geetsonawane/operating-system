#include <stdio.h>

#define MAX_FILES 100
#define MAX_BLOCKS 1000

typedef struct {
    int index_block;
    int length;
    int blocks[MAX_BLOCKS];
} File;

int main() {
    File files[MAX_FILES];
    int block_map[MAX_BLOCKS] = {0};
    int num_files, i, j;

    printf("Enter number of files: ");
    scanf("%d", &num_files);

    for (i = 0; i < num_files; i++) {
        printf("Enter index block of file%d: ", i + 1);
        scanf("%d", &files[i].index_block);

        if (files[i].index_block >= MAX_BLOCKS || block_map[files[i].index_block]) {
            printf("❌ Error: Index block %d is already allocated or out of range!\n", files[i].index_block);
            i--; continue;
        }

        printf("Enter number of blocks occupied by file%d: ", i + 1);
        scanf("%d", &files[i].length);

        int overlap = 0;
        printf("Enter the blocks of file%d: ", i + 1);
        for (j = 0; j < files[i].length; j++) {
            scanf("%d", &files[i].blocks[j]);
            if (files[i].blocks[j] >= MAX_BLOCKS || block_map[files[i].blocks[j]])
                overlap = 1;
        }

        if (overlap) {
            printf("❌ Error: One or more blocks are already allocated or invalid!\n");
            i--; continue;
        }

        block_map[files[i].index_block] = 1;
        for (j = 0; j < files[i].length; j++)
            block_map[files[i].blocks[j]] = 1;

        printf("✅ File%d allocated successfully. Blocks allocated (index: %d): ", i + 1, files[i].index_block);
        for (j = 0; j < files[i].length; j++)
            printf("%d ", files[i].blocks[j]);
        printf("\n");
    }

    // Allocation Table
    printf("\n-----------------------------\n");
    printf("| Filename | Index | Length |\n");
    printf("-----------------------------\n");
    for (i = 0; i < num_files; i++) {
        printf("| %-8d | %-5d | %-6d |\n", i + 1, files[i].index_block, files[i].length);
        printf("-----------------------------\n");
    }

    return 0;
}
