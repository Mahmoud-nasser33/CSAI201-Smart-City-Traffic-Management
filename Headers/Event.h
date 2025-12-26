#pragma once
#ifndef EVENT_H
#define EVENT_H

#include "Vehicle.h"

enum EventType {
  EVENT_CANCEL = 1,
  EVENT_PROMOTE = 2,
  EVENT_ACCIDENT = 3,
  EVENT_ROAD_CLOSURE = 4,
  EVENT_ARRIVAL = 5
};

class Event {
private:
  EventType type;
  int time;
  int vehicle_id;
  int vehicle_type;
  int crossing_duration;
  int intersection_id;
  int lane_id;
  int duration;
  int priority;

public:
  // Constructors for different event types
  Event(int t, int vType, int vID, int inter, int lane, int xd, int pr = 0);
  Event(EventType et, int t, int vID);
  Event(int t, int inter, int lane, int dur);
  Event(int t, int inter, int dur);

  // Event ordering
  int getOrderingKey() const;

  // Getters
  EventType getType() const;
  int getTime() const;
  int getVehicleID() const;
  int getVehicleType() const;
  int getIntersectionID() const;
  int getLaneID() const;
  int getDuration() const;
  int getCrossingDuration() const;
  int getPriority() const;

  // Display
  void print() const;
};

#endif