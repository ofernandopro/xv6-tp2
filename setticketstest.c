#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char* argv[]) {
  int tickets = atoi(argv[1]);
  set_tickets(tickets);

  exit();
}