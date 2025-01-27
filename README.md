# Jurassic Park Simulator

## Introduction  
This is a **C program** that simulates a jurassic park ride system using multithreading. It manages passenger arrivals, a queue system, and ride vehicles. Outputs include total arrivals, riders, rejected passengers, and average waiting times.

---

## How It Works  
1. **Setup**:  
   - Passengers arrive at different rates throughout the day.
   - Explorers load passengers from a queue, up to the maximum capacity per explorer.

2. **Simulation Details**:  
   - Passengers are added to a queue unless it reaches a limit (800).  
   - Explorers operate independently and load passengers.  

3. **Output**:  
   - Logs passenger activity and queue status to `output.txt`.  
   - Displays summary statistics at the end (e.g., total arrivals, riders, and average wait times).


