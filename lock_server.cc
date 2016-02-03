// the lock server implementation

#include "lock_server.h"
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

lock_server::lock_server():
  nacquire (0)
{
	pthread_mutex_init(&mutexmap, NULL);
  pthread_cond_init(&stateVar, NULL);
}
lock_server::~lock_server() {
	locks.clear();
	pthread_mutex_destroy(&mutexmap);
  pthread_cond_destroy(&stateVar);
};

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
  pthread_mutex_lock(&mutexmap);
  if(locks.find(lid) != locks.end() && locks[lid] == LOCKED) {
    while(locks[lid] == LOCKED) {
      pthread_cond_wait(&stateVar, &mutexmap);
    }
  }
  locks[lid] = LOCKED;
  printf("lock %d is granted to %d\n", lid, clt);
  pthread_mutex_unlock(&mutexmap);
  lock_protocol::status ret = lock_protocol::OK;
  return ret;
}

lock_protocol::status
lock_server::release(int clt, lock_protocol::lockid_t lid, int &r)
{
  lock_protocol::status ret; 
  pthread_mutex_lock(&mutexmap);
  std::map<lock_protocol::lockid_t, bool>::iterator it;
  if((it = locks.find(lid)) != locks.end() && it->second == LOCKED) {
  	locks[lid] = FREE;
    pthread_cond_broadcast(&stateVar);
  	printf("clt %d release lock %d\n", clt, lid);
    ret = lock_protocol::OK;
  } else {
  	printf("clt %d does not hold %d\n", clt, lid);
    ret = lock_protocol::NOENT;
  }
  pthread_mutex_unlock(&mutexmap);
  return ret;
}


