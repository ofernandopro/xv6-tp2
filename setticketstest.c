#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char* argv[]) 
{
  set_tickets(10);

  cprintf("myproc tickets -> %d", myproc()->tickets)

  exit();
}