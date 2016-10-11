#ifndef __THREADS_PORTS_H
#define __THREADS_PORTS_H

#include <signal.h>
#include <string.h>
#include <pthread.h>

#define E_OK 0
#define E_OS_LIMIT -100;

typedef int StatusType;


/* Queue id type */
typedef int port_id_t;

typedef struct port_queue_t {
  int queue_size;
  int first_idx;
  int last_idx;
  int write_idx;
  int msg_size;
  char * offset;
} port_queue_t;

union thread_config;

typedef struct thread_queue_t {
  pthread_mutex_t * rez;
  pthread_cond_t * event;
  union thread_config * task;
  char waiting;
  int msg_nb;
  port_queue_t * port_queues[];
} thread_queue_t;

typedef struct data_port_t {
  pthread_mutex_t * rez;
  void * data;
  int size;
} data_port_t;

typedef struct periodic_thread_config
{
  const unsigned int period;
  unsigned long iteration_counter;
} periodic_thread_config_t;

typedef struct sporadic_thread_config
{
  const unsigned int time_interval;
  struct timespec * sporadic_timespec;

  thread_queue_t * global_q;
  char timing_wait_phase;

} sporadic_thread_config_t;

typedef union thread_config
{
  periodic_thread_config_t periodic_config;
  sporadic_thread_config_t sporadic_config;
} thread_config_t;

StatusType SendOutput_runtime(thread_queue_t * global_q, int port_id, void * value);

StatusType NextValue_runtime(thread_queue_t * global_q, int port_id, void * dst);

StatusType GetValue_runtime(thread_queue_t * global_q, int port_id, void * dst);

StatusType ReceiveInputs_runtime(thread_queue_t * global_q, int port_id);

StatusType GetValueDataPort_runtime(data_port_t * p, void * data);

StatusType PutValueDataPort_runtime(data_port_t * p, void * data);

StatusType GetCount_runtime(thread_queue_t * global_q, int port_id, int * count_res);

#endif
