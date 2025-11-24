# Process Scheduling Solver

A powerful web-based tool for visualizing and calculating various CPU process scheduling algorithms. Built with C++ and the Wt (Web Toolkit) library, this application provides an interactive interface to understand how different scheduling algorithms manage process execution.

### [Live Demo](https://process-scheduling-solver-v2.onrender.com/)

## 🚀 Features

*   **Interactive Web Interface**: User-friendly GUI built with Wt for seamless interaction.
*   **Dynamic Gantt Chart**: Visual representation of process execution timeline.
*   **Real-time Statistics**: Automatically calculates and displays:
    *   Average Waiting Time
    *   Average Turnaround Time
    *   Average Response Time
    *   Throughput (Average Burst Time)
*   **Customizable Inputs**: Add any number of processes with custom Arrival Time, Burst Time, and Priority.

## 🧠 Supported Algorithms

The solver supports the following scheduling algorithms:

1.  **FCFS** (First Come First Serve)
2.  **SJF** (Shortest Job First)
3.  **Priority** (Non-Preemptive)
4.  **Round Robin** (RR) - *Requires Time Quantum*
5.  **SRTF** (Shortest Remaining Time First)
6.  **Priority** (Preemptive)
7.  **I/O Scheduling**

## 🛠️ Prerequisites

To build and run this project locally, you need:

*   **C++ Compiler**: `g++` (supports C++14 or later)
*   **Wt Library**: The Web Toolkit library must be installed on your system.
    *   [Wt Installation Guide](https://www.webtoolkit.eu/wt/doc/reference/html/InstallationUnix.html)

## 📦 Installation & Compilation (Linux)

1.  **Clone the repository**
    ```bash
    git clone https://github.com/yourusername/ProcessSchedulingSolver_wt_v2.git
    cd ProcessSchedulingSolver_wt_v2
    ```

2.  **Compile the project**
    Use the following command to compile the application:
    ```bash
    g++ _main.cc -include Updaters.hpp -include Results.hpp -include EventHandlers.hpp -include Design-UI.hpp -o _main -lwt -lwthttp
    ```

## 🚀 Usage (Linux)

1.  **Run the application**
    ```bash
    ./_main --docroot . --http-address 0.0.0.0 --http-port 8080
    ```

2.  **Access the interface**
    Open your web browser and navigate to:
    `http://localhost:8080`

3.  **Using the Solver**:
    *   Select an algorithm from the dropdown menu.
    *   Enter the number of processes.
    *   (Optional) Set Time Quantum for Round Robin.
    *   Fill in the Process Table (Arrival Time, Burst Time, Priority).
    *   Click **Calculate** to see the Gantt chart and statistics.