#include <stdio.h>
#include <limits.h>

struct Process {
    int pid, at, bt, priority, ct, tat, wt, completed;
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

void priority_non_preemptive(struct Process p[], int n, int high_priority_is_higher_number) {
    int time = 0, completed = 0;
    float total_tat = 0, total_wt = 0;

    int order[n];        // Order of executed processes
    int ct_seq[n];       // Completion times of each process in execution order
    int k = 0;

    for (int i = 0; i < n; i++) p[i].completed = 0;

    while (completed < n) {
        int selected = -1;
        int best_priority = high_priority_is_higher_number ? INT_MIN : INT_MAX;

        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && !p[i].completed) {
                if ((high_priority_is_higher_number && p[i].priority > best_priority) ||
                    (!high_priority_is_higher_number && p[i].priority < best_priority)) {
                    best_priority = p[i].priority;
                    selected = i;
                } else if (p[i].priority == best_priority && p[i].at < p[selected].at) {
                    selected = i; // tie-breaker: earlier arrival
                }
            }
        }

        if (selected == -1) {
            time++;
            continue;
        }

        // Gantt tracking
        order[k] = selected;
        time += p[selected].bt;
        ct_seq[k] = time;
        k++;

        // Process calculations
        p[selected].ct = time;
        p[selected].tat = p[selected].ct - p[selected].at;
        p[selected].wt = p[selected].tat - p[selected].bt;
        p[selected].completed = 1;

        total_tat += p[selected].tat;
        total_wt += p[selected].wt;
        completed++;
    }

    // Print Gantt Chart
    printf("\nGantt Chart:\n-");
    for (int i = 0; i < k; i++) printf("--------");
    printf("\n|");
    for (int i = 0; i < k; i++) printf("   P%-2d |", p[order[i]].pid);
    printf("\n-");
    for (int i = 0; i < k; i++) printf("--------");
    printf("\n");

    printf("0");
    for (int i = 0; i < k; i++) {
        printf("      %2d", ct_seq[i]);
    }
    printf("\n");

    // Print process table
    sort_by_pid(p, n);

    printf("\nPID\tAT\tBT\tPriority\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t\t%d\t%d\t%d\n",
               p[i].pid, p[i].at, p[i].bt, p[i].priority,
               p[i].ct, p[i].tat, p[i].wt);
    }

    printf("\nAverage Turnaround Time: %.2f\n", total_tat / n);
    printf("Average Waiting Time   : %.2f\n", total_wt / n);
}

int main() {
    int n, priority_mode;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process p[n];
    printf("Enter Process ID, Arrival Time, Burst Time, and Priority:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d %d %d %d", &p[i].pid, &p[i].at, &p[i].bt, &p[i].priority);
    }

    printf("\nChoose Priority Mode:\n");
    printf("1. Lower number = Higher priority\n");
    printf("2. Higher number = Higher priority\n");
    printf("Enter choice (1 or 2): ");
    scanf("%d", &priority_mode);

    priority_non_preemptive(p, n, priority_mode == 2);

    return 0;
}

