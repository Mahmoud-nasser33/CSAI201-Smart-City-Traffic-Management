#pragma once
#ifndef ORGANIZER_H
#define ORGANIZER_H

#include "Event.h"
#include "Intersection.h"
#include "PriorityQueue.h"
#include "ReroutingMap.h"
#include "STATISTICS.h"
#include "UI.h"
#include <string>

using namespace std;

class Organizer {
private:
  Intersection *intersections[100];
  int max_inter_id;

  PriorityQueue<Event> events;
  Statistics stats;
  ReroutingMap rerouting_map;
  UI ui;

  int current_timestep;
  int auto_promote_time;
  int cancel_thresh;
  int mode;

  // Helper functions for simulation
  void process_events();
  bool update_intersections();
  void try_start_crossing(Intersection *inter);

  // Helper functions for vehicle management
  void update_vehicle_stats(int vehicle_type);
  Vehicle *find_and_remove_vehicle(int vehicle_id);

  // Helper functions for rerouting
  void build_closed_map(bool closed_map[]);
  void reroute_vehicles_from_lane(Lane *lane, int target_inter_id);
  Lane *find_best_lane(int inter_id);
  void transfer_vehicle(Vehicle *v, int new_inter_id);

public:
  // Simulation control
  void set_mode(int m);
  void simulate();

  // Initialization
  Organizer();
  ~Organizer();
  bool load_file(string filename);

  // Event handlers
  void handle_arrival(Event *e);
  void handle_promote(Event *e);
  void handle_cancel(Event *e);
  void handle_accident(Event *e);
  void handle_closure(Event *e);

  // Utility
  void simple_delay(int milliseconds);
};
#endif