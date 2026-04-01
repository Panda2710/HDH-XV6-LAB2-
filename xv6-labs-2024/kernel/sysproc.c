#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "ptree.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_ptree(void)
{
  // Get 'max' argument
  int max;
  argint(1, &max);

  if (max <= 0)
    return -1;

  // Get address of buffer argument
  uint64 bufAddr;
  argaddr(0, &bufAddr);

  // Kernel-side buffer to store tree info
  struct ptreeinfo kernelBuf[64];
  
  // Getting the list of running processes from the process table
  struct proc *rpTable = kalloc();
  parse_running_processes(rpTable);
  // if (parseAmount > max)
  //   return -1;
  
  // Transfer attributes from list to kernel-side buffer
  for (int i = 0; i < max; i++) {
    kernelBuf[i].pid = rpTable[i].pid;
    memmove(kernelBuf[i].name, rpTable[i].name, 16);
    kernelBuf[i].state = rpTable[i].state;
    kernelBuf[i].memsize = rpTable[i].sz;
    if (rpTable[i].parent != 0)
      kernelBuf[i].ppid = rpTable[i].parent->pid;
  }

  // Copy kernel-side buffer to user space buffer argument
  struct proc *curProc = myproc();
  if (copyout(curProc->pagetable, bufAddr, (char*)kernelBuf, sizeof(kernelBuf)) != 0)
    return -1;

  kfree(rpTable);
  return 0;
}