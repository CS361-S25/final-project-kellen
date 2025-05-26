#ifndef WORLD_H
#define WORLD_H

#include "emp/Evolve/World.hpp"
#include "emp/math/random_utils.hpp"
#include "emp/math/Random.hpp"
#include "Task.h"
#include "Org.h"
#include "ConfigSetup.h"

#include <memory>
#include <vector>
#include <iostream>

class OrgWorld : public emp::World<Organism> {
  emp::Random &random;
  std::vector<emp::WorldPosition> reproduce_queue;

public:
  // Add the DataMonitor pointer for the organism count
  emp::Ptr<emp::DataMonitor<int>> org_count;

  // Add the DataMonitor pointer for the cooperation probability
  emp::Ptr<emp::DataMonitor<int, emp::data::Histogram>> point_count;

  // Add
  emp::Ptr<emp::DataMonitor<int, emp::data::Histogram>> tasks_completed_monitor;

  // Store tasks using smart pointers to avoid memory leaks
  std::vector<std::unique_ptr<Task>> tasks;

  /**
   * @brief Construct a new OrgWorld object
   * 
   * Initializes the world with a random number generator and sets up the task list.
   * 
   * @param _random Random number generator for the world.
   */
  OrgWorld(emp::Random &_random) : emp::World<Organism>(_random), random(_random) {
    MyConfigType config;
    tasks.push_back(std::make_unique<NOT>());
    tasks.push_back(std::make_unique<NAND>());
    tasks.push_back(std::make_unique<AND>());
    tasks.push_back(std::make_unique<OR_N>());
    tasks.push_back(std::make_unique<OR>());
    tasks.push_back(std::make_unique<AND_N>());
    tasks.push_back(std::make_unique<NOR>());
    tasks.push_back(std::make_unique<XOR>());
    tasks.push_back(std::make_unique<EQU>());
  }

  /**
   * @brief Destructor for OrgWorld
   * 
   * Cleans up the DataMonitor objects to prevent memory leaks.
   */
  ~OrgWorld() {
    if (org_count) {
      org_count.Delete();  // Deallocate the DataMonitor if it exists
    }
    if (point_count) {
      point_count.Delete();  // Deallocate the DataMonitor if it exists
    }
  }

  /**
   * @brief Gets the population of the world
   * 
   * @return The current population of organisms.
   */
  const pop_t &GetPopulation() { return pop; }

  /**
   * @brief Retrieves the DataMonitor for the organism count
   * 
   * Initializes the monitor if not already created and sets up the update callback.
   * 
   * @return Reference to the DataMonitor for organism count.
   */
  emp::DataMonitor<int>& GetOrgCountDataNode() {
      if (!org_count) {
          org_count.New();  // Allocate a new DataMonitor if not already created
          OnUpdate([this](size_t) {
              org_count->Reset();  // Reset the monitor on update
              for (size_t i = 0; i < pop.size(); i++) {
                  if (IsOccupied(i)) {
                      org_count->AddDatum(1);  // Add data for each occupied organism
                  }
              }
          });
      }
      return *org_count;  // Return a reference to the DataMonitor
  }

  /**
   * @brief Retrieves the DataMonitor for cooperation probability
   * 
   * Initializes the monitor if not already created and sets up the update callback.
   * 
   * @return Reference to the DataMonitor for cooperation probability.
   */
  emp::DataMonitor<int, emp::data::Histogram>& GetPointValuesDataNode() {
      if (!point_count) {
          point_count.New();
          OnUpdate([this](size_t){
              point_count->Reset();  // Reset before each update
              for (size_t i = 0; i < pop.size(); ++i) {
                  if (IsOccupied(i)) {
                      point_count->AddDatum(pop[i]->GetPoints());
                  }
              }
          });
      }
      point_count->SetupBins(0,9.1,10); // Set up bins for cooperation values
      return *point_count;
  }

  /**
   * @brief Retrieves the DataMonitor for tasks completed
   * 
   * Initializes the monitor if not already created and sets up the update callback.
   * 
   * @return Reference to the DataMonitor for tasks completed.
   */
  emp::DataMonitor<int, emp::data::Histogram>& GetTasksCompletedDataNode() {
    if (!tasks_completed_monitor) {
      tasks_completed_monitor.New();
      OnUpdate([this](size_t){
        tasks_completed_monitor->Reset();
        for (size_t i = 0; i < pop.size(); ++i) {
          if (IsOccupied(i)) {
            int last = pop[i]->GetLastTaskCompleted();  // <-- new accessor needed
            if (last >= 0) {
              tasks_completed_monitor->AddDatum(last);
            }
          }
        }
      });
    }
    return *tasks_completed_monitor;
  }

