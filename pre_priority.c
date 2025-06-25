#include <stdio.h>
#include <limits.h>

struct Process {
    int pid, at, bt, priority, ct, tat, wt, remaining, completed;
};

// Sort by PID for table
void sort_by_pid(struct Process p[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (p[j].pid > p[j + 1].pid) {
                struct Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
}

void preemptive_priority(struct Process p[], int n, int priority_type) {
    int time = 0, completed = 0;
    float total_tat = 0, total_wt = 0;

    for (int i = 0; i < n; i++) {
        p[i].remaining = p[i].bt;
        p[i].completed = 0;
    }

    // For Gantt Chart tracking
    int timeline[1000], timeline_pid[1000];
    int tl_idx = 0;
    int last_pid = -1;

    while (completed < n) {
        int idx = -1;
        int best = (priority_type == 1) ? INT_MAX : INT_MIN;

        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].remaining > 0) {
                if ((priority_type == 1 && p[i].priority < best) || 
                    (priority_type == 2 && p[i].priority > best)) {
                    best = p[i].priority;
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            // CPU Idle
            if (last_pid != -1) {
                timeline_pid[tl_idx] = -1;
                timeline[tl_idx++] = time;
                last_pid = -1;
            }
            time++;
            continue;
        }

        // If new process is scheduled
        if (p[idx].pid != last_pid) {
            timeline_pid[tl_idx] = p[idx].pid;
            timeline[tl_idx++] = time;
            last_pid = p[idx].pid;
        }

        // Execute process
        p[idx].remaining--;
        time++;

        if (p[idx].remaining == 0) {
            p[idx].ct = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            p[idx].completed = 1;
            total_tat += p[idx].tat;
            total_wt += p[idx].wt;
            completed++;
        }
    }
    // Add final time
    timeline[tl_idx] = time;

    // Gantt Chart
    printf("\nGantt Chart:\n-");
    for (int i = 0; i < tl_idx; i++) printf("--------");
    printf("\n|");
    for (int i = 0; i < tl_idx; i++) {
        if (timeline_pid[i] == -1)
            printf("  Idle |");
        else
            printf("  P%-3d |", timeline_pid[i]);
    }
    printf("\n-");
    for (int i = 0; i < tl_idx; i++) printf("--------");
    printf("\n");

    // Print timeline
    printf("%d", timeline[0]);
    for (int i = 1; i <= tl_idx; i++)
        printf("      %2d", timeline[i]);
    printf("\n");

    // Print Table
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
    int n, priority_type;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process p[n];

    printf("Enter PID, Arrival Time, Burst Time, and Priority:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d %d %d %d", &p[i].pid, &p[i].at, &p[i].bt, &p[i].priority);
    }

    printf("\nPriority Type:\n");
    printf("1. Lower number = Higher Priority\n");
    printf("2. Higher number = Higher Priority\n");
    printf("Enter choice (1 or 2): ");
    scanf("%d", &priority_type);

    preemptive_priority(p, n, priority_type);
    return 0;
}
