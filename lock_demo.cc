//
// Lock demo
//

#include "lock_protocol.h"
#include "lock_client.h"
#include "rpc.h"
#include <arpa/inet.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>

std::string dst;
lock_client *lc;

int
main(int argc, char *argv[])
{
  int r;

  if(argc != 2){
    fprintf(stderr, "Usage: %s [host:]port\n", argv[0]);
    exit(1);
  }

  dst = argv[1];
  lc = new lock_client(dst);
  r = lc->stat(1);
  printf ("stat returned %d\n", r);
  r = lc->acquire(1);
  printf ("acq returned %d\n", r);
  r = lc->stat(1);
  printf ("stat returned %d\n", r);
  r = lc->release(1);
  printf ("release returned %d\n", r);
  r = lc->stat(1);
  printf ("stat returned %d\n", r);

  r = lc->stat(1);
  printf ("stat returned %d\n", r);
  r = lc->acquire(1);
  printf ("1 acq returned %d\n", r);

  int r2;
  lock_client *lc2 = new lock_client(dst);
  r2 = lc->acquire(1);
  printf ("2 acq returned %d\n", r2);
  r2 = lc->acquire(2);
  printf ("2 acq returned %d\n", r2);
  r = lc->release(1);
  printf ("release returned %d\n", r);
  r2 = lc->acquire(1);
  printf ("2 acq returned %d\n", r2);
}
