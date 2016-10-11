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
          NYI("remove event and wait for its activation");

	  // Update the server budget
          NYI("update server budget");

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
      NYI("remove event and wait for its activation time");

      // Update server budget
      NYI("update server budget");

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
      NYI ("evaluate computation time for event");

      // Wait for event activation
      NYI("wait for event activation");

      // Update computation time needed to complete event in queue.
      // Remove the event once it is completed.
      // Do not update server budget yet.
      // We want to print the server status before and after this operation.
      NYI("evaluate remaining computation time of current event");
      NYI("remove event when completed");
      NYI("update event in queue when not completed");

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
      p.kind = PRODUCE;
      p.name = s.name;
      NYI("schedule the replenish event");

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
      NYI("update server budget");
      NYI("compute event");

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
