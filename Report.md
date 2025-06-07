# Replicating Lenski et al. (2003) with Empirical and a New Logical Task

## Summary of the Methodology of the Original Paper

[Lenski et al. (2003)](https://adamilab.msu.edu/wp-content/uploads/Reprints/2003/Lenskietal2003.pdf) used *Avida*, a digital evolution platform, to study how digital organisms evolve the ability to perform logical tasks over time. They tracked genomic changes across generations to observe how simple traits could scaffold the evolution of more complex ones.

---

## Summary of Results from the Original Paper

Populations evolved in environments that rewarded **multiple** logic tasks were more likely to develop **complex behaviors** (like solving `EQU`) compared to those in environments that rewarded fewer or no tasks.

Although I'm using *Empirical* rather than *Avida*, I attempted to replicate these findings. My expansion includes the **COMPLEX (COM)** task, defined as `(A ^ B) || (C ^ D)`. I'm not entirely sure if COM is harder than EQU, but I suspect it might be due to the extra logical operations required.

I recreated each experimental group from the base study and added the COM task to each, to see if the additional task might change results—especially because my initial replication didn’t entirely match the original findings. I wondered if EQU might have been too easy for my organisms.

---

## Experimental Groups

### 1. Reward-All

Organisms were rewarded for completing logic tasks with **exponentially increasing** point values. The original paper didn’t include final results for this group, but it's generally expected to produce the **highest proportion of EQU-performing organisms**.

#### Point Table:

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

#### Reward-All Expansion

Same reward values, with the addition of the COM task.

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
| COM    | 64     |

---

### 2. Only-EQU

Only the most complex task, `EQU`, was rewarded. In the original study, **no organisms evolved the ability to perform EQU** under this condition.

#### Point Table:

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

#### Only-EQU Expansion

Same structure, but `COM` is rewarded instead of `EQU`.

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
| EQU    | 0      |
| COM    | 64     |

---

### 3. Minus-Two

Two simpler functions were deliberately not rewarded. The original paper doesn’t specify which, but I removed `XOR` and `NOR`, the two most frequently performed tasks in Reward-All (excluding EQU). In the original study, **at least one organism evolved EQU in all 10 environments**.

#### Point Table:

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

#### Minus-Two Expansion

Same structure with `COM` added.

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
| COM    | 64     |

---

## Reimplementation Details

In my replication, I made the following changes from the original paper:

- Used **points** instead of CPU cycles
- Reduced:
  - Starting organisms
  - Trials
  - Updates per trial
  - World size
- Set reproduction threshold to **20 points**
- Tracked only **task performance on the final update (1000)**

### Experiment Parameters:

- 50 trials per group  
- 10 starting organisms  
- Mutation rate: 0.01  
- 1000 updates per trial  
- World size: 100  
- `.data` file number = seed

Data (raw output, munged data, and graphs) are stored in `/datafolder`.

---

## Replication Results

### Reward-All
[Plot](./datafolder/reward-all/reward-all-Rplot.pdf)
- **Avg:** 2.5% – 5% of organisms performed EQU at final update
- **Range:** 0% – 12.5%
- **Zero Trials:** 7/50

### Reward-All Expansion
[Plot](./datafolder/reward-all-expanded/Rplot.png)
- **Avg:** 5% – 10% performed COM
- **Range:** 0% – 21%
- **Zero Trials:** 2/50

### Only-EQU
[Plot](./datafolder/only-equ/only-equ-Rplot.pdf)
- **Avg:** 25% – 35%
- **Range:** 0% – ~66.6%
- **Zero Trials:** 8/50

### Only-EQU Expansion
[Plot](./datafolder/only-equ-expanded/Rplot.png)
- **Avg:** 30% – 40% performed COM
- **Range:** 0% – 55%
- **Zero Trials:** 9/50

### Minus-Two
[Plot](./datafolder/minus-two/minus-two-Rplot.pdf)
- **Avg:** Around 30%
- **Range:** ~20% – 47.5%
- **Zero Trials:** 0/50

### Minus-Two Expansion
[Plot](./datafolder/minus-two-expanded/Rplot.png)
- **Avg:** Around 25%
- **Range:** 14% – 41%
- **Zero Trials:** 0/50

---

## Reflections on Differences from the Original Study

- **Seeds 2 and 9** had 0% EQU in both Reward-All and Only-EQU. This might mean EQU was either:
  - Never evolved, or
  - Bred out by the final update.

- **Reward-All** was expected to have the most EQU performers, but it actually had the fewest.

- **Only-EQU** performed better than the original study suggested. It showed high averages, but also high variability (with several trials failing to evolve EQU at all).

- **Minus-Two** most closely matched the original paper — in all trials, **at least one organism evolved EQU**.

---

## Additional Observations After Second Round of Results

- In most of my trials, at least one organism performed a task on the **first update** — not the case in the original study. This may have impacted results.

- I used the `Instructions.h` file from the DE lab. My professor noted this could differ from what was given in the original paper. It may have provided an advantage to my organisms, though I’m not sure how.

### Expansion Behavior

- **Minus-Two** was the most consistent again. Possibly, `XOR` and `NOR` outcompete other functions when included.

- **Only-EQU Expansion** had more stable populations performing COM, with fewer fluctuations compared to the original EQU condition.

- **Reward-All Expansion** showed **more organisms evolved COM than EQU**, which was surprising — I expected the opposite. This suggests COM might not be harder than EQU after all.

---

## Conclusion

My expansions aligned more with **my own replication results** than with the original study. I hoped that adding a harder task would make results more consistent with the original paper — but that didn’t happen.

Further analysis is needed to understand:
- Why some traits evolved inconsistently
- How task interactions affect final behavior
- Whether my implementation differences (e.g., instructions or reward structure) shifted outcomes significantly
