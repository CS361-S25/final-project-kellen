#ifndef CONFIG_SETUP_H
#define CONFIG_SETUP_H

#include "emp/config/config.hpp"

EMP_BUILD_CONFIG( MyConfigType,
  VALUE(SEED, int, 1, "Random number seed"),
  VALUE(NUM_START, int, 10, "Number of organisms to start with"),
  VALUE(MUTATION_RATE, double, 0.01, "Mutation rate per instruction"),
  VALUE(NUM_TASKS, int, 7, "How many tasks should be in the world?"),
  VALUE(FILE_PATH, std::string, "", "Output file path"),
  VALUE(FILE_NAME, std::string, "_data.dat", "Root output file name")
);

#endif
