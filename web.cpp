#define UIT_VENDORIZE_EMP
#define UIT_SUPPRESS_MACRO_INSEEP_WARNINGS
#include "emp/math/Random.hpp"
#include "emp/web/Animate.hpp"
#include "emp/web/Document.hpp"
#include "emp/web/web.hpp"
#include "World.h"
#include "Org.h"
#include "ConfigSetup.h"
#include "emp/config/ArgManager.hpp"
#include "emp/prefab/ConfigPanel.hpp"
#include "emp/web/UrlParams.hpp"

emp::web::Document doc("target");
emp::web::Document settings("settings");
emp::web::Document controls("controls");
MyConfigType config;

const std::unordered_map<int, std::string> task_colors = {
  {0, "blue"},   // Task 0: NOT
  {1, "green"},  // Task 1: NAND
  {2, "red"},    // Task 2: AND
  {3, "orange"}, // Task 3: OR_N
  {4, "purple"}, // Task 4:  OR
  {5, "pink"},   // Task 5: AND_N
  {6, "cyan"},    // Task 6: NOR
  {7, "yellow"},    // Task 7: XOR
  {8, "magenta"},    // Task 8: EQU
  {9, "gray"}    // Task 9: No task completed
};

/**
 * @brief AEAnimator class that manages animation of organisms in a simulation.
 *
 * This class handles the setup of the canvas, configuration, world, and draws the organisms
 * on each frame. It controls the movement, drawing, and simulation of digital organisms.
 */
class AEAnimator : public emp::web::Animate {

    const int num_h_boxes = 10; ///< Number of height boxes in the grid.
    const int num_w_boxes = 10; ///< Number of width boxes in the grid.
    const double RECT_SIDE = 10; ///< Side length of each rectangle representing an organism.
    const double width{num_w_boxes * RECT_SIDE}; ///< Total width of the canvas.
    const double height{num_h_boxes * RECT_SIDE}; ///< Total height of the canvas.

    // ==+++=++++++++++++++++++++++++++++++++++++++++++ ADD config.SEED() +++++++++++++++==//
    emp::Random random{23904}; ///< Random number generator initialized with the seed.



    OrgWorld world{random}; ///< The world where organisms live and interact.
    emp::web::Canvas canvas{width, height, "canvas"}; ///< Canvas for drawing the world.

public:
    /**
     * @brief Constructor to initialize the AEAnimator.
     *
     * Sets up the canvas, configuration, and world for the simulation.
     */
    AEAnimator() {
        SetupCanvas();
        SetupConfig();
        SetupWorld();
    }

