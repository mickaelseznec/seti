#ifndef __THREADS_DISPATCH_H__
#define __THREADS_DISPATCH_H__

#include "threads_ports.h"

#include <pthread.h>
#include <sched.h>
#include <time.h>
#include <signal.h>

#ifdef USE_MACH
#include <mach/clock.h>
#include <mach/mach.h>
#include <sys/time.h>
#endif


int create_fp_thread (unsigned int priority,
		      unsigned int stack_size,
		      void* (*start_routine)(void),
		      pthread_t * tid,
		      int policy);

int init_periodic_config (thread_config_t *info);

int init_sporadic_config (thread_config_t *info);

void wait_next_period (thread_config_t *info);

int mask_realtime_signals();

StatusType await_periodic_dispatch(thread_config_t * global_q);

StatusType await_sporadic_dispatch(thread_queue_t * global_q);

void set_start_time();

void display_relative_date(char * id, unsigned long iteration);

#endif
