//IMPLEMENTACAO SANITY
#include "types.h"
#include "user.h"

#define CPU_BOUND 0
#define SHORT_CPU 1
#define IO_BOUND 2

#define NUM_PRIORITIES 4
#define N_PRIO_TESTS 7 // define o valor de n para os testes de prioridade

int round_up(int dividend, int divisor) {
    return (dividend + divisor - 1) / divisor;
}

void cpu_bound(int prio_test, int priority) {
    if(prio_test) changeprio(priority); 
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 1000000; j++) {}
    }
}

void short_cpu() {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 1000000; j++) {
            if(j == 0) yield();
        }
    }
}

void io_bound() {
    for (int i = 0; i < 100; i++) {
        sleep(1);
    }
}

int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        
        printf(1, "help:\n");
        printf(1, "Como usar: sanity <n> \n", argv[0]);
        printf(1, "Realiza comparações para <n> maior que 0\n", argv[0]);
        printf(1, "Realiza testes de mudança de prioridade para <n> igual a 0\n", argv[0]);
        return 1;
        
    }

    int n = atoi(argv[1]);
    int ready_time, running_time, sleeping_time, n_execs;

    int cpu_bound_ready = 0;
    int cpu_bound_sleeping = 0;
    int cpu_bound_turnaround = 0;
    int cpu_bound_n_execs = 0;
    
    int s_cpu_ready = 0;
    int s_cpu_sleeping = 0;
    int s_cpu_turnaround = 0;
    int s_cpu_n_execs = 0;
    
    int io_bound_ready = 0;
    int io_bound_sleeping = 0;
    int io_bound_turnaround = 0;
    int io_bound_n_execs = 0;

    int prio_test = !n;

    if(prio_test) n = N_PRIO_TESTS;
    
    for (int i = 0; i < 3 * n; i++) {
        
        int pid = fork();
        
        if (pid < 0) {
            
            printf(1, "Falha ao criar processo\n");
            return 1;

        } else if (pid == 0) {
            
            int child_pid = getpid();
            
            if(prio_test) child_pid = 3;
            
            switch (child_pid % 3) {
                case CPU_BOUND:
                    cpu_bound(prio_test, i % NUM_PRIORITIES);
                    break;

                case SHORT_CPU:
                    short_cpu();
                    break;

                case IO_BOUND:
                    io_bound();
                    break;
            }
            exit();
        }
    }

    for (int i = 0; i < 3 * n; i++) {
        int pid = wait2(&ready_time, &running_time, &sleeping_time, &n_execs);
        
        int switch_condition = pid % 3;

        if(prio_test) switch_condition = CPU_BOUND;

        switch (switch_condition) {
            case CPU_BOUND:
                cpu_bound_ready += ready_time;
                cpu_bound_sleeping += sleeping_time;
                cpu_bound_turnaround += ready_time + running_time + sleeping_time;
                cpu_bound_n_execs += n_execs;

                printf(1, "\nCPU-B:\t pid: %d\t ready: %d\t running: %d\t sleeping: %d\t n_execs: %d",
                pid, ready_time, running_time, sleeping_time, n_execs);
                break;
            
            case SHORT_CPU:
                s_cpu_ready += ready_time;
                s_cpu_sleeping += sleeping_time;
                s_cpu_turnaround += ready_time + running_time + sleeping_time;
                s_cpu_n_execs += n_execs;

                printf(1, "\nS-CPU:\t pid: %d\t ready: %d\t running: %d\t sleeping: %d\t n_execs: %d",
                pid, ready_time, running_time, sleeping_time, n_execs);
                break;
            
            case IO_BOUND:
                io_bound_ready += ready_time;
                io_bound_sleeping += sleeping_time;
                io_bound_turnaround += ready_time + running_time + sleeping_time;
                io_bound_n_execs += n_execs;

                printf(1, "\nIO-B:\t pid: %d\t ready: %d\t running: %d\t sleeping: %d\t n_execs: %d",
                pid, ready_time, running_time, sleeping_time, n_execs);
                break;
        }
    }

    cpu_bound_ready = round_up(cpu_bound_ready, n);
    cpu_bound_sleeping = round_up(cpu_bound_sleeping, n);
    cpu_bound_turnaround = round_up(cpu_bound_turnaround, n);
    cpu_bound_n_execs = round_up(cpu_bound_n_execs, n);
    
    s_cpu_ready = round_up(s_cpu_ready, n);
    s_cpu_sleeping = round_up(s_cpu_sleeping, n);
    s_cpu_turnaround = round_up(s_cpu_turnaround, n);
    s_cpu_n_execs = round_up(s_cpu_n_execs, n);
    
    io_bound_ready = round_up(io_bound_ready, n);
    io_bound_sleeping = round_up(io_bound_sleeping, n);
    io_bound_turnaround = round_up(io_bound_turnaround, n);
    io_bound_n_execs = round_up(io_bound_n_execs, n);

    printf(1, "\n------------------------------------------------------------------------------------");
    printf(1, "\n           \t\t AVERAGE TIMES");
    printf(1, "\n-----------\t CPU-B\t S-CPU\t IO-B");
    
    printf(1, "\nready time:\t %d\t %d\t %d",
    cpu_bound_ready, s_cpu_ready, io_bound_ready);
    
    printf(1, "\nsleep time:\t %d\t %d\t %d",
    cpu_bound_sleeping, s_cpu_sleeping, io_bound_sleeping);

    printf(1, "\nturnaround:\t %d\t %d\t %d",
    cpu_bound_turnaround, s_cpu_turnaround, io_bound_turnaround);
    
    printf(1, "\nn_execs:\t %d\t %d\t %d\n",
    cpu_bound_n_execs, s_cpu_n_execs, io_bound_n_execs);
    
    exit();

}