#include "events.h"
#include "periodictasks.h"
#include "tasks.h"

// Main procedure of polling server

void *runDeferredServer (void *t){
  TaskInfo           s;
  EventInfo          e;
  TimeSpan           c = 0;
  TimeSpan           l = 0;
  Time               d = 0;
  int                i;
  int                overlap;

  s = periodicTaskTable[*(int *)t];

  // Schedule the initial replenishment event
  e.kind        = PRODUCE;
  e.name        = s.name;
  e.activation  = nextActivation (s.period);
  e.computation = s.computation;
  appendEvent (e);

  // Print the event pushed in the queue
  putHeader (s.name);
  putString ("@");
  putTime   (e.activation);
  putString (" ");
  putString (s.name);
  putString ("=+");
  putTime   (e.computation);
  newLine ();

  while (systemNotCompleted () && (0 <= lastEvent)) {
    e = getEvent (firstEvent);
    
    // Before handling next event check the server budget is not empty
    if (s.computation == 0) {

      // Look for the first PRODUCE event
      for (i=firstEvent; i <= lastEvent; i++){
	e = getEvent(i);
	if (e.kind == PRODUCE) {
	  if (systemCompletedAt (e.activation)) {
	    return NULL;
	  }

          // Remove PRODUCE event and then wait for its activation time.
          removeEvent(i);
          delayUntil(e.activation);

          // Update the server budget and schedule the next PRODUCE
          // event. To do so compute the next activation time and the
          // computation time related to this replenishment event.
          e.activation  = nextActivation (s.period);
          appendEvent (e);
          s.computation = e.computation;

          // Print the arrival of this event
	  putHeader (s.name);
	  putString (s.name);
	  putString ("=+");
	  putTime   (e.computation);
	  newLine ();

          // Print the event pushed in the queue
	  putHeader (s.name);
	  putString ("@");
	  putTime   (e.activation);
	  putString (" ");
	  putString (s.name);
	  putString ("=+");
	  putTime   (e.computation);
	  newLine ();
      
	  break;
	}
      }
    }

    e = getEvent (firstEvent);
    if (systemCompletedAt (e.activation)){
      return NULL;
    }

    if (e.kind == PRODUCE) {
      // Remove Produce event and then wait for its activation time
      removeEvent(0);
      delayUntil(e.activation);

      // Update the server budget. And schedule the next replenishment.
      e.activation  = nextActivation (s.period);
      appendEvent (e);
      s.computation = e.computation;

      // Print the event pushed in the queue
      putHeader (s.name);
      putString (s.name);
      putString ("=+");
      putTime   (e.computation);
      putString (" @");
      putTime   (e.activation);
      newLine ();
      
    } else {

      overlap = 0;
      // Wait for event activation
      delayUntil(e.activation);

      
      // Evaluate the computation time that can be possibly allocated to
      // handle this event, that is the computation time requested by
      // the event and the one available on the server. But the
      // computation time must not extend over the next replenishment in
      // order to take into account a replenishment that would happen
      // while handling an event. As a consequence, the final
      // computation time must not last after the next replenishment.
      c = (s.computation > e.computation) ? e.computation : s.computation;

      if (e.activation + c > nextActivation(s.period)) {
          overlap = 1;
          c = nextActivation(s.period) - e.activation;
      }

      // Update computation time needed to complete event in queue.
      // Remove the event once it is completed.
      // Do not update server budget yet.
      // We want to print the server status before and after this operation.
      //printf("Will compute for %d sec (task = %d)\n", c, e.computation);
      e.computation -= c;
      if (e.computation <= 0) {
          removeEvent(0);
      } else {
          setEvent(0, e);
      }

      // Print status of both server and event status
      putHeader    (s.name);
      putString    (s.name);
      putString    ("=");
      putTime      (s.computation);
      putString    ("-");
      putTime      (c);
      putString    (" ");
      putString    (e.name);
      putString    ("=");
      putTime      (e.computation);
      newLine ();

      // Update the server budget after this operation.

      // Simulate the execution of this event using
      // computeDuringTimeSpan. Provide the name of the event, its
      // worst case execution time, and the period of the server.
      // ATTENTION : the period parameter in computeDuringTimeSPan is
      // used to compute the execution priority. See tasks.h.
      s.computation -= c;
      computeDuringTimeSpan(s.name, d, s.period);

      // When the computation overlaps with a replenishment, we limit
      // the computation time to let this event arises. But we need to
      // force the server to extract a PRODUCE event. In this specific
      // case, force the server budget to zero.
      if (overlap)
          s.computation = 0;

      // Print event completion if needed
      if (e.computation == 0) {
          putHeader    (s.name);
          putString    ("completed ");
          putString    (e.name);
          newLine ();
      }
    }
  }

  return NULL;
}
