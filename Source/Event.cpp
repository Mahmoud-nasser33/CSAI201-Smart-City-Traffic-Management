#include "Event.h"
#include <iostream>
using namespace std;

Event::Event(int t, int vType, int vID, int inter, int lane, int xd, int pr) {
    type = EVENT_ARRIVAL;
    time = t;
    vehicle_type = vType;
    vehicle_id = vID;
    intersection_id = inter;
    lane_id = lane;
    crossing_duration = xd;
    priority = pr;
    duration = 0;
}

Event::Event(EventType et, int t, int vID) {
    type = et;
    time = t;
    vehicle_id = vID;
    vehicle_type = 0;
    intersection_id = -1;
    lane_id = -1;
    crossing_duration = 0;
    duration = 0;
    priority = 0;
}

Event::Event(int t, int inter, int lane, int dur) {
    type = EVENT_ACCIDENT;
    time = t;
    intersection_id = inter;
    lane_id = lane;
    duration = dur;
    vehicle_id = 0;
    vehicle_type = 0;
    crossing_duration = 0;
    priority = 0;
}

Event::Event(int t, int inter, int dur) {
    type = EVENT_ROAD_CLOSURE;
    time = t;
    intersection_id = inter;
    duration = dur;
    lane_id = -1;
    vehicle_id = 0;
    vehicle_type = 0;
    crossing_duration = 0;
    priority = 0;
}

EventType Event::getType() const {
    return type;
}

int Event::getTime() const {
    return time;
}

int Event::getVehicleID() const {
    return vehicle_id;
}

int Event::getVehicleType() const {
    return vehicle_type;
}

int Event::getIntersectionID() const {
    return intersection_id;
}

int Event::getLaneID() const {
    return lane_id;
}

int Event::getDuration() const {
    return duration;
}

int Event::getCrossingDuration() const {
    return crossing_duration;
}

int Event::getPriority() const {
    return priority;
}

int Event::getOrderingKey() const {
    int typeOrder;
    switch (type) {
    case EVENT_CANCEL:
        typeOrder = 1;
        break;
    case EVENT_PROMOTE:
        typeOrder = 2;
        break;
    case EVENT_ACCIDENT:
    case EVENT_ROAD_CLOSURE:
        typeOrder = 3;
        break;
    case EVENT_ARRIVAL:
        typeOrder = 4;
        break;
    default:
        typeOrder = 5;
    }
    return -(time * 10 + typeOrder);
}

void Event::print() const {
    cout << "Event at timestep " << time << ": Type " << type << endl;
}