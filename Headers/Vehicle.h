#pragma once
#ifndef VEHICLE_H
#define VEHICLE_H
#include <iostream>
using namespace std;

class Vehicle {
private:
  int id;
  int type;
  int arrival_time;
  int crossing_duration;
  int priority;

  int auto_promotion_time;
  int cancel_threshold;

  int waiting_time;
  int penalty_time;
  int start_crossing_time;
  int completion_time;

  int intersection_id;
  int lane_id;

  bool is_crossing;
  bool is_canceled;
  bool was_promoted;

public:
  // Constructor
  Vehicle(int id = 0, int type = 0, int at = 0, int xd = 0);
  void set_priority(int p);
  void set_location(int inter, int lane);

  // Promotion
  void set_auto_promotion_time(int t);
  bool get_was_promoted() const;
  void promote_to_EV();

  // Cancellation
  void set_cancel_threshold(int t);
  bool get_is_canceled() const;
  void set_is_canceled(bool canceled);

  // Rerouting
  void add_penalty_time(int p);

  // Priority calculation
  int get_urgency() const;

  // Crossing state
  void set_start_crossing_time(int time);

  // Completion
  void set_completion_time(int time);

  // Getters
  int get_id() const;
  int get_type() const;
  int get_arrival_time() const;
  int get_crossing_duration() const;
  int get_priority() const;
  int get_waiting_time() const;
  int get_crossing_time() const;
  int get_intersection_id() const;
  int get_lane_id() const;
  void increment_waiting_time();

  // Display
  void print() const;
};
#endif