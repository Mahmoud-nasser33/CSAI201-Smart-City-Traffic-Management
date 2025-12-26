#pragma once
#ifndef UI_H
#define UI_H

#include "Event.h"
#include "Intersection.h"
#include "Lane.h"
#include "PriorityQueue.h"
#include "ReroutingMap.h"
#include "Vehicle.h"
#include <iostream>
#include <string>


using namespace std;

class UI {
private:
  int current_timestep;
  int mode;

public:
  // Initialization
  UI();
  void set_mode(int m);
  void set_timestep(int t);

  // Input file reading
  bool read_input_file(string filename, Intersection **intersections,
                       int &max_inter_id, PriorityQueue<Event> &events,
                       ReroutingMap &rerouting_map, int &auto_promote_time,
                       int &cancel_thresh);

  // User interaction
  void wait_for_input();

  // Display functions
  void print_startup_banner();
  void print_simulation_end();
  void print_system_state(Intersection **intersections, int max_inter_id,
                          bool rerouting_enabled, int reroute_penalty);

  // Timing
  void delay(int milliseconds);

  // Messages
  void print_event_message(const string &message);
  void print_error(const string &error);
  void clear_screen();
};

#endif