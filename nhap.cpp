#include <stdio.h>

#define MAX_PRIORITY 9999

struct process {
    int process_no;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completion_time;
    int waiting_time;
    int turnaround_time;
    int priority;
    int original_priority;
};

void inputProcess(struct process *p, int process_no) {
    printf("\nProcess No: %d\n", process_no);
    p->process_no = process_no;

    printf("Enter Arrival Time: ");
    scanf("%d", &(p->arrival_time));

    printf("Enter Burst Time: ");
    scanf("%d", &(p->burst_time));

    printf("Enter Priority: ");
    scanf("%d", &(p->priority));

    p->original_priority = p->priority;
    p->remaining_time = p->burst_time;
}

void smallestPriorityFirstScheduling(struct process p[], int n) {
    int remaining_processes = n;
    int current_time = 0;

    while (remaining_processes > 0) {
        int min_priority = MAX_PRIORITY;
        int selected_process = -1;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && p[i].priority < min_priority && p[i].remaining_time > 0) {
                min_priority = p[i].priority;
                selected_process = i;
            }
        }

        if (selected_process == -1) {
            current_time++;
        } else {
            p[selected_process].remaining_time--;

            if (p[selected_process].remaining_time == 0) {
                remaining_processes--;
                current_time++;

                p[selected_process].completion_time = current_time;
                p[selected_process].turnaround_time = p[selected_process].completion_time - p[selected_process].arrival_time;
                p[selected_process].waiting_time = p[selected_process].turnaround_time - p[selected_process].burst_time;

                p[selected_process].priority = MAX_PRIORITY; // Mark as completed
            } else {
                current_time++;
            }
        }
    }
}

void displayProcessTable(struct process p[], int n) {
    printf("\nProcessNo\tAT\tBT\tPri\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t\t%d\t%d\t%d\t%d\t%d\t%d\n", p[i].process_no, p[i].arrival_time, p[i].burst_time,
               p[i].original_priority, p[i].completion_time, p[i].turnaround_time, p[i].waiting_time);
    }
}

int main() {
    int n;

    printf("<-- Smallest Priority First Scheduling Algorithm (Preemptive) -->\n");
    printf("Enter Number of Processes: ");
    scanf("%d", &n);

    struct process processes[10];

    for (int i = 0; i < n; i++) {
        inputProcess(&processes[i], i + 1);
    }

    smallestPriorityFirstScheduling(processes, n);

    displayProcessTable(processes, n);

    float avg_turnaround_time = 0, avg_waiting_time = 0;

    for (int i = 0; i < n; i++) {
        avg_turnaround_time += processes[i].turnaround_time;
        avg_waiting_time += processes[i].waiting_time;
    }

    avg_turnaround_time /= n;
    avg_waiting_time /= n;

    printf("\nAverage Turnaround Time = %f\nAverage Waiting Time = %f\n", avg_turnaround_time, avg_waiting_time);

    return 0;
}
