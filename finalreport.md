## Summary of the Methodology of the Original Paper

The authors used *Avida*, a digital evolution platform, to study how organisms evolve the ability to perform logical tasks over time. They tracked genomic changes across generations to understand how organisms acquired the capacity to solve increasingly complex logic problems.

In one experimental condition, organisms were rewarded for completing specific logic tasks with varying point values:

| Task                              | Points |
|-----------------------------------|--------|
| `NOT`                             | 2      |
| `NAND`                            | 2      |
| `AND`                             | 4      |
| `OR_N`                            | 4      |
| `OR`                              | 8      |
| `AND_N`                           | 8      |
| `NOR`                             | 16     |
| `XOR`                             | 16     |
| `EQU`                             | 32     |

They also tested two other experimental groups:

1. A group where only the most complex function (`EQU`) was rewarded. No organisms in this group evolved the ability to perform EQU.
2. A group where two simpler functions were deliberately not rewarded (though the paper does not specify which). I plan to remove rewards for the two most frequently performed tasks in the first group (excluding `EQU`).

---

## Summary of the Results from the Original Paper That You Attempted to Replicate

I attempted to replicate organisms with the ability to mutate and ran experiments under similar conditions to those used in the original study.

I ran experiments using seeds 1 through 5, each starting with 1–10 organisms. I’m planning to run experiments for the same three test cases used in the original study.

---

## Description of Your Reimplementation and Any Changes You Made

My implementation uses a point-based reward system instead of CPU cycles. In this system, organisms require 20 points to reproduce.

---

## Description of Your Replication Results So Far

Nothing conclusive yet.

Replicated the "well-distributed" point trial. All starting from 9 organisms, 0.01 mutation rate, and seeds ranging 1-20 for each trial.

---

## (If Relevant) Initial Analysis on Why Your Results Differ from the Original Results

*Pending further experimentation and analysis.*
