#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_RESOURCE_TYPES 3
#define NUM_PROCESSES 5

int i = 0;
int j = 0;
pthread_mutex_t mutex;

int available_resources[NUM_RESOURCE_TYPES];
int allocation_matrix[NUM_PROCESSES][NUM_RESOURCE_TYPES] = {{0, 1, 0}, {2, 0, 0}, {3, 0, 2}, {2, 1, 1}, {0, 0, 2}};
int max_matrix[NUM_PROCESSES][NUM_RESOURCE_TYPES] = {{7, 5, 3}, {3, 2, 2}, {9, 0, 2}, {2, 2, 2}, {4, 3, 3}};
int need_matrix[NUM_PROCESSES][NUM_RESOURCE_TYPES];

void init_resources(int available[]);
void init_need_matrix();
void print_need_matrix();
void print_allocation_matrix();
void print_available();
void print_vector(int vec[]);
int is_process_completed(int process_id);
int is_system_safe();
void *customer(void *customer_id);
int request_resources(int process_id, int request_vector[]);
int release_resources(int process_id, int release_vector[]);
int can_request_be_satisfied(int process_id, int request_vector[]);
int can_release_be_done(int process_id, int release_vector[]);
int can_allocate(int request_vector[]);

int main() {
    int available_resources_values[] = {3, 3, 2};
    init_resources(available_resources_values);
    init_need_matrix();

    printf("Initial Available Vector:\n");
    print_available();
    printf("Initial Allocation Matrix:\n");
    print_allocation_matrix();
    printf("Initial Need Matrix:\n");
    print_need_matrix();

    pthread_mutex_init(&mutex, NULL);
    pthread_attr_t attr_default;
    pthread_attr_init(&attr_default);
    pthread_t *thread_ids = (pthread_t *)malloc(sizeof(pthread_t) * NUM_PROCESSES);
    int *process_ids = (int *)malloc(sizeof(int) * NUM_PROCESSES);

    for (i = 0; i < NUM_PROCESSES; i++) {
        *(process_ids + i) = i;
        pthread_create((thread_ids + i), &attr_default, customer, (process_ids + i));
    }

    for (i = 0; i < NUM_PROCESSES; i++) {
        pthread_join(*(thread_ids + i), NULL);
    }

    return 0;
}

void init_resources(int available[]) {
    for (i = 0; i < NUM_RESOURCE_TYPES; i++) {
        available_resources[i] = available[i];
    }
}

void init_need_matrix() {
    for (i = 0; i < NUM_PROCESSES; ++i) {
        for (j = 0; j < NUM_RESOURCE_TYPES; ++j) {
            need_matrix[i][j] = max_matrix[i][j] - allocation_matrix[i][j];
        }
    }
}

void *customer(void *customer_id) {
    int process_id = *(int *)customer_id;

    do {
        // sleep(1);
        int request_vector[NUM_RESOURCE_TYPES];
        for (i = 0; i < NUM_RESOURCE_TYPES; i++) {
            request_vector[i] = (need_matrix[process_id][i] != 0) ? need_matrix[process_id][i] : 0;
        }
        pthread_mutex_lock(&mutex);
        printf("\nCustomer %d Requesting Resources:\n", process_id);
        print_vector(request_vector);
        int check = request_resources(process_id, request_vector);

        if (is_process_completed(process_id) == 1) {
            // sleep(1);
            int release_vector[NUM_RESOURCE_TYPES];
            for (i = 0; i < NUM_RESOURCE_TYPES; i++) {
                release_vector[i] = (allocation_matrix[process_id][i] != 0) ? allocation_matrix[process_id][i] : 0;
            }
            printf("\nCustomer %d Releasing Resources:\n", process_id);
            print_vector(release_vector);
            release_resources(process_id, release_vector);
            pthread_mutex_unlock(&mutex);
        } else {
            pthread_mutex_unlock(&mutex);
        }

        pthread_mutex_lock(&mutex);
        if (is_system_safe() == 0 || is_process_completed(process_id) == 1) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);
    } while (1);

    pthread_exit(NULL);
}

