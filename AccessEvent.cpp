/*
  AccessEvent.cpp
  
  Defines the objects used to log access events on the device.

*/

#include "AccessEvent.h"

EventList::EventList(Timer *t, int max_size) {
  // Reset FIFO and counter
  _list_max_size = max_size;
  _list = new AccessEvent[max_size];
  _list_start    = 0;
  _list_stop     = 0; 
  _isempty       = 1;
  
  // Set timer instance
  _t = t;
  int trackEvent = _t->every(1000, TrackEventTime);
}

EventList::~EventList() {
  delete[] _list;
}

AccessEvent* EventList::getEvent() {
  // If the list is empty, just reset the counter
  if(_isempty) { 
    EventList::event_list_counter = 0UL;
    return NULL; 
  }
  // Retrieve the oldest event 
  AccessEvent *oldest = &_list[_list_start];
  
  // The desired time is the number of seconds since the 
  // event happened. This is computed from the current counter
  // value minus the counter value when the event happened. 
  _list[_list_start].time = EventList::event_list_counter - 
                            _list[_list_start].time;
  
  // Remove event from FIFO
  _list_start += 1;  
  if(_list_start >= _list_max_size) {
    // Wrap around
    _list_start = 0;
  }
  
  // If this was the last recorded event, reset counter
  if(_list_start == _list_stop) {
    EventList::event_list_counter = 0UL;
    _isempty = 1;  
  }
  
  // Return event 
  return oldest;
}

int EventList::addEvent(byte type, byte *tag) {
  // Check FIFO state
  if(getListSize() == _list_max_size) {
    // FIFO full
    return -1;
  }
  
  // Assign event information
  _list[_list_stop].type = type;
  _list[_list_stop].time = EventList::event_list_counter;
  memcpy(&_list[_list_stop].tag[0], tag, 4 * sizeof(byte));
  
  // Add event to FIFO
  _list_stop += 1;
  if(_list_stop >= _list_max_size) {
    // Wrap around
    _list_stop = 0;
  }
  if(_isempty) {
    // Reset counter
    EventList::event_list_counter = 0UL;
    _isempty = 0;
  }
  
  return 0;
}

int EventList::getListSize() {
  // If the list is empty, just reset the counter
  if(_isempty) { 
    EventList::event_list_counter = 0UL;
    return 0; 
  }
  if(_list_stop > _list_start) {
    return (_list_stop - _list_start);
  }
  else {
    return (_list_stop + (_list_max_size - _list_start));
  }
}

void AccessEvent::ToSerial() {
  // Print time
  Serial.print("Event happened ");
  Serial.print(this->time);
  Serial.println(" seconds ago.");
  // Print type
  Serial.print("The type of the event is: ");
  Serial.println(this->type, HEX);
  // Print tag id
  PrintTag(this->tag);
}

void AccessEvent::PrintTag(byte *serial) {
  Serial.println("The serial nb of the tag is:");
  for (int i = 0; i < 3; i++) {
    Serial.print(serial[i], HEX);
    Serial.print(", ");
  }
  Serial.println(serial[3], HEX);
}

void TrackEventTime() {
  // Increment counter 
  EventList::event_list_counter += 1UL;
  //Serial.print("Event list counter value = ");
  //Serial.println(EventList::event_list_counter);
}

