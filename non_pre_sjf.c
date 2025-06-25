#include <stdio.h>
#include <limits.h>

struct Process {
    int pid, at, bt, ct, tat, wt, completed;
};

// Sort by PID for final table
void sort_by_pid(struct Process p[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (p[j].pid > p[j + 1].pid) {
                struct Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
}

void sjf_non_preemptive(struct Process p[], int n) {
    int time = 0, completed = 0;
    float total_tat = 0, total_wt = 0;

    int order[n];           // Execution order of process index
    int ct_sequence[n];     // Completion time sequence for Gantt chart
    int k = 0;

    for (int i = 0; i < n; i++) p[i].completed = 0;

    while (completed < n) {
        int shortest = -1, min_bt = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && !p[i].completed && p[i].bt < min_bt) {
                min_bt = p[i].bt;
                shortest = i;
            }
        }

        if (shortest == -1) {
            time++;
            continue;
        }

        order[k] = shortest;  // Save the process execution order
        time += p[shortest].bt;

        p[shortest].ct = time;
        p[shortest].tat = p[shortest].ct - p[shortest].at;
        p[shortest].wt = p[shortest].tat - p[shortest].bt;
        p[shortest].completed = 1;

        ct_sequence[k] = time;
        total_tat += p[shortest].tat;
        total_wt += p[shortest].wt;
        k++;
        completed++;
    }

    // Gantt Chart
    printf("\nGantt Chart:\n");

    for (int i = 0; i < k; i++) printf("--------");
    printf("-\n|");

    for (int i = 0; i < k; i++) printf("  P%-2d  |", p[order[i]].pid);

    printf("\n");
    for (int i = 0; i < k; i++) printf("--------");
    printf("-\n");

    // Time labels
    int start = 0;
    printf("%d", start);
    for (int i = 0; i < k; i++) {
        printf("      %2d", ct_sequence[i]);
    }
    printf("\n");

    // Final process table
    sort_by_pid(p, n);

    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }

    printf("\nAverage Turnaround Time: %.2f\n", total_tat / n);
    printf("Average Waiting Time   : %.2f\n", total_wt / n);
}

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process p[n];
    printf("Enter Process ID, Arrival Time, and Burst Time:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d %d %d", &p[i].pid, &p[i].at, &p[i].bt);
    }

    sjf_non_preemptive(p, n);
    return 0;
}
