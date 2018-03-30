// SIMULATOR.H
#ifndef SIMULATOR_H
#define SIMULATOR_H

/*
 * This module provides methods for starting and reinitializing the simulator
 * as well as the core function -step- which performs one step of the simulation
 */


#include "memory.h"
#include <set>

// Breakpoints
extern std::set<addr_t> Breakpoints;

// initialize simulator
int initialize();

// perform one step
int step();

// reset simulator
int reset();

// shell
int shell();

#endif
