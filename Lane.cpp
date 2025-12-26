#include "Lane.h"

Lane::Lane(int l_id, int inter_id) {
  lane_id = l_id;
  intersection_id = inter_id;
  is_locked = false;
  is_blocked_accident = false;
}

Lane::~Lane() {}

void Lane::add_vehicle_with_time(Vehicle *v, int current_time) {
  if (v == nullptr) {
    return;
  }

  int type = v->get_type();

  if (type == 1) {
    int urgency = v->get_priority();
    int wt = current_time - v->get_arrival_time();
    int final_prio = (urgency * 2) - wt;
    EV_Queue.Enqueue(v, final_prio);

  } 
  else if (type == 2) {
    PT_Queue.enqueue(v);
  }
  
  else if (type == 3) {
    NC_Queue.enqueue(v);
  }
  
  else if (type == 4) {
    FV_Queue.enqueue(v);
  }
}

Vehicle *Lane::peek_next_vehicle() {
  if (is_blocked_accident || is_locked) {
    return nullptr;
  }

  Vehicle *v = nullptr;
  if (EV_Queue.peek(v)) {
    return v;
  }
  else if (PT_Queue.peek(v)) {
    return v;
  }
  else if (FV_Queue.peek(v)) {
    return v;
  }
  
  else if (NC_Queue.peek(v)) {
    return v;
  }

  return nullptr;
}

Vehicle *Lane::get_next_vehicle() {
  if (is_blocked_accident || is_locked) {
    return nullptr;
  }

  Vehicle *v = nullptr;
  if ((v = EV_Queue.Dequeue()) != nullptr) {
    return v;
  }

  if (PT_Queue.dequeue(v)) {
    return v;
  }
  if (FV_Queue.dequeue(v)) {
    return v;
  }
  if (NC_Queue.dequeue(v)) {
    return v;
  }
  return nullptr;
}

// Search and remove vehicle from all queues
Vehicle *Lane::find_and_remove_vehicle(int vehicle_id) {
  Vehicle *v = nullptr;
  LinkedQueue<Vehicle *> temp;

  // Search in PT_Queue
  while (PT_Queue.dequeue(v)) {
    if (v->get_id() == vehicle_id) {
      while (temp.dequeue(v)) {
        PT_Queue.enqueue(v);
      }
      return v;
    }
    temp.enqueue(v);
  }
  while (temp.dequeue(v)) {
    PT_Queue.enqueue(v);
  }

  // Search in NC_Queue
  while (NC_Queue.dequeue(v)) {
    if (v->get_id() == vehicle_id) {
      while (temp.dequeue(v)) {
        NC_Queue.enqueue(v);
      }
      return v;
    }
    temp.enqueue(v);
  }
  while (temp.dequeue(v)) {
    NC_Queue.enqueue(v);
  }

  // Search in FV_Queue
  while (FV_Queue.dequeue(v)) {
    if (v->get_id() == vehicle_id) {
      while (temp.dequeue(v)) {
        FV_Queue.enqueue(v);
      }
      return v;
    }
    temp.enqueue(v);
  }
  while (temp.dequeue(v)) {
    FV_Queue.enqueue(v);
  }

  // Search in EV_Queue (Priority Queue)
  PriorityQueue<Vehicle> temp_pq;
  while (!EV_Queue.isEmpty()) {
    v = EV_Queue.Dequeue();
    if (v->get_id() == vehicle_id) {

      // Restore remaining vehicles
      while (!temp_pq.isEmpty()) {
        Vehicle *restore = temp_pq.Dequeue();
        
        int urgency = restore->get_priority();
        int wt = restore->get_waiting_time();
        int prio = (urgency * 2) - wt;
        
        EV_Queue.Enqueue(restore, prio);
      }
      return v;
    }
    int urgency = v->get_priority();
    int wt = v->get_waiting_time();
    int prio = (urgency * 2) - wt;
    
    temp_pq.Enqueue(v, prio);
  }
  // Restore all vehicles
  while (!temp_pq.isEmpty()) {
    v = temp_pq.Dequeue();
    int urgency = v->get_priority();
    int wt = v->get_waiting_time();
    int prio = (urgency * 2) - wt;
    EV_Queue.Enqueue(v, prio);
  }

  return nullptr;
}

void Lane::get_all_PT_vehicles(Vehicle **result, int &count) {
  count = 0;
  Vehicle *v;
  while (PT_Queue.dequeue(v)) {
    result[count] = v;
    count++;
  }
}

void Lane::get_all_NC_vehicles(Vehicle **result, int &count) {
  count = 0;
  Vehicle *v;
  while (NC_Queue.dequeue(v)) {
    result[count] = v;
    count++;
  }
}

void Lane::get_all_FV_vehicles(Vehicle **result, int &count) {
  count = 0;
  Vehicle *v;
  while (FV_Queue.dequeue(v)) {
    result[count] = v;
    count++;
  }
}



bool Lane::isEmpty() const {
  return EV_Queue.isEmpty() && PT_Queue.isEmpty() && NC_Queue.isEmpty() &&
         FV_Queue.isEmpty();
}

void Lane::set_blocked(bool b) {
    
    is_blocked_accident = b; 

}

bool Lane::is_lane_blocked() const {
    
    return is_blocked_accident;

}

bool Lane::is_lane_locked() const {
    
    return is_locked;
}

void Lane::set_locked(bool l) {
    
    is_locked = l;
}

int Lane::get_lane_id() const { 
    
    return lane_id;
}

PriorityQueue<Vehicle> &Lane::get_EV_Queue() { 
    return EV_Queue;
}

LinkedQueue<Vehicle *> &Lane::get_PT_Queue() { 
    
    return PT_Queue;
}

LinkedQueue<Vehicle *> &Lane::get_NC_Queue() {
    return NC_Queue;

}

LinkedQueue<Vehicle *> &Lane::get_FV_Queue() {

    return FV_Queue;
 }

// Update waiting times for all vehicles in all queues
void Lane::update_waiting_times() {
  // Update EV_Queue
  PriorityQueue<Vehicle> temp_ev;

  while (!EV_Queue.isEmpty()) {
    Vehicle *v = EV_Queue.Dequeue();
    v->increment_waiting_time();

    int urgency = v->get_priority();
    int wt = v->get_waiting_time();
    int prio = (urgency * 2) - wt;

    temp_ev.Enqueue(v, prio);
  }
  while (!temp_ev.isEmpty()) {
    Vehicle *v = temp_ev.Dequeue();

    int urgency = v->get_priority();
    int wt = v->get_waiting_time();
    int prio = (urgency * 2) - wt;

    EV_Queue.Enqueue(v, prio);
  }

  // Update PT_Queue
  LinkedQueue<Vehicle *> temp_pt;
  Vehicle *v;

  while (PT_Queue.dequeue(v)) {
    v->increment_waiting_time();
    temp_pt.enqueue(v);
  }

  while (temp_pt.dequeue(v)) {
    PT_Queue.enqueue(v);
  }

  // Update NC_Queue
  LinkedQueue<Vehicle *> temp_nc;
  while (NC_Queue.dequeue(v)) {

    v->increment_waiting_time();
    temp_nc.enqueue(v);
  }
  while (temp_nc.dequeue(v)) {
    NC_Queue.enqueue(v);
  }

  // Update FV_Queue
  LinkedQueue<Vehicle *> temp_fv;
  while (FV_Queue.dequeue(v)) {
    v->increment_waiting_time();
    temp_fv.enqueue(v);
  }

  while (temp_fv.dequeue(v)) {
    FV_Queue.enqueue(v);
  }
}