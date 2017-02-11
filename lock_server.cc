// the lock server implementation

#include "lock_server.h"
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
lock_server::lock_server():
  nacquire (0)
{
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond, NULL);
}

lock_protocol::status
lock_server::stat(int clt, lock_protocol::lockid_t lid, int &r)
{
  lock_protocol::status ret = lock_protocol::OK;
  printf("stat request from clt %d\n", clt);
  r = nacquire;
  return ret;
}

lock_protocol::status
lock_server::acquire(int clt, lock_protocol::lockid_t lid, int &r)
{
  lock_protocol::status ret = lock_protocol::OK;
  pthread_mutex_lock(&mutex);
  while(locks[lid]) {
    pthread_cond_wait(&cond, &mutex);
  }
  locks[lid] = 1;
  holders[lid] = clt;
  printf("Grant lock %lld to %d\n", lid, clt);
  pthread_mutex_unlock(&mutex);
  return ret;
}

lock_protocol::status
lock_server::release(int clt, lock_protocol::lockid_t lid, int &r)
{
  lock_protocol::status ret = lock_protocol::OK;
  pthread_mutex_lock(&mutex);
  if(locks[lid] && holders[lid] == clt) {
    locks[lid] = 0;
    holders[lid] = -1;
    pthread_cond_broadcast(&cond);
  }
  printf("Client %d release %lld\n", clt, lid);
  pthread_mutex_unlock(&mutex);
  return ret;
}
