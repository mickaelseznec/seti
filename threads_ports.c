#include "threads_ports.h"

#ifdef DEBUG
#include <stdio.h>
#endif


StatusType SendOutput_runtime(thread_queue_t * global_q, int port_id, void * value)
{

  StatusType status = E_OK;

  struct port_queue_t *port_q = (struct port_queue_t*) global_q->port_queues[port_id];

  status = pthread_mutex_lock(global_q->rez);


  if (status != E_OK)
  {
#ifdef DEBUG
    printf("Send output: ERROR %d when acquiring lock\n", status);
#endif
    return status;
  }

  if ((port_q->write_idx+1) % port_q->queue_size == port_q->first_idx) {
    status = E_OS_LIMIT;
#if defined DEBUG
    printf("Send output: ERROR, full queue\n");
#endif
  } else {
    global_q->msg_nb++;
    port_q->write_idx = (port_q->write_idx+1) % port_q->queue_size;
    
    memcpy(&port_q->offset[port_q->write_idx*(port_q->msg_size/sizeof(char))],
	   value,
	   port_q->msg_size);

#if defined DEBUG
    printf("Write in index %d, %d \n", port_q->write_idx, *((char*) value));
#endif

    if (global_q->waiting)
      status = pthread_cond_signal(global_q->event);
  }

  status = pthread_mutex_unlock(global_q->rez);

  return status;
}

StatusType NextValue_runtime(thread_queue_t * global_q, int port_id, void * dst)
{

  StatusType status = E_OK;

  struct port_queue_t *port_q = (struct port_queue_t*) global_q->port_queues[port_id];

  status = pthread_mutex_lock(global_q->rez);

  if (status != E_OK)
  {
#if defined DEBUG
    printf("Next value: ERROR %d when acquiring lock\n", status);
#endif
    return status;
  }

  if(port_q->first_idx == port_q->last_idx)
  {

#ifdef DEBUG
    printf("Next value: ERROR, empty queue\n");
#endif
    pthread_mutex_unlock(global_q->rez);
    return E_OS_LIMIT;
  }

  port_q->first_idx=(port_q->first_idx+1)%port_q->queue_size;


  memcpy(dst, &port_q->offset[(port_q->first_idx)*(port_q->msg_size/sizeof(char))], port_q->msg_size);

#if defined DEBUG
  printf("Next value: first index is %d, %d\n", port_q->first_idx, *((char*)dst) );
#endif

  global_q->msg_nb--;

  pthread_mutex_unlock(global_q->rez);

  return status;
}

StatusType GetValue_runtime(thread_queue_t * global_q, int port_id, void * dst)
{

  StatusType status = E_OK;

  struct port_queue_t *port_q = (struct port_queue_t*) global_q->port_queues[port_id];

  status = pthread_mutex_lock(global_q->rez);

  if (status != E_OK)
  {
#ifdef DEBUG
    printf("Get value: ERROR %d when acquiring lock\n", status);
#endif
    return status;
  }

  memcpy(dst, &port_q->offset[(port_q->first_idx)*(port_q->msg_size/sizeof(char))], port_q->msg_size);

  pthread_mutex_unlock(global_q->rez);

  return status;
}


StatusType ReceiveInputs_runtime(thread_queue_t * global_q, int port_id)
{

  StatusType status = E_OK;

  struct port_queue_t * port_q = (struct port_queue_t*) global_q->port_queues[port_id];

  status = pthread_mutex_lock(global_q->rez);

  if (status != E_OK)
  {
#ifdef DEBUG
    printf("Receive inputs: ERROR %d when acquiring lock\n", status);
#endif
    return status;
  }

  int discarded_msg_nb = 0;

  if(port_q->last_idx < port_q->first_idx)
    discarded_msg_nb = port_q->queue_size-port_q->first_idx+port_q->last_idx;
  else
    discarded_msg_nb = port_q->last_idx-port_q->first_idx;

  if(port_q->write_idx != port_q->last_idx)
  { // new messages received
    port_q->first_idx = (port_q->last_idx+1) % port_q->queue_size;  
  }
  else
    port_q->first_idx = port_q->last_idx;
  
  port_q->last_idx = port_q->write_idx;

#if defined (DEBUG)
    printf("Receive inputs: number of discarded messages is %d\n", discarded_msg_nb);
    printf("Receive inputs: first: %d, last: %d\n", port_q->first_idx, port_q->last_idx);
#endif

  global_q->msg_nb-=discarded_msg_nb;

  status = pthread_mutex_unlock(global_q->rez);

  return status;
}

StatusType GetValueDataPort_runtime(data_port_t * p, void* data)
{

  StatusType status = E_OK;

  status = pthread_mutex_lock(p->rez);

  if (status != E_OK)
    return status;

  memcpy (data, p->data, p->size);

  status = pthread_mutex_unlock(p->rez);

  return status;
}

StatusType PutValueDataPort_runtime(data_port_t * p, void* data)
{

  StatusType status = E_OK;

  status = pthread_mutex_lock(p->rez);

  if (status != E_OK)
    return status;

  memcpy (p->data, data, p->size);

  status = pthread_mutex_unlock(p->rez);

  return status;
}

StatusType GetCount_runtime(thread_queue_t * global_q, int port_id, int * count_res)
{

  StatusType status = E_OK;

  struct port_queue_t * port_q = (struct port_queue_t*) global_q->port_queues[port_id];

  status = pthread_mutex_lock(global_q->rez);

  if (status != E_OK)
    return status;

  if(port_q->last_idx<port_q->first_idx)
    *count_res = port_q->queue_size-port_q->first_idx+port_q->last_idx;
  else
    *count_res = port_q->last_idx-port_q->first_idx;

  status = pthread_mutex_unlock(global_q->rez);

  return status;
}