    /**
     * @brief Sets up the canvas and adds it to the document.
     *
     * This function initializes the canvas and embeds it in the HTML document. It also adds
     * a description of the simulation and its settings in a styled div.
     */
    void SetupCanvas() {
        doc << canvas;

        // Adds a description of the simluation to the webpage
        doc << R"(
                <div style="margin-top:1em; padding:1em; border:1px solid #ccc; border-radius:8px; max-width:500px;">
                    <h3>Simulation Overview</h3>
                    <p>
                    Each square represents a digital organism within a 10x10 environment grid. Organisms move and attempt computational tasks. Their color indicates the most recent task they completed successfully:
                    </p>
                    <ul>
                    <li><span style='color:gray;'>Gray</span>: No task completed</li>
                    <li><span style='color:blue;'>Blue</span>: NOT</li>
                    <li><span style='color:green;'>Green</span>: NAND</li>
                    <li><span style='color:red;'>Red</span>: AND</li>
                    <li><span style='color:orange;'>Orange</span>: OR_N</li>
                    <li><span style='color:purple;'>Purple</span>: OR</li>
                    <li><span style='color:pink;'>Pink</span>: AND_N</li>
                    <li><span style='color:cyan;'>Cyan</span>: NOR</li>
                    <li><span style='color:yellow;'>Yellow</span>: XOR</li>
                    <li><span style='color:magenta;'>Magenta</span>: EQU</li>
                    </ul>
                    <p>
                    You can adjust simulation settings and control execution using the sliders and buttons.
                    </p>
                </div>
                )";
    }

    /**
     * @brief Sets up the simulation configuration panel.
     *
     * This function initializes the configuration panel, adding controls to toggle the simulation
     * and adjust parameters like mutation rate and number of starting organisms.
     */
    void SetupConfig() {
        // Setup config panel and controls
        auto specs = emp::ArgManager::make_builtin_specs(&config);
        emp::ArgManager am(emp::web::GetUrlParams(), specs);
        am.UseCallbacks();
        if (am.HasUnused()) std::exit(EXIT_FAILURE);

        controls << GetToggleButton("Toggle");
        controls << GetStepButton("Step");

        emp::prefab::ConfigPanel config_panel(config);
        config_panel.SetRange("NUM_START", "1", "20");
        config_panel.SetRange("MUTATION_RATE", "0.01", "0.07");
        config_panel.ExcludeSetting("SEED");
        config_panel.ExcludeSetting("NUM_TASKS");
        

        settings.SetCSS("max-width", "500px");
        settings << config_panel;
    }

    /**
     * @brief Initializes the world and injects organisms.
     *
     * Sets up the world grid and populates it with the initial set of organisms based on the
     * configuration settings.
     */
    void SetupWorld() {
        world.SetPopStruct_Grid(num_w_boxes, num_h_boxes);
        world.Resize(num_h_boxes, num_w_boxes);

        for (int i = 0; i < config.NUM_START(); i++) {
            Organism* new_org = new Organism(&world);
            world.Inject(*new_org);
        }
    }

    /**
     * @brief Performs operations on each frame when the animation is active.
     *
     * This function is automatically called on each frame of the animation. It clears the canvas,
     * moves all organisms, updates the world, and redraws the organisms.
     */
    void DoFrame() override {
        canvas.Clear();
        MoveAllOrganisms();
        world.Update();
        DrawAllOrganisms();
    }
    
    /**
     * @brief Moves all organisms in the world.
     *
     * This function loops through the world and moves each organism that is occupied in its grid cell.
     */
    void MoveAllOrganisms() {
        for (size_t i = 0; i < world.GetSize(); ++i) {
            if (world.IsOccupied(i)) {
                world.MoveOrganism(i);
            }
        }
    }

    /**
     * @brief Draws all organisms in the world.
     *
     * This function draws all the organisms on the canvas based on their grid positions.
     * It loops through the grid and calls DrawOrganismAt for each position.
     */
    void DrawAllOrganisms() {
        int org_num = 0;
        for (int x = 0; x < num_w_boxes; x++) {
            for (int y = 0; y < num_h_boxes; y++) {
                DrawOrganismAt(org_num, x, y);
                org_num++;
            }
        }
    }

    /**
     * @brief Draws a single organism at the specified coordinates.
     *
     * This function draws a single organism at the given grid coordinates. The color is determined
     * by the last task completed by the organism.
     * 
     * @param index The index of the organism in the world.
     * @param x The x-coordinate of the organism in the grid.
     * @param y The y-coordinate of the organism in the grid.
     */
    void DrawOrganismAt(int index, int x, int y) {
        if (world.IsOccupied(index)) {
            auto& org = world.GetOrg(index);
            int task_id = org.cpu.state.last_task_completed;

            std::string color = "gray"; // default if no task completed
            if (task_id == 0) color = "blue";
            else if (task_id == 1) color = "green";
            else if (task_id == 2) color = "red";
            else if (task_id == 3) color = "orange";
            else if (task_id == 4) color = "purple";
            else if (task_id == 5) color = "pink";
            else if (task_id == 6) color = "cyan";
            else if (task_id == 7) color = "yellow";
            else if (task_id == 8) color = "magenta";

            canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, color, "black");
        } else {
            canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "white", "black");
        }
    }
};

AEAnimator animator;

/**
 * @brief Main function to start the animation.
 *
 * Initializes an AEAnimator instance and starts the simulation by calling Step.
 */
int main() { animator.Step(); }
