#include "Organizer.h"
#include <iostream>

using namespace std;

int main() {
  Organizer system;

  cout << "Welcome to Traffic Simulation" << endl;
  cout << "Select Mode:" << endl;
  cout << "1. Interactive Mode (Press Enter)" << endl;
  cout << "2. Step-by-Step Mode (Auto)" << endl;
  cout << "3. Silent Mode (Output file direct)" << endl;
  int choice;
  cin >> choice;
  cin.ignore();

  system.set_mode(choice);

  if (system.load_file("input.txt")) {
    system.simulate();
  } else {
    cout << "Could not open input.txt" << endl;
  }
  return 0;
}