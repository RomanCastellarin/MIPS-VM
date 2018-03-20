// SIMULATOR.H
#ifndef SIMULATOR_H
#define SIMULATOR_H

/*
 * This module provides methods for starting and reinitializing the simulator
 * as well as the core function -step- which performs one step of the simulation
 */

// initialize simulator
int initialize();

// perform one step
int step();

// reset simulator
int reset();

#endif
