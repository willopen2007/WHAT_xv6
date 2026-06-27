#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0; // not reached
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
  if (growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n)
  {
    if (killed(myproc()))
    {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

#ifdef LAB_PGTBL
int sys_pgaccess(void)
{
  uint64 first_addr;
  int num_to_check;
  uint64 buffer_addr;
  uint64 buffer = 0;
  pte_t *curr_pte;
  argaddr(0, &first_addr);
  argint(1, &num_to_check);
  argaddr(2, &buffer_addr);
  pagetable_t pagetable = myproc()->pagetable;
  for (int i = 0; i < num_to_check; i++)
  {
    curr_pte = walk(pagetable, first_addr + PGSIZE * i, 0);
    if (curr_pte == 0 || !(*curr_pte & PTE_V) || !(*curr_pte & PTE_A))
    {
      buffer = buffer | (0L << i);
      /*if (curr_pte != 0 && (*curr_pte & PTE_V))
      {
        *curr_pte = *curr_pte & (~PTE_A); // set the access bit to be off
      }*/
    }
    else
    {
      buffer = buffer | (1L << i);
      *curr_pte = *curr_pte & (~PTE_A); // set the access bit to be off
    }
  }
  copyout(pagetable, buffer_addr, (char *)&buffer, sizeof(buffer));
  pte_t *zero_pte = walk(pagetable, 0, 0);
  if (zero_pte)
  {
    printf("ZERO PTE IS NOT NULL, IT'S %p\n", *zero_pte);
  }
  else
  {
    printf("ZERO PTE IS NULL\n");
  }
  return 0;
}
#endif

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
