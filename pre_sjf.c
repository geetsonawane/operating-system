#include <stdio.h>
#include <limits.h>

struct Process {
    int pid, at, bt, ct, tat, wt;
    int remaining_bt;
};

int findShortest(struct Process p[], int n, int time) {
    int min_bt = INT_MAX, index = -1;
    for (int i = 0; i < n; i++) {
        if (p[i].at <= time && p[i].remaining_bt > 0 && p[i].remaining_bt < min_bt) {
            min_bt = p[i].remaining_bt;
            index = i;
        }
    }
    return index;
}

void sjf_preemptive(struct Process p[], int n) {
    int completed = 0, time = 0;
    float total_tat = 0, total_wt = 0;
    int gantt_time[100], gantt_pid[100], g_index = 0;
    int last = -1;

    for (int i = 0; i < n; i++)
        p[i].remaining_bt = p[i].bt;

    printf("\nGantt Chart:\n");
    for (int i = 0; i <= (n+1) * 6; i++) printf("-");
    printf("\n|");

    while (completed < n) {
        int idx = findShortest(p, n, time);

        if (idx == -1) {
            time++;
            continue;
        }

        if (last != p[idx].pid) {
            printf("  P%d |", p[idx].pid);
            gantt_pid[g_index] = p[idx].pid;
            gantt_time[g_index++] = time;
            last = p[idx].pid;
        }

        p[idx].remaining_bt--;
        time++;

        if (p[idx].remaining_bt == 0) {
            completed++;
            p[idx].ct = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            total_tat += p[idx].tat;
            total_wt += p[idx].wt;
        }
    }

    printf("\n");
    for (int i = 0; i <= (n+1) * 6; i++) printf("-");
    printf("\n");
    printf("%d", gantt_time[0]);
    for (int i = 1; i < g_index; i++) printf("    %2d", gantt_time[i]);
    printf("    %2d\n", time);

    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }
    printf("\nAverage Turnaround Time: %.2f\n", total_tat / n);
    printf("Average Waiting Time: %.2f\n", total_wt / n);
}

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process p[n];
    printf("Enter PID, Arrival Time and Burst Time:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d %d %d", &p[i].pid, &p[i].at, &p[i].bt);
    }

    sjf_preemptive(p, n);
    return 0;
}