int is_system_safe() {
    int work[NUM_RESOURCE_TYPES];
    int finish[NUM_PROCESSES] = {0};

    for (int a = 0; a < NUM_RESOURCE_TYPES; a++)
        work[a] = available_resources[a];

    for (int b = 0; b < NUM_PROCESSES; b++) {
        if (finish[b] == 0) {
            int c;
            for (c = 0; c < NUM_RESOURCE_TYPES; c++)
                if (need_matrix[b][c] > work[c])
                    break;
            if (c == NUM_RESOURCE_TYPES) {
                for (int d = 0; d < NUM_RESOURCE_TYPES; d++)
                    work[d] += allocation_matrix[b][d];
                finish[b] = 1;
                b = -1;
            }
        }
    }

    for (int e = 0; e < NUM_PROCESSES; e++)
        if (finish[e] == 0)
            return 0;

    return 1;
}

int request_resources(int process_id, int request_vector[]) {
    if (!can_request_be_satisfied(process_id, request_vector)) {
        printf("Requested resources are greater than needed or not enough resources available.\n");
        return -1;
    }

    printf("Simulating Allocation..\n");

    if (!can_allocate(request_vector)) {
        printf("Not enough resources\n");
        return -1;
    }

    for (i = 0; i < NUM_RESOURCE_TYPES; ++i) {
        need_matrix[process_id][i] -= request_vector[i];
        allocation_matrix[process_id][i] += request_vector[i];
        available_resources[i] -= request_vector[i];
    }

    if (is_system_safe() == 1) {
        printf("Safe. Allocated successfully.\nAvailable Resources:\n");
        print_available();
        printf("Allocated matrix:\n");
        print_allocation_matrix();
        printf("Need matrix:\n");
        print_need_matrix();
    } else {
        printf("Not safe. Rolling back\n");
        for (i = 0; i < NUM_RESOURCE_TYPES; ++i) {
            need_matrix[process_id][i] += request_vector[i];
            allocation_matrix[process_id][i] -= request_vector[i];
            available_resources[i] += request_vector[i];
        }
        return -1;
    }
}

int release_resources(int process_id, int release_vector[]) {
    if (!can_release_be_done(process_id, release_vector)) {
        printf("Process does not own enough resources\n");
        return -1;
    }

    for (i = 0; i < NUM_RESOURCE_TYPES; i++) {
        allocation_matrix[process_id][i] -= release_vector[i];
        available_resources[i] += release_vector[i];
    }
    printf("Release successfully.\nAvailable Resources:\n");
    print_available();
    printf("Allocated matrix:\n");
    print_allocation_matrix();
    printf("Need matrix:\n");
    print_need_matrix();
    return 0;
}

int can_release_be_done(int process_id, int release_vector[]) {
    for (i = 0; i < NUM_RESOURCE_TYPES; ++i) {
        if (release_vector[i] > allocation_matrix[process_id][i]) {
            return 0;
        }
    }
    return 1;
}

int can_request_be_satisfied(int process_id, int request_vector[]) {
    for (i = 0; i < NUM_RESOURCE_TYPES; ++i) {
        if (request_vector[i] > need_matrix[process_id][i]) {
            return 0;
        }
    }
    return 1;
}

int can_allocate(int request_vector[]) {
    for (i = 0; i < NUM_RESOURCE_TYPES; ++i) {
        if (request_vector[i] > available_resources[i]) {
            return 0;
        }
    }
    return 1;
}

void print_need_matrix() {
    for (i = 0; i < NUM_PROCESSES; ++i) {
        printf("{ ");
        for (j = 0; j < NUM_RESOURCE_TYPES; ++j) {
            printf("%d, ", need_matrix[i][j]);
        }
        printf("}\n");
    }
    return;
}

void print_allocation_matrix() {
    for (i = 0; i < NUM_PROCESSES; ++i) {
        printf("{ ");
        for (j = 0; j < NUM_RESOURCE_TYPES; ++j) {
            printf("%d, ", allocation_matrix[i][j]);
        }
        printf("}\n");
    }
    return;
}

void print_available() {
    for (i = 0; i < NUM_RESOURCE_TYPES; ++i) {
        printf("%d, ", available_resources[i]);
    }
    printf("\n");
    return;
}

void print_vector(int vec[]) {
    for (i = 0; i < NUM_RESOURCE_TYPES; ++i) {
        printf("%d, ", vec[i]);
    }
    printf("\n");
    return;
}

int is_process_completed(int process_id) {
    for (j = 0; j < NUM_RESOURCE_TYPES; ++j) {
        if (need_matrix[process_id][j] != 0) {
            return 0;
        }
    }
    return 1;
}
