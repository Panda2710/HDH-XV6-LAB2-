#include "kernel/types.h"
#include "user/user.h"

void printIndent(const int level) {
    if (level < 1)
        return;
    for (int i = 1; i <= level; i++)
        printf("   ");
}

void printPTree(struct ptreeinfo *buf, struct ptreeinfo prcs, const int max, const int level, const int maxLevel) {
    // Stop when reaching max depth or if entry is empty
    if (level > maxLevel || prcs.pid == 0)
        return;
    
    // Print entry
    printf("pid=%d name=%s state=%d mem=%u\n", prcs.pid, prcs.name, prcs.state, (int)prcs.memsize);
    
    // Find children and recursion
    for (int i = 0; i < max; i++) {
        if (buf[i].ppid == prcs.pid) {
            printIndent(level + 1);
            printPTree(buf, buf[i], max, level + 1, maxLevel);
            buf[i].pid = 0;     // Mark child process as empty after recursive iteration to avoid repeating
        }
    }
}

int main() {
    int max = 64;
    struct ptreeinfo *buffer = malloc(sizeof(struct ptreeinfo) * max);
    int res = ptree(buffer, max);
    if (res != 0) {
        printf("An error occurred while parsing processes.\n");
        exit(res);
    }
    for (int i = 0; i < max; i++) {
        printPTree(buffer, buffer[i], max, 0, 64);
    }

    free(buffer);
    exit(0);
}