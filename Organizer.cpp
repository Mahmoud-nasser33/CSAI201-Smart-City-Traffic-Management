#include "Organizer.h"
#include <ctime>
#include <iostream>

using namespace std;

Organizer::Organizer() {
  max_inter_id = 0;
  current_timestep = 0;
  mode = 1;
  auto_promote_time = 0;
  cancel_thresh = 0;

  for (int i = 0; i < 100; i++) {
    intersections[i] = nullptr;
  }
}

Organizer::~Organizer() {
  for (int i = 0; i <= max_inter_id; i++) {
    if (intersections[i] != nullptr) {
      delete intersections[i];
    }
  }
}

void Organizer::set_mode(int m) {
  mode = m;
  ui.set_mode(m);
}

void Organizer::simple_delay(int milliseconds) {
  clock_t goal = clock() + (milliseconds * CLOCKS_PER_SEC / 1000);
  while (clock() < goal) {
  }
}

bool Organizer::load_file(string filename) {
  return ui.read_input_file(filename, intersections, max_inter_id, events,
                            rerouting_map, auto_promote_time, cancel_thresh);
}

void Organizer::simulate() {
  ui.print_startup_banner();

  bool running = true;
  while (running) {
    current_timestep++;
    ui.set_timestep(current_timestep);

    process_events();
    bool active = update_intersections();

    ui.print_system_state(intersections, max_inter_id,
                          rerouting_map.is_rerouting_enabled(),
                          rerouting_map.get_reroute_penalty());

    ui.wait_for_input();
    ui.delay(1000);

    if (events.isEmpty() && !active) {
      running = false;
    }
  }

  ui.print_simulation_end();
  stats.save_file();
  stats.print_statistics(true);
}

void Organizer::process_events() {
  Event *e;
  while (events.peek(e) && e->getTime() == current_timestep) {
    events.Dequeue();

    switch (e->getType()) {
    case EVENT_ARRIVAL:
      handle_arrival(e);
      break;
    case EVENT_PROMOTE:
      handle_promote(e);
      break;
    case EVENT_CANCEL:
      handle_cancel(e);
      break;
    case EVENT_ROAD_CLOSURE:
      handle_closure(e);
      break;
    case EVENT_ACCIDENT:
      handle_accident(e);
      break;
    }

    delete e;
  }
}

bool Organizer::update_intersections() {
  bool has_active_vehicles = false;

  for (int i = 0; i <= max_inter_id; i++) {
    if (intersections[i] == nullptr) {
      continue;
    }

    Intersection *inter = intersections[i];

    inter->check_and_reopen(current_timestep);
    inter->update_switching_timer();

    Vehicle *finished = inter->update_crossing(current_timestep);
    if (finished != nullptr) {
      stats.add(finished);
    }

    try_start_crossing(inter);
    inter->update_lane_waiting_times();

    if (!inter->isEmpty()) {
      has_active_vehicles = true;
    }
  }

  return has_active_vehicles;
}

void Organizer::try_start_crossing(Intersection *inter) {
  if (inter->is_vehicle_crossing() || !inter->can_switch()) {
    return;
  }

  Lane *next_lane = inter->select_next_green_lane(
      current_timestep, auto_promote_time, cancel_thresh);
  if (next_lane == nullptr) {
    return;
  }

  Vehicle *v = next_lane->get_next_vehicle();
  if (v != nullptr) {
    inter->start_crossing(v, current_timestep);
  }
}

void Organizer::handle_arrival(Event *e) {
  Vehicle *v = new Vehicle(e->getVehicleID(), e->getVehicleType(), e->getTime(),
                           e->getCrossingDuration());

  if (e->getVehicleType() == 1) {
    v->set_priority(e->getPriority());
  }

  v->set_auto_promotion_time(auto_promote_time);
  v->set_cancel_threshold(cancel_thresh);
  v->set_location(e->getIntersectionID(), e->getLaneID());

  update_vehicle_stats(e->getVehicleType());

  Intersection *inter = intersections[e->getIntersectionID()];
  if (inter != nullptr) {
    Lane *lane = inter->get_lane(e->getLaneID());
    if (lane != nullptr) {
      lane->add_vehicle_with_time(v, current_timestep);
    }
  }
}

void Organizer::update_vehicle_stats(int vehicle_type) {
  switch (vehicle_type) {
  case 1:
    stats.inc_EV();
    break;
  case 2:
    stats.inc_PT();
    break;
  case 3:
    stats.inc_NC();
    break;
  case 4:
    stats.inc_FV();
    break;
  }
}

Vehicle *Organizer::find_and_remove_vehicle(int vehicle_id) {
  for (int i = 0; i <= max_inter_id; i++) {
    if (intersections[i] == nullptr) {
      continue;
    }

    for (int l = 1; l <= 8; l++) {
      Lane *lane = intersections[i]->get_lane(l);
      Vehicle *v = lane->find_and_remove_vehicle(vehicle_id);
      if (v != nullptr) {
        return v;
      }
    }
  }
  return nullptr;
}

