#include "threads_dispatch.h"

#ifdef DEBUG
#include <stdio.h>
#endif

struct timespec start_timespec;

int create_fp_thread (unsigned int priority,
		      unsigned int stack_size,
		      void* (*start_routine)(void),
		      pthread_t * tid,
		      int policy)
{
  int                sys_ret;
  pthread_attr_t     attr;
  struct sched_param param;

  sys_ret = pthread_attr_init (&attr);
  if (sys_ret != 0)
    {
#ifdef DEBUG
      printf("Error: thread attr init %d\n", sys_ret);
#endif
      return sys_ret;
    }

  pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

  sys_ret = pthread_attr_setscope (&attr, PTHREAD_SCOPE_SYSTEM);
  if (sys_ret != 0)
    {
#ifdef DEBUG
      printf("Error: thread attr set scope %d\n", sys_ret);
#endif
      return sys_ret;
    }

  if (stack_size != 0)
    {
      // On some systems, pthread_attr_setstacksize() can fail with the error
      // EINVAL if stacksize is not a multiple of the system page size.
      sys_ret = pthread_attr_setstacksize (&attr, stack_size);
      if (sys_ret != 0)
	{
#ifdef DEBUG
          printf("Error: thread attr set stack size %d\n", sys_ret);
#endif
	  return sys_ret;
	}
    }

  sys_ret = pthread_attr_setschedpolicy(&attr, policy);
  if (sys_ret != 0)
    {
      return sys_ret;
    }

  /*
   * Q1: to be completed, set the priority of tid
   */

  sys_ret = pthread_create (tid, &attr, (void* (*)(void*))start_routine, NULL);

  return sys_ret;
}


int init_sporadic_config (thread_config_t * config)
{
  sporadic_thread_config_t * info = &config->sporadic_config;
  int ret;
  /*Q3: to be completed with the dynamic initialization of fields in
    info (e.g. coditional variables, mutexes, etc.) */
  return ret;
}

int init_periodic_config (thread_config_t * config)
{
  periodic_thread_config_t * info = &(config->periodic_config);
  int ret;
  /* nothing to do in the "simple" implementation, see the correction
     for the "sophisticated" solution */
  return ret;
}

StatusType await_periodic_dispatch(thread_config_t * config)
{
  periodic_thread_config_t * info = &(config->periodic_config);

  /* Q1: to be completed, block the thread until the next occurence of
   * the period
   */

  return E_OK;
}

StatusType await_sporadic_dispatch(thread_queue_t * global_q)
{
  thread_config_t * config = global_q->task;
  sporadic_thread_config_t * info = &config->sporadic_config;

  StatusType status = E_OK;

  // wait next period
  if(info->timing_wait_phase)
  {
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, info->sporadic_timespec, NULL);
  }
  info->timing_wait_phase = 1;

  // wait for messages
  /* Q3: to be completed, make the thread waits until it receives a
   * message. When a message is in the queue (info->global_q) its field
   * msg_nb is different than 0. When a message is sent, a signal is
   * sent through conditional variable info->global_q->event. We
   * recommand to read code contained in file threads_ports.c to
   * better understand how messages are exchanged among threads. In
   * particular, read those used in T3_body and T1_body, implemented
   * in file main.c
   */
  status = pthread_mutex_lock(info->global_q->rez);
  if (status != E_OK)
    return status;

  if(info->global_q->msg_nb==0)
  {
    info->global_q->waiting=info->global_q->waiting+1;
    pthread_cond_wait(info->global_q->event, info->global_q->rez);
    info->global_q->waiting=info->global_q->waiting-1;
  }

  /* Q3: to be completed, compute the time (absolute date) until which
   * we need to put the sporadic thread asleep. Store in
   * info->sporadic_timespec
   */
  
  return status;
}

void set_start_time()
{
  clock_gettime(CLOCK_MONOTONIC, &start_timespec);
}

void display_relative_date(char * id, unsigned long iteration)
{
  struct timespec current_timespec;
#ifdef DEBUG
  clock_gettime(CLOCK_MONOTONIC, &current_timespec);
  unsigned long sec = current_timespec.tv_sec-start_timespec.tv_sec;
  unsigned long nsec = current_timespec.tv_nsec-start_timespec.tv_nsec;
  printf("%s - date: %ld sec: %ld nsec - iteration %ld\n", id, sec, nsec, iteration);
#endif
}
