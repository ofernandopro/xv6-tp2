#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "proc.h"

char* getTypeProc(int i) {
    if (i == 0) {
        return "CPU-Bound";
    } else if (i == 1) {
        return "S-Bound";
    } else {
        return "IO-Bound";
    }
}

int main(int argc, char* argv[])  {

    if (argc > 2) {
        printf(1, "n deve ser um inteiro >= 0.\n");
        exit();
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        printf(1, "n deve ser um inteiro >= 0.\n");
        exit();
    }

    int qntProcs[3], readyTime[3], sleepingTime[3], turnaroundTime[3];

    int i;
    for (i = 0; i < 3; i++) {
        qntProcs[i] = 0;
        readyTime[i] = 0;
        sleepingTime[i] = 0;
        turnaroundTime[i] = 0;
    }

    int procTickets = {10, 30, 20};

    for (i = 0; i < 3 * n; i++) {
        int pid = fork();

        // struct proc *p;
        // p->pid = pid;
        // set_tickets(procTickets[i]);

        // printf(1, "Tickets = %d\n", p->tickets);

        if (pid == 0) {
            int pidAux = getpid() % 3;
            
            if (pidAux == 0) {  // CPU-Bound Processes
                int i;
                for (i = 0; i < 100; i++) {
                    int j;
                    for (j = 0; j < 10000000; j++) {
                        asm("nop");
                    }
                }
            } 
            else if (pidAux == 1) { // S-CPU Processes
                int i;
                for (i = 0; i < 100; i++) {
                    int j;
                    for (j = 0; j < 1000000; j++) {
                        asm("nop");
                        if (j % 10000 == 0)
                            user_yield();
                    }
                }
            } 
            else { // IO-Bound Processes
                int i;
                for (i = 0; i < 100; i++) {
                    sleep(1);
                }
            }
            exit();
        } else {
            qntProcs[pid % 3]++;
            continue;
        }
    }

    for (i = 0; i < 3 * n; i++) {
        int retime, rutime, stime;
        int pidChild = wait2(&retime, &rutime, &stime);
        int pidChildAux = pidChild % 3;
        char *type = getTypeProc(pidChildAux);

        printf(1, "Pid = %d, type = %s, RETIME = %d, RUTIME = %d, STIME = %d\n", pidChild, type, retime, rutime, stime);

        readyTime[pidChildAux] += retime;
        sleepingTime[pidChildAux] += stime;
        turnaroundTime[pidChildAux] += retime + rutime + stime;
    }

    int averageReady[3], averageSleeping[3], averageTurnaround[3];
    int j;

    for (j = 0; j < 3; j++) {
        printf(1, "Type %d\n", j);
        printf(1, "Number of processes: %d\n", qntProcs[j]);

        averageReady[j] = readyTime[j] / qntProcs[j];
        averageSleeping[j] = sleepingTime[j] / qntProcs[j];
        averageTurnaround[j] = turnaroundTime[j] / qntProcs[j];
        
        printf(1, "Average ready time: %d\n", averageReady[j]);
        printf(1, "Average sleeping time: %d\n", averageSleeping[j]);
        printf(1, "Average turnaround time: %d\n", averageTurnaround[j]); 

        printf(1, "\n"); 
    }

    exit();
}
