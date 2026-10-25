
# Smart City Route Finding and Traffic Management System

## Project Overview
This project is a C-based Smart City Route Finding and Traffic Management System. It helps users find routes between locations and manage simulated traffic conditions.

The project uses graph data structures and Dijkstra's shortest-path algorithm to calculate routes based on distance and travel time.

## Features
- Add and display locations.
- Add and display roads between locations.
- Find and compare routes.
- Calculate routes based on distance and travel time.
- Update traffic levels: Low, Medium, and High.
- Block and unblock roads.
- Find nearby hospitals, petrol pumps, railway stations, airports, and police stations.
- Find emergency routes to hospitals.
- Search for locations by name.

## Technologies Used
- Programming Language: C
- Data Structures: Graph, adjacency list, linked list, and minimum heap
- Algorithm: Dijkstra's Shortest-Path Algorithm

## How to Run

### Compile the Program
Make sure a C compiler such as GCC is installed.

```bash
gcc smart_city.c -o smart_city
```

### Run on Windows
```bash
smart_city.exe
```

### Run on Linux or macOS
```bash
./smart_city
```

## Traffic Management
The program simulates traffic conditions and adjusts route calculations according to the traffic levels. It also supports road blocking and unblocking.

**Note:** The current project uses simulated traffic conditions and does not connect to a live traffic API.

## Project Files
- `smart_city.c` — Main C source code.
- `README.md` — Project description and instructions.

## Future Improvements
- Integration with live traffic APIs.
- Graphical user interface.
- Digital city map with route visualization.
- Saving city and road data in external files.

## Author
SONAKSHI PANDEY & SANIKA SHERIKAR
