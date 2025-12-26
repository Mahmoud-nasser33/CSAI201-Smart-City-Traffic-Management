#include "STATISTICS.h"

using namespace std;

Statistics::Statistics() {
    count = 0;
    total_vehicles = 0;

    canceled_count = 0;
    promoted_count = 0;

    ev_count = 0;
    pt_count = 0;
    nc_count = 0;
    fv_count = 0;

    sum_wait = 0;
    sum_cross = 0;

    for (int i = 0; i < 1000; i++) {
        list[i] = nullptr;
    }
}

Statistics::~Statistics() {
    for (int i = 0; i < count; i++) {
        if (list[i] != nullptr) {
            delete list[i];
        }
    }
}

void Statistics::add(Vehicle* v) {
    if (v == nullptr) {
        return;
    }
    if (count >= 1000) {
        return;
    }

    list[count] = v;
    count++;

    sum_wait = sum_wait + v->get_waiting_time();
    sum_cross = sum_cross + v->get_crossing_duration();
}

void Statistics::sort() {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (list[j]->get_crossing_time() > list[j + 1]->get_crossing_time()) {
                Vehicle* temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;
            }
        }
    }
}

void Statistics::print_statistics(bool detailed) const {
    cout << endl;

    if (detailed == false) {
        cout << "========== FINAL STATISTICS ==========" << endl;
        cout << "Total: " << total_vehicles << " | Completed: " << (count - canceled_count) << endl;
        cout << "Canceled: " << canceled_count << " | Promoted: " << promoted_count << endl;
    }
    else {
        cout << "==========================================" << endl;
        cout << "Simulation has finished successfully" << endl;
        cout << "==========================================" << endl;

        cout << "Total vehicles processed: " << total_vehicles << endl;
        cout << "------------------------------------------" << endl;
        cout << "Breakdown by vehicle type:" << endl;
        cout << "  Emergency Vehicles (EV): " << ev_count << endl;
        cout << "  Public Transport (PT):   " << pt_count << endl;
        cout << "  Normal Cars (NC):        " << nc_count << endl;
        cout << "  Freight Vehicles (FV):   " << fv_count << endl;

        cout << endl << "Events summary:" << endl;
        cout << "  Vehicles completed:       " << (count - canceled_count) << endl;
        cout << "  Vehicles canceled:        " << canceled_count << endl;
        cout << "  Vehicles promoted to EV:  " << promoted_count << endl;
        cout << "  Vehicles still in queues: " << (total_vehicles - count) << endl;

        if (count > 0) {
            double avg_wait = (double)sum_wait / count;
            double avg_cross = (double)sum_cross / count;

            double promo_rate = 0.0;
            if (total_vehicles > 0) {
                promo_rate = ((double)promoted_count / total_vehicles) * 100.0;
            }

            double cancel_rate = 0.0;
            if (total_vehicles > 0) {
                cancel_rate = ((double)canceled_count / total_vehicles) * 100.0;
            }

            cout << endl << "Performance metrics:" << endl;
            cout << "  Average Waiting Time:      " << avg_wait << " timesteps" << endl;
            cout << "  Average Crossing Duration: " << avg_cross << " timesteps" << endl;
            cout << "  Promotion Rate:            " << promo_rate << "%" << endl;
            cout << "  Cancellation Rate:         " << cancel_rate << "%" << endl;

            sort_and_print_vehicles();
        }

        cout << "Thank you for using the Traffic Management System" << endl;
        cout << "==========================================" << endl;
    }
}

void Statistics::sort_and_print_vehicles() const {
    if (count == 0) {
        return;
    }

    cout << endl << "========== COMPLETED VEHICLES ==========" << endl << endl;

    Vehicle* sorted[1000];
    for (int i = 0; i < count; i++) {
        sorted[i] = list[i];
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (sorted[j]->get_crossing_time() > sorted[j + 1]->get_crossing_time()) {
                Vehicle* temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }

    cout << left << setw(8) << "CT" << setw(8) << "ID" << setw(8) << "AT" << setw(8) << "WT"
        << setw(8) << "XD" << setw(8) << "TYPE" << setw(8) << "INT" << setw(8) << "LANE" << "STATUS" << endl;
    cout << "------------------------------------------------------------------------" << endl;

    for (int i = 0; i < count; i++) {
        Vehicle* v = sorted[i];

        if (v->get_is_canceled()) {
            cout << setw(8) << "--";
        }
        else {
            cout << setw(8) << v->get_crossing_time();
        }

        cout << setw(8) << v->get_id()
            << setw(8) << v->get_arrival_time()
            << setw(8) << v->get_waiting_time()
            << setw(8) << v->get_crossing_duration();

        if (v->get_type() == 1) {
            cout << setw(8) << "EV";
        }
        else if (v->get_type() == 2) {

            cout << setw(8) << "PT";
        }
        else if (v->get_type() == 3) {

            cout << setw(8) << "NC";
        }
        else {
            cout << setw(8) << "FV";
        }
        cout << setw(8) << v->get_intersection_id()
            << setw(8) << v->get_lane_id();

        if (v->get_is_canceled()) {
            cout << "CANCELED";
        }
        cout << endl;
    }
    cout << "========================================================================" << endl;
    cout << "Total vehicles listed: " << count << endl;
}

void Statistics::save_file() {
    ofstream f("output.txt");

    if (!f.is_open()) {
        return;
    }

    sort();

    f << "FT      ID      AT      WT      ST" << endl;

    for (int i = 0; i < count; i++) {
        Vehicle* v = list[i];

        f << v->get_crossing_time() << "       ";
        f << v->get_id() << "       ";
        f << v->get_arrival_time() << "       ";
        f << v->get_waiting_time() << "       ";
        f << v->get_crossing_duration();

        if (v->get_is_canceled()) {
            f << "       CANCELED";
        }

        f << endl;
    }
    f.close();
}

double Statistics::get_average_waiting_time() const {
    if (!count) {
        return 0;
    }
    return double(sum_wait) / count;
}

double Statistics::get_average_crossing_time() const {
    if (!count) {
        return 0;
    }
    return double(sum_cross) / count;
}

double Statistics::get_promotion_percentage() const {
    if (!total_vehicles) {
        return 0;
    }
    return double(promoted_count) / total_vehicles * 100;
}

double Statistics::get_cancellation_percentage() const {
    if (!total_vehicles) {
        return 0;
    }
    return double(canceled_count) / total_vehicles * 100;
}

void Statistics::inc_promote() {
    promoted_count++;
}

void Statistics::inc_cancel() {
    canceled_count++;
}

void Statistics::inc_EV() {
    ev_count++;
    total_vehicles++;
}

void Statistics::inc_PT() {
    pt_count++;
    total_vehicles++;
}

void Statistics::inc_NC() {
    nc_count++;
    total_vehicles++;
}

void Statistics::inc_FV() {
    fv_count++;
    total_vehicles++;
}