#pragma once
#ifndef REROUTINGMAP_H
#define REROUTINGMAP_H

struct AdjNode {
    int target_id;
    int distance;
    AdjNode* next;
    AdjNode(int t, int d) {
        target_id = t;
        distance = d;
        next = nullptr;
    }
};

class ReroutingMap {
private:
    AdjNode* adj_list[100];
    bool enabled;
    int penalty;

public:
    // Initialization Function
    ReroutingMap();
    ~ReroutingMap();
    void add_connection(int from, int to, int dist);

    // Rerouting Structure
    int find_nearest_available(int blocked_id, bool* closed_status, int max_id);
    void set_rerouting_enabled(bool e);
    bool is_rerouting_enabled() const;
    void set_reroute_penalty(int p);
    int get_reroute_penalty() const;
};

#endif