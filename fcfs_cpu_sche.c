#include <stdio.h>

struct Process {
    int pid, at, bt, ct, tat, wt;
};

void fcfs(struct Process p[], int n) {
    // Sort by arrival time
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].at > p[j + 1].at) {
                struct Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }

    int time = 0;
    float total_tat = 0, total_wt = 0;

    for (int i = 0; i < n; i++) {
        if (time < p[i].at) time = p[i].at;
        p[i].ct = time + p[i].bt;
        time = p[i].ct;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        total_tat += p[i].tat;
        total_wt += p[i].wt;
    }

    // Gantt Chart
    printf("\nGantt Chart:\n-");
    for (int i = 0; i < n; i++) printf("-------");
    printf("\n|");
    for (int i = 0; i < n; i++) printf("  P%d  |", p[i].pid);
    printf("\n-");
    for (int i = 0; i < n; i++) printf("-------");
    printf("\n0");
    for (int i = 0; i < n; i++) printf("%7d", p[i].ct);

    // Output table
    printf("\n\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
            p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }

    printf("\nAverage Turnaround Time: %.2f\n", total_tat / n);
    printf("Average Waiting Time: %.2f\n", total_wt / n);
}

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process p[n];
    printf("Enter PID, Arrival Time and Burst Time for each process:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d%d%d", &p[i].pid, &p[i].at, &p[i].bt);
    }

    fcfs(p, n);
    return 0;
}
