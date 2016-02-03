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
}
lock_server::~lock_server() {
	locks.clear();
	pthread_mutex_destroy(&mutexmap);
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
  lock_protocol::status ret = lock_protocol::OK;
  pthread_mutex_lock(&mutexmap);
  if(locks.find(lid) != locks.end()) {
  	printf("lock %d is already granted\n", lid);
    r = 1;
  } else {
  	locks[lid] = clt;
  	printf("clt %d got lock %d\n", clt, lid);
    r = 0;
  }
  pthread_mutex_unlock(&mutexmap);
  return ret;
}

lock_protocol::status
lock_server::release(int clt, lock_protocol::lockid_t lid, int &r)
{
  lock_protocol::status ret = lock_protocol::OK;
  pthread_mutex_lock(&mutexmap);
  std::map<lock_protocol::lockid_t, int>::iterator it;
  if((it = locks.find(lid)) != locks.end() && it->second == clt) {
  	locks.erase(it);
  	printf("clt %d release lock %d\n", clt, lid);
    r = 0;
  } else {
  	printf("clt %d does not hold %d\n", clt, lid);
    r = 1;
  }
  pthread_mutex_unlock(&mutexmap);
  return ret;
}