  /**
   * @brief Updates the world by processing each organism and checking for reproduction
   */
  void Update() {
    emp::World<Organism>::Update();

    // Process each organism
    emp::vector<size_t> schedule = emp::GetPermutation(random, GetSize());
    for (int i : schedule) {
      if (!IsOccupied(i)) continue;
      pop[i]->Process(i);
      if (pop[i]->GetPoints() > 20) {
        ReproduceOrg(pop[i]->cpu.state.current_location);
      }
    }

    // Handle reproduction requests
    for (emp::WorldPosition location : reproduce_queue) {
      if (!IsOccupied(location)) continue;
      std::optional<Organism> offspring = pop[location.GetIndex()]->CheckReproduction();
      if (offspring.has_value()) {
        DoBirth(offspring.value(), location.GetIndex());
      }
    }
    reproduce_queue.clear();
  }

  /**
   * @brief Checks the output of an organism and assigns points based on the best task
   * 
   * @param output The output value to check.
   * @param state The current state of the organism.
   */
  void CheckOutput(float output, OrgState &state) {
    float best_points = 0.0;
    int best_task_index = -1;

    int task_index = -1;
    for (const auto &task : tasks) {
        task_index++;
        float task_points = task->CheckOutput(output, state.last_inputs);
        if (task_points > best_points) {
            best_points = task_points;
            best_task_index = task_index;
        }
    }

    if (best_task_index != -1) {
        state.points += best_points;
        state.last_task_completed = best_task_index;
    }
  }

  /**
   * @brief Reproduces an organism at the given location
   * 
   * Adds the location to the reproduction queue for future processing.
   * 
   * @param location The location where reproduction is requested.
   */
  void ReproduceOrg(emp::WorldPosition location) {
    reproduce_queue.push_back(location);
  }

  /**
   * @brief Extracts an organism from a given position
   * 
   * Removes the organism from the world and returns a pointer to it.
   * 
   * @param pos The position of the organism to extract.
   * @return A pointer to the extracted organism.
   */
  emp::Ptr<Organism> ExtractOrganism(size_t pos) {
      if (!IsOccupied(pos)) return nullptr;
      emp::Ptr<Organism> org = pop[pos];
      pop[pos] = nullptr;
      return org;
  }

  /**
   * @brief Moves an organism to a neighboring position
   * 
   * Randomly selects a neighboring position and moves the organism there.
   * 
   * @param pos The position of the organism to move.
   */
  void MoveOrganism(size_t pos) {
      if (!IsOccupied(pos)) return;  // No organism to move

      // Step 1: Extract the organism
      emp::Ptr<Organism> org = ExtractOrganism(pos);
      if (org == nullptr) return;

      // Step 2: Choose a random neighboring position
      emp::WorldPosition new_pos = GetRandomNeighborPos(pos);

      // Step 3: Skip if neighbor is occupied (optional logic)
      if (IsOccupied(new_pos)) {
          AddOrgAt(org, pos);  // Return organism to original position
          return;
      }

      // Step 4: Move organism to new position
      AddOrgAt(org, new_pos);
  }

  /**
   * @brief Sets up the file for storing data
   * 
   * Configures the file to record organism count, tasks completed, and other data.
   * 
   * @param filename The name of the file to write data to.
   * @return A reference to the data file.
   */
  emp::DataFile & SetupOrgFile(const std::string & filename) {
    auto & file = SetupFile(filename);

    // Point count data
    auto & point_node = GetPointValuesDataNode();
    file.AddVar(update, "update", "Update number");
    file.AddTotal(point_node, "points", "Total number of points earned");

    // Tasks completed data
    auto & task_node = GetTasksCompletedDataNode();
    file.AddMean(task_node, "mean_tasks", "Mean number of tasks completed");
    file.AddTotal(task_node, "total_tasks", "Total number of tasks completed");

    // Org count data
    auto & count_node = GetOrgCountDataNode();
    file.AddTotal(count_node, "total_orgs", "Total number of organisms ever in the world");

    // Define histogram bins manually for cooperation values (0.0 to 1.0, step 0.1)
    task_node.SetupBins(0, tasks.size(), tasks.size()); // tasks.size() should match # of tasks

    // Add one bin per task
    for (size_t i = 0; i < tasks.size(); ++i) {
        std::string label = "task_" + std::to_string(i);
        std::string desc = "Count of organisms that completed task " + std::to_string(i);
        file.AddHistBin(task_node, i, label, desc);
    }

    file.PrintHeaderKeys();
    
    return file;
  }
};

#endif
