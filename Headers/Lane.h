#pragma once
#ifndef LANE_H
#define LANE_H
#include "LinkedQueue.h"
#include "PriorityQueue.h"
#include "Vehicle.h"

class Lane {
private:
  PriorityQueue<Vehicle> EV_Queue;
  LinkedQueue<Vehicle *> PT_Queue;
  LinkedQueue<Vehicle *> NC_Queue;
  LinkedQueue<Vehicle *> FV_Queue;

  int lane_id;
  int intersection_id;

  bool is_locked;
  bool is_blocked_accident;

public:
  // Constructor and Destructor
  Lane(int l_id, int inter_id);
  ~Lane();

  // Add vehicle to appropriate queue
  void add_vehicle_with_time(Vehicle *v, int current_time);

  // Find and remove vehicle by ID
  Vehicle *find_and_remove_vehicle(int vehicle_id);

  // Lane blocking (for accidents)
  void set_blocked(bool b);
  bool is_lane_blocked() const;

  // Queue management
  bool isEmpty() const;
  PriorityQueue<Vehicle> &get_EV_Queue();
  LinkedQueue<Vehicle *> &get_PT_Queue();
  LinkedQueue<Vehicle *> &get_NC_Queue();
  LinkedQueue<Vehicle *> &get_FV_Queue();

  // Get all vehicles for rerouting
  void get_all_PT_vehicles(Vehicle **result, int &count);
  void get_all_NC_vehicles(Vehicle **result, int &count);
  void get_all_FV_vehicles(Vehicle **result, int &count);

  // Get next vehicle for crossing
  Vehicle *peek_next_vehicle();

  // Lane locking
  bool is_lane_locked() const;
  void set_locked(bool l);

  // Start vehicle crossing
  Vehicle *get_next_vehicle();

  // Update waiting times for all vehicles
  void update_waiting_times();

  int get_lane_id() const;
};
#endif