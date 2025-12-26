#include "ReroutingMap.h"
#include <iostream>

ReroutingMap::ReroutingMap() {
    for (int i = 0; i < 100; i++) adj_list[i] = nullptr;
    enabled = false;
    penalty = 0;
}

ReroutingMap::~ReroutingMap() {
    for (int i = 0; i < 100; i++) {
        AdjNode* curr = adj_list[i];
        while (curr) {
            AdjNode* temp = curr;
            curr = curr->next;
            delete temp;
        }
    }
}

void ReroutingMap::add_connection(int from, int to, int dist) {
    if (from < 0 || from >= 100) return;

    AdjNode* node = new AdjNode(to, dist);
    node->next = adj_list[from];
    adj_list[from] = node;

    if (to >= 0 && to < 100) {
        AdjNode* back = new AdjNode(from, dist);
        back->next = adj_list[to];
        adj_list[to] = back;
    }
}

int ReroutingMap::find_nearest_available(int blocked_id, bool* closed_status, int max_id) {
    if (blocked_id < 0 || blocked_id >= 100) return -1;

    AdjNode* curr = adj_list[blocked_id];

    int best_id = -1;
    int min_dist = 99;

    while (curr) {
        int neighbor = curr->target_id;

        if (neighbor <= max_id && !closed_status[neighbor]) {
            if (curr->distance < min_dist) {
                min_dist = curr->distance;
                best_id = neighbor;
            }
        }
        curr = curr->next;
    }

    return best_id;
}

void ReroutingMap::set_rerouting_enabled(bool e) {
    enabled = e;
}

bool ReroutingMap::is_rerouting_enabled() const {
    return enabled;
}

void ReroutingMap::set_reroute_penalty(int p) {
    penalty = p;
}

int ReroutingMap::get_reroute_penalty() const {
    return penalty;
}