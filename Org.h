#ifndef ORG_H
#define ORG_H

#include "CPU.h"
#include "OrgState.h"
#include "emp/Evolve/World_structure.hpp"

/// A class representing an individual digital organism in the simulation.
class Organism {
public:
  CPU cpu;                   ///< The CPU controlling this organism.
  int tasks_completed;       ///< Total number of tasks this organism has completed.

  /// Constructor to initialize the organism with optional points and task count.
  /// @param world The world in which the organism exists.
  /// @param points Initial points (fitness) of the organism.
  /// @param tasks_completed Initial count of tasks completed by the organism.
  Organism(emp::Ptr<OrgWorld> world, double points = 0.0, int tasks_completed = 0)
    : cpu(world), tasks_completed(tasks_completed) {
    SetPoints(points);
  }

  /// Set the number of tasks this organism has completed.
  /// @param numtasks The number of tasks to set for the organism.
  void SetTasksCompleted(int numtasks) { tasks_completed = numtasks; }

  /// Get the number of tasks this organism has completed.
  /// @return The number of tasks completed by the organism.
  double GetTasksCompleted() { return tasks_completed; }

  /// Increment the number of tasks this organism has completed by 1.
  void AddTaskCompleted() { tasks_completed++; }

  /// Get the ID of the last task this organism completed.
  /// @return The ID of the last task completed.
  int GetLastTaskCompleted() const { return cpu.state.last_task_completed; }

  /// Set the points (fitness) of this organism.
  /// @param _in The points value to set.
  void SetPoints(double _in) { cpu.state.points = _in; }

  /// Add to the points (fitness) of this organism.
  /// @param _in The number of points to add.
  void AddPoints(double _in) { cpu.state.points += _in; }

  /// Get the current points (fitness) of this organism.
  /// @return The current points (fitness) of the organism.
  double GetPoints() { return cpu.state.points; }

  /// Reset this organism’s internal CPU state, effectively resetting its state.
  void Reset() { cpu.Reset(); }

  /// Apply mutations to this organism’s genome, altering its behavior or traits.
  void Mutate() { cpu.Mutate(); }

  /**
   * Check whether the organism should reproduce based on its points.
   * If the organism has sufficient points (greater than 20), it will reproduce.
   * A mutated offspring will be returned.
   * If reproduction conditions are not met, std::nullopt is returned.
   * 
   * @return An optional Organism representing the offspring if reproduction occurs.
   */
  std::optional<Organism> CheckReproduction() {
    if (GetPoints() > 20) {
      Organism offspring = *this;
      offspring.Reset();
      offspring.Mutate();
      AddPoints(-20);  // Decrease points after reproduction
      return offspring;
    }
    return {};
  }

  /**
   * Execute a CPU step for this organism at a given world location.
   * The CPU step updates the organism's internal state based on its current
   * location and other factors.
   * 
   * @param current_location The organism's current location in the world.
   */
  void Process(emp::WorldPosition current_location) {
    cpu.state.current_location = current_location;
    cpu.RunCPUStep(10);
  }

  /// Print this organism’s genome to the standard output.
  void PrintGenome() {
    std::cout << "program ------------" << std::endl;
    cpu.PrintGenome();
    std::cout << "end ---------------" << std::endl;
  }
};

#endif // ORG_H
