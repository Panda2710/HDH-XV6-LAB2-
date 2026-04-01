#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/sysinfo.h"
#include "user/user.h"    

int main(int argc, char *argv[]) {
    struct sysinfo info;

    if (sysinfo(&info) < 0) {
        printf("sysinfo call failed!\n");
        exit(1);
    }


    printf("Free memory: %d bytes\n", (int)info.freemem);
    printf("Active processes: %d\n", (int)info.nproc);
    
    printf("sysinfotest: OK\n"); 

    exit(0);
}