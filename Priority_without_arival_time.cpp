#include <stdio.h>

#define MAX_PRIORITY 9999

struct Process {
    int process_no;
    int burst_time;
    int remaining_time;
    int waiting_time;
    int turnaround_time;
    int priority;
    int original_priority;
};

void inputProcess(struct Process *p, int process_no) {
    printf("\nProcess No: %d\n", process_no);
    p->process_no = process_no;

    printf("Enter Burst Time: ");
    scanf("%d", &(p->burst_time));

    printf("Enter Priority: ");
    scanf("%d", &(p->priority));

    p->original_priority = p->priority;
    p->remaining_time = p->burst_time;
}

void Priority_Algorithm(struct Process p[], int n) {
    int remaining_processes = n;
    int total = 0;

    while (remaining_processes > 0) {
        int min_priority = MAX_PRIORITY;
        int selected_process = -1;

        for (int i = 0; i < n; i++) {
            if (p[i].priority < min_priority && p[i].remaining_time > 0) {
                min_priority = p[i].priority;
                selected_process = i;
            }
        }

        if (selected_process == -1) {
        {
            printf("-");
            break;
        }
        } else {
            printf("P%d", p[selected_process].process_no);
            while (p[selected_process].remaining_time > 0)
            {  
                p[selected_process].remaining_time--;
                printf("*");

                if (p[selected_process].remaining_time == 0) {
                    total += p[selected_process].burst_time;
                    remaining_processes--;

                    p[selected_process].turnaround_time = total;
                    p[selected_process].waiting_time = p[selected_process].turnaround_time - p[selected_process].burst_time;

                    p[selected_process].priority = MAX_PRIORITY; // Mark as completed
                }
            }
        }
    }
}

void displayProcessTable(struct Process p[], int n) {
    printf("\nProcessNo\tBT\tPri\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t\t%d\t%d\t%d\t%d\n", p[i].process_no, p[i].burst_time,
               p[i].original_priority, p[i].turnaround_time, p[i].waiting_time);
    }
}

int main() {
    int n;

    printf("<-- Smallest Priority First Scheduling Algorithm (Preemptive) -->\n");
    printf("Enter Number of Processes: ");
    scanf("%d", &n);

    struct Process processes[10];

    for (int i = 0; i < n; i++) {
        inputProcess(&processes[i], i + 1);
    }

    printf("\n--- Bieu do the hien viec nhan CPU cua cac tien trinh ---\n\n");
    Priority_Algorithm(processes, n);
    printf("\n");

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
