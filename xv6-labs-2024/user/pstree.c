#include "kernel/types.h"
#include "user/user.h"

void printIndent(const int level) {
    if (level < 1)
        return;
    for (int i = 1; i <= level; i++)
        printf("   ");
}

void printPTree(struct ptreeinfo *buf, struct ptreeinfo prcs, const int level, const int maxLevel) {
    if (level > maxLevel || prcs.pid == 0)
        return;
    printf("%d %s state=%d mem=%u\n", prcs.pid, prcs.name, prcs.state, (int)prcs.memsize);
    
    for (int i = 0; i < 10; i++) {
        if (buf[i].ppid == prcs.pid) {
            printIndent(level + 1);
            printPTree(buf, buf[i], level + 1, maxLevel);
            buf[i].pid = 0;
        }
    }
}

int main() {
    struct ptreeinfo buffer[10];
    int res = ptree(buffer, 10);
    for (int i = 0; i < 10; i++) {
        printPTree(buffer, buffer[i], 0, 96);
    }

    return res;
}