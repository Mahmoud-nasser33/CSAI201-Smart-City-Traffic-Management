#include "Vehicle.h"

Vehicle::Vehicle(int id1, int type1, int at1, int xd1) {
    id = id1;
    type = type1;
    arrival_time = at1;
    crossing_duration = xd1;
    priority = 0;
    auto_promotion_time = 0;
    cancel_threshold = 0;
    waiting_time = 0;
    penalty_time = 0;
    start_crossing_time = 0;
    completion_time = 0;
    intersection_id = -1;
    lane_id = -1;
    is_crossing = false;
    is_canceled = false;
    was_promoted = false;

    if (type == 1) {
        priority = 4;
    }
    else if (type == 2) {
        priority = 3;
    }
    else if (type == 3) {
        priority = 1;
    }
    else if (type == 4) {
        priority = 2;
    }
}

int Vehicle::get_id() const {
    return id;
}

int Vehicle::get_type() const {
    return type;
}

int Vehicle::get_arrival_time() const {
    return arrival_time;
}

int Vehicle::get_crossing_duration() const {
    return crossing_duration;
}

int Vehicle::get_priority() const {
    return priority;
}

int Vehicle::get_urgency() const {
    return priority;
}

int Vehicle::get_intersection_id() const {
    return intersection_id;
}

int Vehicle::get_lane_id() const {
    return lane_id;
}

bool Vehicle::get_is_canceled() const {
    return is_canceled;
}

bool Vehicle::get_was_promoted() const {
    return was_promoted;
}

int Vehicle::get_crossing_time() const {
    return completion_time;
}

int Vehicle::get_waiting_time() const {
    return waiting_time + penalty_time;
}

void Vehicle::set_priority(int p) {
    priority = p;
}

void Vehicle::set_auto_promotion_time(int t) {
    auto_promotion_time = t;
}

void Vehicle::set_cancel_threshold(int t) {
    cancel_threshold = t;
}

void Vehicle::set_location(int inter, int lane) {
    intersection_id = inter;
    lane_id = lane;
}

void Vehicle::set_is_canceled(bool canceled) {
    is_canceled = canceled;
}

void Vehicle::promote_to_EV() {
    if (type == 2) {
        type = 1;
        priority = 4;
        was_promoted = true;
    }
}

void Vehicle::add_penalty_time(int p) {
    penalty_time += p;
}

void Vehicle::increment_waiting_time() {
    if (completion_time == 0) {
        waiting_time++;
    }
}

void Vehicle::set_start_crossing_time(int time) {
    start_crossing_time = time;
}

void Vehicle::set_completion_time(int time) {
    completion_time = time;
}

void Vehicle::print() const {
    cout << "V" << id << "_T" << type;
}