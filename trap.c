#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "traps.h"
#include "spinlock.h"

// Interrupt descriptor table (shared by all CPUs).
struct gatedesc idt[256];
extern uint vectors[];  // in vectors.S: array of 256 entry pointers
struct spinlock tickslock;
uint ticks;


//COPIED from vim.c, for mappages() to function
// Return the address of the PTE in page table pgdir
// that corresponds to virtual address va.  If alloc!=0,
// create any required page table pages.
static pte_t *
walkpgdir(pde_t *pgdir, const void *va, int alloc)
{
  pde_t *pde;
  pte_t *pgtab;

  pde = &pgdir[PDX(va)];
  if(*pde & PTE_P){
    pgtab = (pte_t*)P2V(PTE_ADDR(*pde));
  } else {
    if(!alloc || (pgtab = (pte_t*)kalloc()) == 0)
      return 0;
    // Make sure all those PTE_P bits are zero.
    memset(pgtab, 0, PGSIZE);
    // The permissions here are overly generous, but they can
    // be further restricted by the permissions in the page table
    // entries, if necessary.
    *pde = V2P(pgtab) | PTE_P | PTE_W | PTE_U;
  }
  return &pgtab[PTX(va)];
}

//COPIED from vm.c
// Create PTEs for virtual addresses starting at va that refer to
// physical addresses starting at pa. va and size might not
// be page-aligned.
static int
mappages(pde_t *pgdir, void *va, uint size, uint pa, int perm)
{
  char *a, *last;
  pte_t *pte;

  a = (char*)PGROUNDDOWN((uint)va);
  last = (char*)PGROUNDDOWN(((uint)va) + size - 1);
  for(;;){
    if((pte = walkpgdir(pgdir, a, 1)) == 0)
      return -1;
    if(*pte & PTE_P)
      panic("remap");
    *pte = pa | perm | PTE_P;
    if(a == last)
      break;
    a += PGSIZE;
    pa += PGSIZE;
  }
  return 0;
}

void
tvinit(void)
{
  int i;

  for(i = 0; i < 256; i++)
    SETGATE(idt[i], 0, SEG_KCODE<<3, vectors[i], 0);
  SETGATE(idt[T_SYSCALL], 1, SEG_KCODE<<3, vectors[T_SYSCALL], DPL_USER);

  initlock(&tickslock, "time");
}

void
idtinit(void)
{
  lidt(idt, sizeof(idt));
}

