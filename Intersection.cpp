#include "Intersection.h"
#include <iostream>

const int MAX_LANES = 8;
const int NO_LANE = -1;
const int EV_TYPE = 1;

Intersection::Intersection(int id, int switch_cost) {
  intersection_id = id;
  lane_count = 0;

  traffic_light = new TrafficLight(id, switch_cost);

  current_crossing_vehicle = nullptr;
  crossing_remaining_time = 0;

  is_closed = false;
  closure_end_time = 0;
  total_busy_time = 0;
  peak_queue_length = 0;

  for (int i = 0; i < MAX_LANES; i++) {
    lanes[i] = nullptr;
  }
}

Intersection::~Intersection() {
  for (int i = 0; i < lane_count; i++) {
    if (lanes[i]) {
      delete lanes[i];
    }
  }
  if (traffic_light) {
    delete traffic_light;
  }
}

void Intersection::add_lane(Lane *lane) {
  if (lane && lane_count < MAX_LANES) {
    lanes[lane_count] = lane;
    lane_count++;
  }
}

Lane *Intersection::get_lane(int laneID) {
  for (int i = 0; i < lane_count; i++) {
    if (lanes[i]->get_lane_id() == laneID) {
      return lanes[i];
    }
  }
  return nullptr;
}

void Intersection::close_intersection(int duration, int current_time) {
  is_closed = true;
  closure_end_time = current_time + duration;
  traffic_light->turn_off_all_lights();
}

void Intersection::check_and_reopen(int current_time) {
  if (is_closed && current_time >= closure_end_time) {
    is_closed = false;
  }
}

void Intersection::update_switching_timer() { traffic_light->update_timer(); }

Vehicle *Intersection::update_crossing(int current_time) {
  if (current_crossing_vehicle) {
    crossing_remaining_time--;
    total_busy_time++;

    if (crossing_remaining_time <= 0) {
      Vehicle *finished_car = current_crossing_vehicle;
      finished_car->set_completion_time(current_time);
      current_crossing_vehicle = nullptr;
      return finished_car;
    }
  }
  return nullptr;
}

// Select next lane to get green light based on priority
Lane *Intersection::select_next_green_lane(int current_time, int auto_promote,
                                           int cancel_thresh) {
  if (is_closed || !traffic_light->can_switch() || current_crossing_vehicle) {
    return nullptr;
  }

  int best_lane_index = NO_LANE;
  bool found_emergency_vehicle = false;
  int max_ev_priority = -20;
  int max_wait_time = -1;

  for (int i = 0; i < lane_count; i++) {
    Lane *current_lane = lanes[i];

    if (!current_lane || current_lane->isEmpty()) {
      continue;
    }
    if (current_lane->is_lane_blocked() || current_lane->is_lane_locked()) {
      continue;
    }

    Vehicle *v = current_lane->peek_next_vehicle();
    if (!v) {
      continue;
    }

    if (v->get_type() == EV_TYPE) {
      int urgency = v->get_priority();
      int wait = v->get_waiting_time();
      int score = (urgency * 2) - wait;

      if (found_emergency_vehicle == false || score > max_ev_priority) {
        max_ev_priority = score;
        best_lane_index = i;
        found_emergency_vehicle = true;
      }
    } else if (found_emergency_vehicle == false) {
      int wait = v->get_waiting_time();
      if (wait > max_wait_time) {
        max_wait_time = wait;
        best_lane_index = i;
      }
    }
  }

  if (best_lane_index != NO_LANE) {
    int next_id = lanes[best_lane_index]->get_lane_id();
    traffic_light->start_switching(next_id);

    if (traffic_light->can_switch()) {
      return lanes[best_lane_index];
    }
    return nullptr;
  }

  traffic_light->set_green_lane(NO_LANE);
  return nullptr;
}

void Intersection::start_crossing(Vehicle *v, int current_time) {
  if (!v) {
    return;
  }
  current_crossing_vehicle = v;
  crossing_remaining_time = v->get_crossing_duration();
  v->set_start_crossing_time(current_time);
}

bool Intersection::can_switch() const {
  return !is_closed && traffic_light->can_switch();
}

bool Intersection::is_vehicle_crossing() const {
  return current_crossing_vehicle != nullptr;
}

bool Intersection::is_intersection_closed() const { return is_closed; }

bool Intersection::isEmpty() const {
  if (current_crossing_vehicle) {
    return false;
  }
  for (int i = 0; i < lane_count; i++) {
    if (lanes[i] && lanes[i]->isEmpty() == false) {
      return false;
    }
  }
  return true;
}

void Intersection::update_peak_queue_length() {
  int active_lanes = 0;
  for (int i = 0; i < lane_count; i++) {
    if (lanes[i] && lanes[i]->isEmpty() == false) {
      active_lanes++;
    }
  }
  if (active_lanes > peak_queue_length) {
    peak_queue_length = active_lanes;
  }
}

void Intersection::update_lane_waiting_times() {
  update_peak_queue_length();

  // Update waiting times for all vehicles in all lanes
  for (int i = 0; i < lane_count; i++) {
    if (lanes[i] != nullptr) {
      lanes[i]->update_waiting_times();
    }
  }
}

void Intersection::get_status_data(Status &s) {
  s.intersection_id = intersection_id;
  s.is_closed = is_closed;
  s.green_lane = traffic_light->get_current_green_lane();
  s.is_switching = traffic_light->is_currently_switching();
}

int Intersection::get_id() const { return intersection_id; }

int Intersection::get_current_green_lane_id() const {
  return traffic_light->get_current_green_lane();
}

Vehicle *Intersection::get_current_crossing_vehicle() const {
  return current_crossing_vehicle;
}

int Intersection::get_crossing_remaining_time() const {
  return crossing_remaining_time;
}

TrafficLight *Intersection::get_traffic_light() const { return traffic_light; }