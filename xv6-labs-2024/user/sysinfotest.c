#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/sysinfo.h"
#include "user/user.h"

void print_info(struct sysinfo *info, char *stage) {
    if (sysinfo(info) < 0) {
        printf("sysinfo call failed at %s!\n", stage);
        exit(1);
    }
    printf("--- %s ---\n", stage);
    printf("Free memory: %d bytes\n", (int)info->freemem);
    printf("Active processes: %d\n\n", (int)info->nproc);
}

int main(int argc, char *argv[]) {
    struct sysinfo info;

    print_info(&info, "1. Normal State (Baseline)");

    int pid = fork();
    
    if (pid < 0) {
        printf("Fork failed!\n");
        exit(1);
    } 
    else if (pid == 0) {
        // De tien trinh con sleep 10 tick de tien trinh cha kip thoi do duoc
        sleep(10);
        exit(0);
    } 
    else {
        // De tien trinh cha sleep 2 tick, doi mot xiu de OS setup xong tien trinh con de do luong
        sleep(2);
        print_info(&info, "2. After Fork (Child is alive)");

        wait(0);
        
        print_info(&info, "3. After Child Exit & Wait");

        printf("sysinfotest: OK\n");
        exit(0);
    }
}