//PAGEBREAK: 41
void
trap(struct trapframe *tf)
{
  if(tf->trapno == T_SYSCALL){
    if(myproc()->killed)
      exit();
    myproc()->tf = tf;
    syscall();
    if(myproc()->killed)
      exit();
    return;
  }

  switch(tf->trapno){
  case T_IRQ0 + IRQ_TIMER:
    if(cpuid() == 0){
      acquire(&tickslock);
      ticks++;
      wakeup(&ticks);
      release(&tickslock);
    }

    //ADDED:increment tick count while process is running
    struct proc *p = myproc();
    if (p && p->state == RUNNING) {
      p->ticks_ran++;
      #ifdef SCHEDULER_PRIORITYRR
      p->ticks_in_quant++;
      if (p->ticks_in_quant >= QUANTUM) {
        yield();
      }
      #endif
    }

    lapiceoi();
    break;
  case T_IRQ0 + IRQ_IDE:
    ideintr();
    lapiceoi();
    break;
  case T_IRQ0 + IRQ_IDE+1:
    // Bochs generates spurious IDE1 interrupts.
    break;
  case T_IRQ0 + IRQ_KBD:
    kbdintr();
    lapiceoi();
    break;
  case T_IRQ0 + IRQ_COM1:
    uartintr();
    lapiceoi();
    break;
  case T_IRQ0 + 7:
  case T_IRQ0 + IRQ_SPURIOUS:
    cprintf("cpu%d: spurious interrupt at %x:%x\n",
            cpuid(), tf->cs, tf->eip);
    lapiceoi();
    break;

  //PAGEBREAK: 13
  default:
    if(myproc() == 0 || (tf->cs&3) == 0){
      // In kernel, it must be our mistake.
      cprintf("unexpected trap %d from cpu %d eip %x (cr2=0x%x)\n",
              tf->trapno, cpuid(), tf->eip, rcr2());
      panic("trap");
    }
    //Part 2. Implementing Lazy Page Allocation (Part 3. adding a choice between pure lazy and locality aware)
    {
      uint fault_address = rcr2();    //virtual address that caused the fault
      struct proc *p = myproc();    //gets pointer for currently running program

      //if there is a pagefault while there is a current process in user space
      if ((tf->trapno == T_PGFLT) && p && (tf->cs & 3) == DPL_USER) {
        uint page_virt_a = PGROUNDDOWN(fault_address);    //aligns to page boundary for mapping
        //no allocation of inavalid or kernel addresses
        if (page_virt_a < p->sz && page_virt_a < KERNBASE) {
  
          #ifdef LOCALITY //locality-aware allocation at compile time, 3 pages at start time
          cprintf("[LOCALITY] Page fault at 0x%x, mapping 3 pages\n", page_virt_a);   //check if in locality
            int mapped_any = 0;
            for (int i = 0; i < 3; i++) {
              uint va = page_virt_a + i * PGSIZE;
              cprintf("[LOCALITY] Mapping page at 0x%x\n", va);   //shows 3 pages being
              //do not map beyond the logical process size
              if (va >= p->sz)
                break; 

              //if already mapped, skip
              pte_t *pte = walkpgdir(p->pgdir, (void*)va, 0);
              if (pte && (*pte & PTE_P))
                continue;

              
              char *mem = kalloc();   //returns pointer to allocated phyical page
              //if out of physical memory
              if (mem == 0) {
                if (mapped_any) {
                  return;
                } else {
                  break;
                }
              }
                //physical memory remaining
                memset(mem, 0, PGSIZE);   //zero allocated page so it is fresh
                //align page fault virtual address to page bounds, calls mappages 
                if (mappages(p->pgdir, (void*)va, PGSIZE, V2P(mem), PTE_W|PTE_U) < 0) {
                  //if mapping failed, free pages
                  kfree(mem);
                  if (mapped_any) {
                    return;
                  } else {
                    break;
                  }
                } 
                mapped_any = 1;
            } 
            if (mapped_any) {
              return; //success of mapping a page, so process continues
            }
          #else   //pure-lazy allocation at compilation time
            cprintf("[LAZY] Page fault at 0x%x, mapping single page\n", page_virt_a);   //check if lazy
            char *mem = kalloc();
            if (mem != 0) {
              memset(mem, 0, PGSIZE);
              if (mappages(p->pgdir, (void*)page_virt_a, PGSIZE, V2P(mem), PTE_W|PTE_U) == 0) {
                return; //mapping pages successful
              }
              kfree(mem);
            }
          #endif
        }
      }
    }
    // In user space, assume process misbehaved.
    cprintf("pid %d %s: trap %d err %d on cpu %d "
            "eip 0x%x addr 0x%x--kill proc\n",
            myproc()->pid, myproc()->name, tf->trapno,
            tf->err, cpuid(), tf->eip, rcr2());
    myproc()->killed = 1;
  }

  // Force process exit if it has been killed and is in user space.
  // (If it is still executing in the kernel, let it keep running
  // until it gets to the regular system call return.)
  if(myproc() && myproc()->killed && (tf->cs&3) == DPL_USER)
    exit();

  // Force process to give up CPU on clock tick.
  // If interrupts were on while locks held, would need to check nlock.
  if(myproc() && myproc()->state == RUNNING &&
     tf->trapno == T_IRQ0+IRQ_TIMER)
    yield();

  // Check if the process has been killed since we yielded
  if(myproc() && myproc()->killed && (tf->cs&3) == DPL_USER)
    exit();
}
