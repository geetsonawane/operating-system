#include <stdio.h>

struct Process {
    int pid, at, bt, ct, tat, wt, rt, remaining;
};

void round_robin(struct Process p[], int n, int tq) {
    int time = 0, completed = 0;
    int queue[100], front = 0, rear = 0;
    int visited[n];
    int gantt[100], gantt_time[100], gantt_index = 0;

    for (int i = 0; i < n; i++) {
        visited[i] = 0;
        p[i].remaining = p[i].bt;
    }

    // Initially add processes that arrived at time 0
    for (int i = 0; i < n; i++) {
        if (p[i].at == 0) {
            queue[rear++] = i;
            visited[i] = 1;
        }
    }

    while (completed < n) {
        if (front == rear) {
            time++;
            for (int i = 0; i < n; i++) {
                if (!visited[i] && p[i].at <= time) {
                    queue[rear++] = i;
                    visited[i] = 1;
                }
            }
            continue;
        }

        int idx = queue[front++];
        int exec_time = (p[idx].remaining > tq) ? tq : p[idx].remaining;

        gantt[gantt_index] = p[idx].pid;
        gantt_time[gantt_index++] = time;

        time += exec_time;
        p[idx].remaining -= exec_time;

        // Add newly arrived processes to queue
        for (int i = 0; i < n; i++) {
            if (!visited[i] && p[i].at <= time) {
                queue[rear++] = i;
                visited[i] = 1;
            }
        }

        if (p[idx].remaining > 0) {
            queue[rear++] = idx;  // Re-queue the same process
        } else {
            p[idx].ct = time;
            completed++;
        }
    }

    gantt_time[gantt_index] = time; // End time for last process

    // Calculate TAT and WT
    for (int i = 0; i < n; i++) {
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
    }

    // Gantt Chart
    printf("\nGantt Chart:\n-");
    for (int i = 0; i < gantt_index; i++) {
        printf("------");
    }
    printf("\n|");
    for (int i = 0; i < gantt_index; i++) {
        printf("  P%d |", gantt[i]);
    }
    printf("\n-");
    for (int i = 0; i < gantt_index; i++) {
        printf("------");
    }
    printf("\n");

    for (int i = 0; i <= gantt_index; i++) {
        printf("%-6d", gantt_time[i]);
    }
    printf("\n");

    // Final Table
    float total_tat = 0, total_wt = 0;
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
        total_tat += p[i].tat;
        total_wt += p[i].wt;
    }

    printf("\nAverage Turnaround Time: %.2f\n", total_tat / n);
    printf("Average Waiting Time: %.2f\n", total_wt / n);
}

int main() {
    int n, tq;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process p[n];
    printf("Enter PID, Arrival Time, and Burst Time:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d %d %d", &p[i].pid, &p[i].at, &p[i].bt);
    }

    printf("Enter Time Quantum: ");
    scanf("%d", &tq);

    round_robin(p, n, tq);
    return 0;
}
