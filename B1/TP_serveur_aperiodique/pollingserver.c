#include "events.h"
#include "periodictasks.h"
#include "tasks.h"

// Main procedure of polling server

void *runPollingServer (void *t){
  TaskInfo           s;
  EventInfo          e;
  TimeSpan           c = 0;
  Time               d = 0;
  int                i;
  
  s = periodicTaskTable[*(int *)t];

  // Schedule the initial replenishment event
  e.kind        = PRODUCE;
  e.name        = s.name;
  e.activation  = nextActivation (s.period);
  e.computation = s.computation;
  s.computation = 0;
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


    // When the first event to handle happens in the future, release the
    // server budget
    NYI("if next event happens in the future, release the server budget");

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
	  NYI("remove event and wait for its activation");
	  
	  // Update the server budget and schedule the next PRODUCE
	  // event. To do so compute the next activation time and the
	  // computation time related to this replenishment event.
	  NYI("update server budget and schedule replenishment");

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
      // Remove PRODUCE event and then wait for its activation time
      NYI("remove event and wait for its activation time");

      // As there are no Consume events to handle, the server discards
      // its budget. But it schedules its replenishment.
      NYI ("update server budget and schedule its replenishment");

      // Print the event pushed in the queue
      putHeader (s.name);
      putString ("@");
      putTime   (e.activation);
      putString (" ");
      putString (s.name);
      putString ("=+");
      putTime   (e.computation);
      newLine ();
      
    } else {

      // Wait for event activation
      NYI("wait for event activation");

      // Evaluate the computation time needed to handle this
      // event that is the computation time requested and the
      // one available on the server.
      NYI ("evaluate computation time for event");

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
      
      // Update the server budget after this operation.
      
      // Simulate the execution of this event using
      // computeDuringTimeSpan. Provide the name of the event, its
      // worst case execution time, and the period of the server.
      // ATTENTION : the period parameter in computeDuringTimeSPan is
      // used to compute the execution priority. See tasks.h.
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
