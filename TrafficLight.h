#pragma once
#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

class TrafficLight {
private:
  int intersection_id;
  int current_green_lane_id;
  int switching_cost;
  int switching_timer;
  bool is_switching;

public:
  // Constructor
  TrafficLight(int inter_id, int switch_cost);

  // Light control
  void turn_off_all_lights();

  // Switching logic
  void set_green_lane(int lane_id);
  int get_current_green_lane() const;
  bool can_switch() const;
  void start_switching(int new_lane_id);
  void update_timer();
  bool is_currently_switching() const;
  int get_switching_timer() const;
  int get_switching_cost() const;
  void set_switching_cost(int cost);
};

#endif