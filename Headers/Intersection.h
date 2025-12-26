#pragma once
#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "Lane.h"
#include "TrafficLight.h"
#include "Vehicle.h"


struct Status {
  int intersection_id;
  bool is_closed;
  int green_lane;
  bool is_switching;
};

class Intersection {
private:
  int intersection_id;
  Lane *lanes[8];
  int lane_count;

  TrafficLight *traffic_light;

  Vehicle *current_crossing_vehicle;
  int crossing_remaining_time;

  bool is_closed;
  int closure_end_time;

  int total_busy_time;
  int peak_queue_length;

public:
  // Initialization
  Intersection(int id, int switch_cost);
  ~Intersection();
  void add_lane(Lane *lane);

  // Lane management
  Lane *get_lane(int laneID);
  bool isEmpty() const;

  // Road closure
  void check_and_reopen(int current_time);
  void close_intersection(int duration, int current_time);
  bool is_intersection_closed() const;

  // Traffic light control
  Lane *select_next_green_lane(int current_time, int auto_promote,
                               int cancel_thresh);
  void update_switching_timer();
  bool can_switch() const;

  // Vehicle crossing
  bool is_vehicle_crossing() const;
  Vehicle *update_crossing(int current_time);
  void start_crossing(Vehicle *v, int current_time);

  // Statistics and state
  void update_lane_waiting_times();
  int get_id() const;
  int get_current_green_lane_id() const;
  Vehicle *get_current_crossing_vehicle() const;
  int get_crossing_remaining_time() const;
  TrafficLight *get_traffic_light() const;

  // UI display
  void get_status_data(Status &s);

  // Statistics
  void update_peak_queue_length();
};
#endif