# Micro Mouse Robot - RoboFest 2023 Winner

This repository contains the code for our victorious robot that successfully navigated a 14x14 maze using the Flood Fill algorithm with additional optimizations.

## Features

- Autonomous maze-solving using Flood Fill algorithm
- Sensor fusion with TOF, encoders, and Sharp IR sensors
- ESP32 platform implementation
- Arduino IDE programming

### Flood Fill Algorithm
The flood fill algorithm involves assigning values to each of the cells in a maze where these values represent the distance from any cell on a maze to the destination cell. The flood fill algorithm mainly contains four parts: update walls, flood maze, turn determination and move to next cell.
