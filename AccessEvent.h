/*
  AccessEvent.h
  
  Defines the objects used to log access events on the device.
  
*/

#ifndef __ACCESSEVENT__
#define __ACCESSEVENT__

#include "Arduino.h"
#include <Event.h>
#include <Timer.h>



// Represents one access event
// Type is one of the following:
//  0x30 : ‘Attempt’ (first authorization in double authorization mode).
//  0x31 : ‘Confirm’ (authorized user activated relay).
//  0x32 : ‘Logout’  (authorized user deactivated relay).
//  0x33 : ‘Fail’    (unauthorized user card detected).
//  0x34 : ‘Unknown’ (unknown user card detected).
class AccessEvent {
  public:
    byte type;
    unsigned long time;
    byte tag[4];
    void ToSerial();
  private:
    void PrintTag(byte *tag);
};

// Simple FIFO buffer of access events
// Tracks read and write event time
class EventList {
  public:
    EventList(Timer *t, int max_size);
   ~EventList();
    int getListSize();
    AccessEvent* getEvent();
    int addEvent(byte type, byte *tag);
    static unsigned long event_list_counter;
    
  private:
    AccessEvent *_list;
    int _list_max_size;
    int _list_start;
    int _list_stop;
    Timer *_t;
    unsigned int _isempty;
     
};

void TrackEventTime();
    
#endif // __ACCESSEVENT__
