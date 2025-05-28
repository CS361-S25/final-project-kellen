#ifndef ORGSTATE_H
#define ORGSTATE_H

#include "emp/Evolve/World_structure.hpp"
#include <cstddef>

/// Forward declaration to avoid cyclic dependency with OrgWorld.
class OrgWorld;

/// Stores the internal state of an organism, including fitness, location, and recent inputs.
struct OrgState {
  emp::Ptr<OrgWorld> world;             ///< Pointer to the simulation world.

  float last_inputs[4];                 ///< Circular buffer of the 4 most recent input values.
  size_t last_input_idx = 0;            ///< Index of the most recent input in the buffer.
  double points = 0.0;                  ///< Fitness or score for the organism.
  emp::WorldPosition current_location;  ///< Current position in the world.

  int last_task_completed = -1;         ///< Last completed task ID; -1 means none.

  /// Add a new input to the circular buffer of recent inputs.
  void add_input(uint32_t input) {
    last_inputs[last_input_idx] = static_cast<float>(input);
    last_input_idx = (last_input_idx + 1) % 4;
  }
};

#endif // ORGSTATE_H
