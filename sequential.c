#include <stdio.h>

#define MAX_FILES 100
#define MAX_BLOCKS 1000

typedef struct {
    int start, length;
} File;

int main() {
    File files[MAX_FILES];
    int block_map[MAX_BLOCKS] = {0};
    int num_files, i, j;

    printf("Enter number of files: ");
    scanf("%d", &num_files);

    for (i = 0; i < num_files; i++) {
        printf("Enter number of blocks occupied by file%d: ", i + 1);
        scanf("%d", &files[i].length);

        printf("Enter the starting block of file%d: ", i + 1);
        scanf("%d", &files[i].start);

        int overlap = 0;
        for (j = files[i].start; j < files[i].start + files[i].length; j++) {
            if (j >= MAX_BLOCKS || block_map[j]) {
                overlap = 1;
                break;
            }
        }

        if (overlap) {
            printf("❌ Error: Blocks from %d to %d are already allocated or out of range!\n",
                   files[i].start, files[i].start + files[i].length - 1);
            i--; continue;
        }

        for (j = files[i].start; j < files[i].start + files[i].length; j++)
            block_map[j] = 1;

        printf("✅ File%d allocated successfully. Blocks allocated: ", i + 1);
        for (j = 0; j < files[i].length; j++)
            printf("%d ", files[i].start + j);
        printf("\n");
    }

    // Allocation Table
    printf("\n-----------------------------------\n");
    printf("| Filename | Start block | Length |\n");
    printf("-----------------------------------\n");
    for (i = 0; i < num_files; i++) {
        printf("| %-8d | %-11d | %-6d |\n", i + 1, files[i].start, files[i].length);
        printf("-----------------------------------\n");
    }

    return 0;
}
