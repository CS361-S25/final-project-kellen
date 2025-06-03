## Experiment Overview


This experiment used a **EQU-only distribution** outlined in the study for the following logical operations:

**NOT, NAND, AND, OR_N, OR, AND_N, NOR, XOR, EQU**

with corresponding point values:

`0, 0, 0, 0, 0, 0, 0, 0, 32`

### Configuration Details
- Each trial **started with 10 organisms**  
  _(Note: This should eventually be configurable through the experiment setup)_
- **Mutation rate:** `0.01`
- The **number** assigned to each trial corresponds to the **seed value** used for generating that trial’s results.

### Contents of This Folder
This directory includes all relevant files for the experiment:
-  Raw **output files** from each individual trial  
-  A **munged data file** created by processing and combining the trial outputs  
-  A **graph** generated from the munged data using **RStudio**
