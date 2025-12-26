# Smart City Traffic Management System - CSAI 201

An event-driven traffic simulation system designed to model intelligent traffic control across multiple intersections. This project focuses on efficient vehicle scheduling, priority handling, and dynamic disruption management in a smart city environment.

## 🎓 Academic Context
- **Course**: CSAI 201 Data Structures
- **Institution**: University of Science and Technology, Zewail City
- **Term**: Fall 2025

## 🚀 Key Features
- **Intelligent Scheduling**: Specialized handling for Emergency Vehicles, Public Transport, and Freight.
- **Dynamic Events**: Processes real-time events such as accidents, road closures, and vehicle promotions.
- **Smart Rerouting**: Automatically redirects traffic to alternative intersections during disruptions.
- **Performance Analytics**: Tracks waiting times, intersection utilization, and system efficiency.

## 🛠️ Technical Implementation
The project is built using custom-designed data structures:
- **Priority Services**: Specialized queues for urgency-based vehicle selection.
- **FIFO Management**: Sequential queues for standard traffic flow.
- **Graph Topology**: An adjacency-based map for city-wide routing logic.
- **Simulation Engine**: An event-driven core that manages the simulation timestep lifecycle.

## 📁 Project Structure
- **Headers/**: Core data structures and entity declarations.
- **Source/**: Simulation logic and implementation files.
- **Tests/**: Standardized input scenarios for system validation.
