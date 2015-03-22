/* La Fabrique

   AccessEvent class test 
   by David Beaudette
   
   Tests the AccessEvent and EventList class methods.
   
*/
#include "Timer.h"
#include "AccessEvent.h"

// Declare timer object
Timer t;

// Event type will cycle through the possible 
// Type is one of the following:
//  0x30 : ‘Attempt’ (first authorization in double authorization mode).
//  0x31 : ‘Confirm’ (authorized user activated relay).
//  0x32 : ‘Logout’  (authorized user deactivated relay).
//  0x33 : ‘Fail’    (unauthorized user card detected).
//  0x34 : ‘Unknown’ (unknown user card detected).
int event_type = 0x30;
byte tag[] = {0x10, 0x20, 0x30, 0x40};

// Using implicit zero-initialisation  
unsigned long EventList::event_list_counter;

// Declare list of events
EventList eventList(&t, 4);

void setup() {
  Serial.begin(115200);
  
  Serial.println("AccessEvent class test");
  Serial.println("---------");

  // Create an event to create an event every 3 s
  int createEvent = t.every(3000, CreateEvent);
  
  // Create an event to dump logging to screen every 20 s
  // 2 events should be lost
  int dumpEvent = t.every(20000, DumpLogging);
}

void loop() {
  // Update  timer
  t.update();
    
}

void CreateEvent() {
  Serial.print("Adding an event at time ");
  Serial.println(EventList::event_list_counter);
    // Add an event to the list
  eventList.addEvent(event_type, tag);  

  // Modify event_type
  event_type = (0x34 == event_type) ? 0x30 : (event_type+1);
  
  // Increment user tag
  tag[0] += 1;
  tag[1] += 2;
  tag[2] += 3;
  tag[3] += 4;
}

void DumpLogging() {
  AccessEvent *event;
  int list_size = eventList.getListSize();
  Serial.print("Dumping events; list size is ");
  Serial.println(list_size);
  for(int i = 0; i < list_size; i++) {
    event = eventList.getEvent();
    Serial.println("---------");
    event->ToSerial();
  }    
  Serial.println("---------");
}
