// Compile with `c++ -std=c++17 -Isignalgp-lite/include native.cpp`

#include <iostream>
#include "World.h"
#include "ConfigSetup.h" 
#include "emp/base/vector.hpp"
#include "emp/math/random_utils.hpp"
#include "emp/math/Random.hpp"
#include "emp/config/ArgManager.hpp"

// This is the main function for the NATIVE version of this project.

int main(int argc, char *argv[]) {
  // Access config values
  MyConfigType config;
  
  bool success = config.Read("MySettings.cfg");
  if(!success) config.Write("MySettings.cfg");

  auto args = emp::cl::ArgManager(argc, argv);

  if (args.ProcessConfigOptions(config, std::cout, "MySettings.cfg") == false) {
    std::cerr << "There was a problem processing the options file." << std::endl;
    exit(1);
  }

  if (args.TestUnknown() == false) {
    std::cerr << "Leftover args no good." << std::endl;
    exit(1);
  }

  std::cout << "Config after loading:\n";
  config.Write(std::cout);  // Dump all config values to console
  
  std::cout << "Start Organisms: " << config.NUM_START() << std::endl;
  std::cout << "Mutation Rate: " << config.MUTATION_RATE() << std::endl;
 // std::cout << "Task Difficulty: " << config.TASK_DIFFICULTY() << std::endl;

  // Some SignalGP-Lite functionality uses its own emp::Random instance
  // so it's important to set that seed too when the main Random is created
  sgpl::tlrand.Get().ResetSeed(config.SEED());  // Use SEED from the config

  emp::Random random(config.SEED());  // Use SEED from the config (manual for now)
  OrgWorld world(random); // This is where I would change the seed
  std::cout << "Random Seed: " << config.SEED() << std::endl;


  world.SetPopStruct_Grid(10, 10);
  // Set the size of the world
  world.Resize(10,10);

  // Setting up data file
  world.SetupOrgFile(config.FILE_PATH()+"Org_Vals"+std::to_string(config.SEED())+config.FILE_NAME());

  for (int i = 0; i < 10; i++){ // THis is also　adding 9 organisms to start each time even though the print says 1
            // This was causing me SO many issues
            Organism* new_org = new Organism(&world);
            world.Inject(*new_org);
        }

  for (int update = 0; update < 1000; update++) {
    world.Update();
    // Print the population size
    //std::cout << "Population size: " << world.GetNumOrgs() << std::endl;
  }

}
