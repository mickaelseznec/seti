#include "events.h"
#include "periodictasks.h"
#include "tasks.h"

// Main procedure of sporadic server

void *runSporadicServer (void *t){
  TaskInfo           s;
  EventInfo          e;
  EventInfo          p;
  TimeSpan           c = 0;
  Time               d = 0;
  int                i;

  s = periodicTaskTable[*(int *)t];

  while (systemNotCompleted () && (0 <= lastEvent)) {
    e = getEvent (firstEvent);
    
    // Before handling next event check the server budget is not empty
    if (s.computation == 0) {
      for (i=firstEvent; i <= lastEvent; i++){
	e = getEvent(i);
	if (e.kind == PRODUCE) {
	  if (systemCompletedAt (e.activation)) {
	    return NULL;
	  }

          // Remove PRODUCE event and then wait for its activation time.
          removeEvent(i);
          delayUntil(e.activation);

	  // Update the server budget
          s.computation = e.computation;

           // Print the arrival of this event
          putHeader (s.name);
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

      // Remove PRODUCE event and then wait for its activation time
      removeEvent(i);
      delayUntil(e.activation);


      // Update server budget
      s.computation = e.computation;

      // Print the arrival of this event
      putHeader (s.name);
      putString (s.name);
      putString("=+");
      putTime (e.computation);
      newLine ();
      
    } else {

      // Evaluate the computation time needed to handle this
      // event that is the computation time requested and the
      // one available on the server.
      if (e.computation > s.computation)
          c = s.computation;
      else
          c = e.computation;


      // Wait for event activation
      delayUntil(e.activation);

      // Update computation time needed to complete event in queue.
      // Remove the event once it is completed.
      // Do not update server budget yet.
      // We want to print the server status before and after this operation.
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

      // Schedule the replenishment with the appropriate computation time
      s.computation -= c;

      p.kind = PRODUCE;
      p.name = s.name;
      p.computation = c;
      p.activation  = roundClock(localClock()) + s.period;
      appendEvent (p);

      // Print the event pushed in the queue
      putHeader (s.name);
      putString ("@");
      putTime   (p.activation);
      putString (" ");
      putString (s.name);
      putString ("=+");
      putTime   (p.computation);
      newLine ();

      // Update the server budget after this operation.
      
      // Simulate the execution of this event using
      // computeDuringTimeSpan. Provide the name of the event, its
      // worst case execution time, and the period of the
      // server. ATTENTION : the period parameter in
      // computeDuringTimeSPan is used to compute the execution
      // priority. See tasks.h.
      computeDuringTimeSpan(s.name, c, s.period);

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
