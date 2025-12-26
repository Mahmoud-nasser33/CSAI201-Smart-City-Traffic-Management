#include "UI.h"
#include <ctime>
#include <fstream>
#include <iostream>


using namespace std;

UI::UI() {
  current_timestep = 0;
  mode = 1;
}

void UI::set_mode(int m) {
     mode = m;
     }

void UI::set_timestep(int t) {
     current_timestep = t; 
    }

bool UI::read_input_file(string filename, Intersection **intersections,
                         int &max_inter_id, PriorityQueue<Event> &events,
                         ReroutingMap &rerouting_map, int &auto_promote_time,
                         int &cancel_thresh) {
  ifstream file(filename);

  if (!file.is_open()) {
    print_error("Cannot open file: " + filename);
    return false;
  }

  max_inter_id = 0;
  int num_inter;
  file >> num_inter;

  cout << "Loading " << num_inter << " intersections..." << endl;
  for (int i = 0; i < num_inter; i++) {
    int id, cost;
    file >> id >> cost;

    intersections[id] = new Intersection(id, cost);

    for (int l = 1; l <= 8; l++) {
      intersections[id]->add_lane(new Lane(l, id));
    }

    if (id > max_inter_id) {
      max_inter_id = id;
    }
  }

  int num_roads;
  if (file >> num_roads) {
  
    cout << "Loading " << num_roads << " road connections..." << endl;
    for (int i = 0; i < num_roads; i++) {
      int from, to, dist;
      file >> from >> to >> dist;
      rerouting_map.add_connection(from, to, dist);
    }
  }

  file >> auto_promote_time >> cancel_thresh;
  cout << "Auto-promote time: " << auto_promote_time << endl;
  cout << "Cancel threshold: " << cancel_thresh << endl;

  int num_events;
  file >> num_events;
  cout << "Loading " << num_events << " events..." << endl;

  char type;
  while (file >> type) {
    if (type == 'A') {

      int vType, time, id, inter, lane, xd;
      int pr = 0;

      file >> vType >> time >> id >> inter >> lane >> xd;

      if (vType == 1) {
        file >> pr;
      }

      Event *e = new Event(time, vType, id, inter, lane, xd, pr);
      events.Enqueue(e, e->getOrderingKey());

    } else if (type == 'X') {
      
        int time, id;
      file >> time >> id;
      Event *e = new Event(EVENT_CANCEL, time, id);
      events.Enqueue(e, e->getOrderingKey());
    } 
    
    else if (type == 'P') {
      int time, id;
      file >> time >> id;
      Event *e = new Event(EVENT_PROMOTE, time, id);
      events.Enqueue(e, e->getOrderingKey());
    }
    
    else if (type == 'B') {
      int time, inter, lane, dur;
      file >> time >> inter >> lane >> dur;
      Event *e = new Event(time, inter, lane, dur);
      events.Enqueue(e, e->getOrderingKey());
    }
    
    else if (type == 'C') {

      int time, inter, dur;
      file >> time >> inter >> dur;
      Event *e = new Event(time, inter, dur);
      events.Enqueue(e, e->getOrderingKey());
    }
  }

  rerouting_map.set_rerouting_enabled(true);
  rerouting_map.set_reroute_penalty(5);

  file.close();
  cout << "File loaded successfully!" << endl;
  return true;
}

void UI::print_startup_banner() {
  if (mode == 3) {
    return;
  }

  clear_screen();
  cout << "===========================================" << endl;
  cout << "      Traffic Simulation System     " << endl;
  cout << "===========================================" << endl;
  cout << "Starting simulation..." << endl;

  if (mode == 1) {
    cout << "Interactive Mode: Press ENTER to advance." << endl;
  }

  if (mode == 2) {
    cout << "Step-by-Step Mode: Auto-advancing." << endl;
  }

  cout << "===========================================" << endl;
  wait_for_input();
}

void UI::print_simulation_end() {
  if (mode == 3) {
    return;
  }
  cout << "===========================================" << endl;
  cout << "      Simulation Complete    " << endl;
  cout << "===========================================" << endl;
}

void UI::print_event_message(const string &message) {
  if (mode == 3) {
    return;
  }
  cout << " Event: " << message << endl;
}

void UI::print_error(const string &error) {
  cout << "ERROR: " << error << endl;
}

void UI::clear_screen() {
  if (mode != 3) {
    for (int i = 0; i < 50; i++) {
      cout << endl;
    }
  }
}

void UI::wait_for_input() {
  if (mode == 1) {
    if (cin.peek() == '\n') {
      cin.ignore();
    }
    cin.get();
  }
}

void UI::delay(int milliseconds) {
  if (mode == 2) {
    clock_t goal = clock() + (milliseconds * CLOCKS_PER_SEC / 1000);
    while (clock() < goal) {
    }
  }
}

void UI::print_system_state(Intersection **intersections, int max_inter_id,
                            bool rerouting_enabled, int reroute_penalty) {
  if (mode == 3) {
    return;
  }

  clear_screen();

  cout << "==============================================================="
       << endl;
  cout << "  Timestep: " << current_timestep << endl;

  if (rerouting_enabled) {
    cout << "  Rerouting enabled (Penalty: " << reroute_penalty << ")" << endl;
  }
  cout << "==============================================================="
       << endl;

  for (int i = 0; i <= max_inter_id; i++) {
    if (intersections[i] == nullptr) {
      continue;
    }

    Intersection *inter = intersections[i];

    cout << endl << " Intersection " << inter->get_id();

    Status s;
    inter->get_status_data(s);

    if (inter->is_intersection_closed()) {
      cout << " (Closed)";
    }

    if (s.is_switching) {
      cout << " (Switching)";
    }
    cout << endl;

    for (int l = 1; l <= 8; l++) {
      Lane *lane = inter->get_lane(l);

      if (lane == nullptr) {
        continue;
      }

      bool is_active = !lane->isEmpty();
      bool is_problem = lane->is_lane_locked() || lane->is_lane_blocked();

      if (is_active == false && is_problem == false) {
        continue;
      }

      cout << "   Lane " << l << ": ";

      if (lane->is_lane_locked()) {
        cout << "(Locked) ";
      }
      if (lane->is_lane_blocked()) {
        cout << "(Blocked) ";
      }

      cout << "EV:" << lane->get_EV_Queue().count() << " | ";
      cout << "PT:" << lane->get_PT_Queue().count() << " | ";
      cout << "NC:" << lane->get_NC_Queue().count() << " | ";
      cout << "FV:" << lane->get_FV_Queue().count();
      cout << endl;
    }

    int green_id = inter->get_current_green_lane_id();

    if (green_id != -1) {
      if (!s.is_switching && !s.is_closed) {
        cout << "   Green light: Lane " << green_id << endl;
      }
    }

    Vehicle *v = inter->get_current_crossing_vehicle();
    if (v != nullptr) {
      cout << "   Crossing: ";
      v->print();
      cout << " (Time left: " << inter->get_crossing_remaining_time() << ")"
           << endl;
    }
  }

  cout << "==============================================================="
       << endl;

  if (mode == 1) {
    cout << "Press Enter to continue..." << endl;
  }
}