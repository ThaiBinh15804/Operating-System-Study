#include<stdio.h>

#define MAX_priority 100

struct process {
    int process_no;         //Mã tiến trình
    int arrival_time;       //Thời gian tiến trình đến nhận CPU
    int burst_time;         //Thời gian tiến trình cần để xử lí
    int remaining_time;     //Thời gian còn lại (giá trị ban đầu = burst time)
    int completion_time;    //Thời gian hoàn thành
    int waiting_time;       //Thời gian chờ
    int turnaround_time;    //Thời gian từ khi đến cho đến khi hoàn thành
    int priority;           //Độ ưu tiên (có thể thay đổi để đánh dấu)
    int original_priority;  //Độ ưu tiên gốc không đổi
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

void Priority_preemptive(process p[], int n)
{
    int remaining_processes = n;
    int current_time = 0;
    int last_process_no = -1;

    // Chạy đến khi hết số tiến trình -> Cần giảm số tiến trình còn lại (remaining_processes) khi có 1 tiến trình hoàn tất
    while(remaining_processes > 0)
    {
        int min_priority = MAX_priority;
        int selected_process = -1; //Chỉ số tiến trình được chọn (tiến trình có độ ưu tiên cao nhất)

        //Duyệt toàn bộ danh sách để chọn tiến trình có độ ưu tiên cao nhất tại thời điểm hiện tại
        for (int i = 0; i < n; i ++)
        {
            if (p[i].arrival_time <= current_time && p[i].priority < min_priority && p[i].remaining_time > 0)
            {
                min_priority = p[i].priority; //Lưu tiến trình có độ ưu tiên cao nhất thể hiện đúng ý nghĩa thuật toán
                selected_process = i;
            }
        }

        // Kiểm tra xem current_time hiện tại có tiến trình nào được chọn?
        if (selected_process == -1)
        {
            printf("-");
            current_time ++; //Thời gian sẽ tăng lên cho đến khi có tiến trình được chọn
        }
        else
        {
            if (last_process_no != p[selected_process].process_no)
            {
                printf("|P%d", p[selected_process].process_no);
                last_process_no = p[selected_process].process_no;
            }
            p[selected_process].remaining_time --; //Thời gian thực hiện của tiến trình được chọn giảm 1

            // Kiểm tra sau khi tiến trình được chọn giảm remaining_time có hết chưa
            if (p[selected_process].remaining_time == 0)
            {
                remaining_processes --; //Tiến trình được chọn hoàn thành -> giảm số tiến trình còn lại
                printf("*");
                current_time ++;
                // Lưu lại thông tin về: completion_time, waiting_time, turnaround_time và đánh dấu tiến trình được chọn này đã hoàn thành
                p[selected_process].completion_time = current_time;
                p[selected_process].turnaround_time = p[selected_process].completion_time - p[selected_process].arrival_time;
                p[selected_process].waiting_time = p[selected_process].turnaround_time - p[selected_process].burst_time;
                
                p[selected_process].priority = MAX_priority;
            }
            else
            {
                current_time ++;
                printf("*");
            }
        }
    }
    printf("|");
}

void Priority_non_preemptive(process p[], int n)
{
    int remaining_processes = n;
    int current_time = 0;
    int last_process_no = -1;

    // Chạy đến khi hết số tiến trình -> Cần giảm số tiến trình còn lại (remaining_processes) khi có 1 tiến trình hoàn tất
    while(remaining_processes > 0)
    {
        int min_priority = MAX_priority;
        int selected_process = -1; //Chỉ số tiến trình được chọn (tiến trình có độ ưu tiên cao nhất)

        //Duyệt toàn bộ danh sách để chọn tiến trình có độ ưu tiên cao nhất tại thời điểm hiện tại
        for (int i = 0; i < n; i ++)
        {
            if (p[i].arrival_time <= current_time && p[i].priority < min_priority && p[i].remaining_time > 0)
            {
                min_priority = p[i].priority; //Lưu tiến trình có độ ưu tiên cao nhất thể hiện đúng ý nghĩa thuật toán
                selected_process = i;
            }
        }
        // Kiểm tra xem current_time hiện tại có tiến trình nào được chọn?
        if (selected_process == -1)
        {
            printf("-");
            current_time ++; //Thời gian sẽ tăng lên cho đến khi có tiến trình được chọn
        }
        else
        {
            while (p[selected_process].remaining_time > 0)
            {
                if (last_process_no != p[selected_process].process_no)
                {
                    printf("|P%d", p[selected_process].process_no);
                    last_process_no = p[selected_process].process_no;
                }
                p[selected_process].remaining_time --; //Thời gian thực hiện của tiến trình được chọn giảm 1

                // Kiểm tra sau khi tiến trình được chọn giảm remaining_time có hết chưa
                if (p[selected_process].remaining_time == 0)
                {
                    remaining_processes --; //Tiến trình được chọn hoàn thành -> giảm số tiến trình còn lại
                    printf("*|");
                    current_time ++;
                    // Lưu lại thông tin về: completion_time, waiting_time, turnaround_time và đánh dấu tiến trình được chọn này đã hoàn thành
                    p[selected_process].completion_time = current_time;
                    p[selected_process].turnaround_time = p[selected_process].completion_time - p[selected_process].arrival_time;
                    p[selected_process].waiting_time = p[selected_process].turnaround_time - p[selected_process].burst_time;
                    
                    p[selected_process].priority = MAX_priority;
                }
                else
                {
                    current_time ++;
                    printf("*");
                }
            }
        }
    }
}

void displayProcessTable(process p[], int n) {
    printf("\nProcessNo\tAT\tBT\tPri\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t\t%d\t%d\t%d\t%d\t%d\t%d\n", p[i].process_no, p[i].arrival_time, p[i].burst_time,
               p[i].original_priority, p[i].completion_time, p[i].turnaround_time, p[i].waiting_time);
    }
}

