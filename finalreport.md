## Summary of the Methodology of the Original Paper

[Lenski et al. (2003)](https://adamilab.msu.edu/wp-content/uploads/Reprints/2003/Lenskietal2003.pdf) used *Avida*, a digital evolution platform, to study how digital organisms evolve the ability to perform logical tasks over time. They tracked genomic changes across generations to observe how simple traits could scaffold the evolution of more complex ones.

---

## Summary of Results from the Original Paper You Attempted to Replicate

Populations evolved in environments that reward **multiple** logic tasks were more likely to develop **complex behaviors** (like solving EQU) compared to those in environments that reward fewer or no tasks.

### Experimental Groups

#### 1. Exponential-Distribution
Organisms were rewarded for completing logic tasks with **exponentially increasing** point values. The original paper does not include results for this group, but we would expect it to produce the **highest proportion of EQU-performing organisms**.

##### Point Table:
| Task   | Points |
|--------|--------|
| NOT    | 2      |
| NAND   | 2      |
| AND    | 4      |
| OR_N   | 4      |
| OR     | 8      |
| AND_N  | 8      |
| NOR    | 16     |
| XOR    | 16     |
| EQU    | 32     |

---

#### 2. Only-EQU
Only the most complex task, `EQU`, was rewarded. In the original study, **no organisms evolved the ability to perform EQU** under this condition.

##### Point Table:
| Task   | Points |
|--------|--------|
| NOT    | 0      |
| NAND   | 0      |
| AND    | 0      |
| OR_N   | 0      |
| OR     | 0      |
| AND_N  | 0      |
| NOR    | 0      |
| XOR    | 0      |
| EQU    | 32     |

---

#### 3. Minus-Two
Two simpler functions were deliberately not rewarded (the paper does not specify which). I chose to remove rewards for `XOR` and `NOR`, the two most frequently performed tasks in the **Exponential-Distribution** group (excluding `EQU`). In all 10 environments tested in the original study, **at least one organism evolved EQU**.

##### Point Table:
| Task   | Points |
|--------|--------|
| NOT    | 2      |
| NAND   | 2      |
| AND    | 4      |
| OR_N   | 4      |
| OR     | 8      |
| AND_N  | 8      |
| NOR    | 0      |
| XOR    | 0      |
| EQU    | 32     |

---

## Description of Your Reimplementation and Any Changes You Made

In my replication, I rewarded organisms with **points** instead of CPU cycles. I made additional changes, such as reducing the number of:
- Starting organisms  
- Trials  
- Updates per trial  
- World size

I set the reproduction threshold to **20 points** — not based on the original paper, but because it felt appropriate relative to point rewards.

My measurement approach also differs:
- I only track how many organisms **performed EQU in the final update** (i.e., update 1000).
- This means an organism must actively perform EQU on the last update to be counted, regardless of whether it could have done so earlier.

> I’m still unsure whether this is a faithful replication metric. Another option might be to use the **maximum number of EQU-performing organisms** at any point in the trial.

**Current experiment details:**
- 50 trials per experimental group  
- 10 organisms at start  
- Mutation rate: 0.01  
- 1000 updates per trial  
- World size: 100  
- Each `.data` file number = random seed used

All relevant data — raw outputs, munged data, and `.pdf` graphs — are stored in the `/datafolder`.

---

## Description of Replication Results So Far

I haven’t done formal statistical analysis yet, so here are visual interpretations from the graphs:

### Exponential-Distribution
- **Average:** 2.5% – 5% of organisms performed EQU in the final update
- **Min/Max:** 0% – 12.5%
- **Zero Trials:** 7 out of 50 trials resulted in 0% EQU in the final update

### Only-EQU
- **Average:** Roughly 25% – 35%
- **Min/Max:** 0% – ~66.6%
- **Zero Trials:** 8 out of 50

### Minus-Two
- **Average:** Around 35%
- **Min/Max:** ~20% – 47.5%
- **Zero Trials:** 0 out of 50

---

## Initial Thoughts on Differences from the Original Results

*Further analysis is still pending, but here are some initial observations:*

- **Seeds 2 and 9** were the only cases where both the **Exponential-Distribution** and **Only-EQU** groups had **0% EQU performance** at the final update. This might suggest either:
  - EQU was **bred out** during evolution in these trials, or
  - EQU **never evolved** in the first place under those conditions.

- The **Exponential-Distribution** group was expected to produce the **most EQU-performing organisms**, given the steep rewards, but it actually produced the **fewest**.

- The **Only-EQU** group performed **better than expected** in my trials. While the original study reported **no organisms evolved EQU**, my results show this group had the **highest average percentage** of EQU-performing organisms at the final update. However, it also showed **high variability**, with several trials resulting in **no EQU evolution** at all.

- The **Minus-Two** group gave the **most consistent results** with the original paper. The original study noted that in all trials, **at least one organism** evolved to perform EQU—and I found **the same**. In my trials, **every single one** had EQU-performing organisms at the final update.

---

More trials and deeper comparisons (especially mid-run behavior and task acquisition rates) will be necessary to confirm these patterns and understand why my results diverge from the original study.
