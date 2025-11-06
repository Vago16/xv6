#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

//int
//sys_sbrk(void)
//{
//  int addr;
//  int n;

//  if(argint(0, &n) < 0)
//    return -1;
//  addr = myproc()->sz;
//  if(growproc(n) < 0)
//    return -1;
//  return addr;
//}

//sys_sbrk without call to growproc()
int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  
  addr = myproc()->sz; //initialize
  myproc()->sz += n; //ncrement the process memory size

  return addr;  
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

//add syscall hello() such that contents/functionality of this syscall outputs “Hello from Kernel Mode!”
int 
sys_hello(void)
{
  cprintf("Hello from Kernel Mode!\n");   //prints directly to console (doesnt need stdout.h)
  return 0;
}

//add syscall ticks_running() to show how many ticks a process is taking
int
sys_ticks_running(void)
{
  int pid;
  //if specified process is not surrently running
  if (argint(0, &pid)) {
    return -1;
  }
  //otherwise return ticks
  return ticks_running(pid);
}

//add syscall to predit length of jobs/processes
int sys_sjf_length(void) 
{
  int pid;
  struct proc *p;
  //if retrival from mem fails, return fail
  if (argint(0, &pid) < 0) {
    return -1;
  }
  //call helper function from proc.c
  return get_length(pid);

}
//add syscall for set_sched_priority
int
sys_set_sched_priority(void)
{
    int priority;
    if(argint(0, &priority) < 0)
        return -1;
    return set_sched_priority(priority);
}
//add syscall for get_sched_priority
int
sys_get_sched_priority(void)
{
    int pid;
    if(argint(0, &pid) < 0)
        return -1;
    return get_sched_priority(pid);
}