void Organizer::handle_promote(Event *e) {
  int id = e->getVehicleID();
  Vehicle *v = find_and_remove_vehicle(id);

  if (v != nullptr) {
    v->promote_to_EV();

    Intersection *inter = intersections[v->get_intersection_id()];
    Lane *lane = inter->get_lane(v->get_lane_id());
    lane->add_vehicle_with_time(v, current_timestep);

    stats.inc_promote();
    ui.print_event_message("Promoted Vehicle " + to_string(id));
  }
}

void Organizer::handle_cancel(Event *e) {
  int id = e->getVehicleID();
  Vehicle *v = find_and_remove_vehicle(id);

  if (v != nullptr) {
    v->set_is_canceled(true);
    stats.inc_cancel();
    stats.add(v);
    ui.print_event_message("Canceled Vehicle " + to_string(id));
  }
}

void Organizer::build_closed_map(bool closed_map[]) {
  for (int k = 0; k < 100; k++) {
    closed_map[k] = false;
  }

  for (int k = 0; k <= max_inter_id; k++) {
    if (intersections[k] && intersections[k]->is_intersection_closed()) {
      closed_map[k] = true;
    }
  }
}

void Organizer::reroute_vehicles_from_lane(Lane *lane, int target_inter_id) {
  Vehicle *temp[100];
  int count = 0;

  lane->get_all_PT_vehicles(temp, count);
  for (int k = 0; k < count; k++) {
    transfer_vehicle(temp[k], target_inter_id);
  }

  lane->get_all_NC_vehicles(temp, count);
  for (int k = 0; k < count; k++) {
    transfer_vehicle(temp[k], target_inter_id);
  }

  lane->get_all_FV_vehicles(temp, count);
  for (int k = 0; k < count; k++) {
    transfer_vehicle(temp[k], target_inter_id);
  }
}

void Organizer::handle_closure(Event *e) {
  int id = e->getIntersectionID();

  if (intersections[id] == nullptr) {
    return;
  }

  intersections[id]->close_intersection(e->getDuration(), current_timestep);
  ui.print_event_message("ROAD CLOSURE at Intersection " + to_string(id));

  if (!rerouting_map.is_rerouting_enabled()) {
    return;
  }

  bool closed_map[100];
  build_closed_map(closed_map);

  int target =
      rerouting_map.find_nearest_available(id, closed_map, max_inter_id);
  if (target == -1) {
    return;
  }

  for (int l = 1; l <= 8; l++) {
    Lane *lane = intersections[id]->get_lane(l);
    reroute_vehicles_from_lane(lane, target);
  }
}

void Organizer::handle_accident(Event *e) {
  int inter_id = e->getIntersectionID();
  int lane_id = e->getLaneID();

  if (intersections[inter_id] == nullptr) {
    return;
  }

  Lane *lane = intersections[inter_id]->get_lane(lane_id);
  if (lane == nullptr) {
    return;
  }

  lane->set_blocked(true);
  ui.print_event_message("ACCIDENT at Intersection " + to_string(inter_id) +
                         " Lane " + to_string(lane_id));

  if (!rerouting_map.is_rerouting_enabled()) {
    return;
  }

  bool closed_map[100];
  build_closed_map(closed_map);

  int target =
      rerouting_map.find_nearest_available(inter_id, closed_map, max_inter_id);
  if (target != -1) {
    reroute_vehicles_from_lane(lane, target);
  }
}

void Organizer::transfer_vehicle(Vehicle *v, int new_inter_id) {
  if (v == nullptr || intersections[new_inter_id] == nullptr) {
    return;
  }

  Lane *best_lane = find_best_lane(new_inter_id);
  if (best_lane == nullptr) {
    best_lane = intersections[new_inter_id]->get_lane(1);
  }

  if (best_lane != nullptr) {
    v->set_location(new_inter_id, best_lane->get_lane_id());
    v->add_penalty_time(rerouting_map.get_reroute_penalty());
    best_lane->add_vehicle_with_time(v, current_timestep);
  }
}

Lane *Organizer::find_best_lane(int inter_id) {
  Lane *best_lane = nullptr;
  int min_queue_count = 999999;

  for (int l = 1; l <= 8; l++) {
    Lane *lane = intersections[inter_id]->get_lane(l);
    if (lane == nullptr || lane->is_lane_blocked() || lane->is_lane_locked()) {
      continue;
    }

    int total_count =
        lane->get_EV_Queue().count() + lane->get_PT_Queue().count() +
        lane->get_NC_Queue().count() + lane->get_FV_Queue().count();

    if (total_count < min_queue_count) {
      min_queue_count = total_count;
      best_lane = lane;
    }
  }

  return best_lane;
}