void ganGtri(process &a, process &b)
{
    a.process_no = b.process_no;
    a.arrival_time = b.arrival_time;
    a.burst_time = b.burst_time;
    a.completion_time = b.completion_time;
    a.original_priority = b.original_priority;
    a.priority = b.priority;
    a.remaining_time = b.remaining_time;
    a.turnaround_time = b.turnaround_time;
    a.waiting_time = b.waiting_time;
}

void ganMang(process a[], process b[], int n)
{
    for (int i = 0; i < n; i++)
    {
        ganGtri(a[i], b[i]);
    }
}

void InKetQua(process p[], int n)
{
    displayProcessTable(p, n);

    float avg_turnaround_time = 0, avg_waiting_time = 0;

    for (int i = 0; i < n; i++) {
        avg_turnaround_time += p[i].turnaround_time;
        avg_waiting_time += p[i].waiting_time;
    }

    avg_turnaround_time /= n;
    avg_waiting_time /= n;

    printf("\nAverage Turnaround Time = %f\nAverage Waiting Time = %f\n", avg_turnaround_time, avg_waiting_time);

}

void MENU(process p[], int n)
{

    while (1!=0)
    {
        printf("\n<----- Lua chon che do uu tien ----->");
        printf("\n1. Preemptive");
        printf("\n2. Non-preemptive");
        printf("\n0. Exit");
        
        int lc;
        printf("\nNhap lua chon: "); 
        scanf("%d", &lc);

        switch (lc)
        {
        case 1:
        {
            process tmp[50];
            ganMang(tmp, p, n);
            printf("\n--- Bieu do the hien viec nhan CPU cua cac tien trinh ---\n\n");
            Priority_preemptive(tmp, n);
            printf("\n");
            InKetQua(tmp, n);
            break;
        }
        case 2:
        {
            process tmp[50];
            ganMang(tmp, p, n);
            printf("\n--- Bieu do the hien viec nhan CPU cua cac tien trinh ---\n\n");
            Priority_non_preemptive(tmp, n);
            printf("\n");
            InKetQua(tmp, n);
            break;
        }
        
        case 0: return;


        default: printf("\nVui long nhap lai!");
            break;
        }
    }
}


int main() {
    int n;

    printf("\n\n<-- Priority Scheduling Algorithm -->\n");
    printf("Enter Number of Processes: ");
    scanf("%d", &n);

    struct process processes[50];

    for (int i = 0; i < n; i++) {
        inputProcess(&processes[i], i + 1);
    }

    if (n > 0)
        MENU(processes, n);
    

    return 0;
}