#pragma once
#ifndef STATISTICS_H
#define STATISTICS_H

#include "Vehicle.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>


using namespace std;

class Statistics {
private:
  Vehicle *list[1000];
  int count;

  int total_vehicles;
  int promoted_count;
  int canceled_count;

  int ev_count, pt_count, nc_count, fv_count;
  int sum_wait;
  int sum_cross;

  void sort();
  double get_average_waiting_time() const;
  double get_average_crossing_time() const;
  double get_promotion_percentage() const;
  double get_cancellation_percentage() const;

public:
  // Constructor and Destructor
  Statistics();
  ~Statistics();

  // Add completed vehicle
  void add(Vehicle *v);

  // Counters
  void inc_promote();
  void inc_cancel();
  void inc_EV();
  void inc_PT();
  void inc_NC();
  void inc_FV();

  // Output
  void save_file();
  void print_statistics(bool detailed) const;

  // Display
  void sort_and_print_vehicles() const;
};

#endif