#include "TrafficLight.h"

const int NO_LANE = -1;

TrafficLight::TrafficLight(int inter_id, int switch_cost) {
    intersection_id = inter_id;
    switching_cost = switch_cost;
    current_green_lane_id = NO_LANE;
    switching_timer = 0;
    is_switching = false;
}

void TrafficLight::set_green_lane(int lane_id) {
    current_green_lane_id = lane_id;
    is_switching = false;
    switching_timer = 0;
}

int TrafficLight::get_current_green_lane() const {
    return current_green_lane_id;
}

bool TrafficLight::can_switch() const {
    return !is_switching;
}

void TrafficLight::start_switching(int new_lane_id) {
    if (current_green_lane_id != NO_LANE && current_green_lane_id != new_lane_id) {
        is_switching = true;
        switching_timer = switching_cost;
        current_green_lane_id = new_lane_id;
    }
    else {
        current_green_lane_id = new_lane_id;
        is_switching = false;
        switching_timer = 0;
    }
}

void TrafficLight::update_timer() {
    if (is_switching && switching_timer > 0) {
        switching_timer--;
        if (switching_timer <= 0) {
            is_switching = false;
            switching_timer = 0;
        }
    }
}

bool TrafficLight::is_currently_switching() const {
    return is_switching;
}

int TrafficLight::get_switching_timer() const {
    return switching_timer;
}

int TrafficLight::get_switching_cost() const {
    return switching_cost;
}

void TrafficLight::set_switching_cost(int cost) {
    switching_cost = cost;
}

void TrafficLight::turn_off_all_lights() {
    current_green_lane_id = NO_LANE;
    is_switching = false;
    switching_timer = 0;
}