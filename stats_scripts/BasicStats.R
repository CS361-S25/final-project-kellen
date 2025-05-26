require(ggplot2)
require(scales)

# Color-blind friendly palette
colors <- c(
  "#8B0000", "#B22222", "#DC143C", "#FF0000", "#FF3030", # Deep to bright reds
  "#FF4500", "#FF6347", "#FF7F50", "#FF8C00", "#FFA500", # Orange hues
  "#FFB347", "#FFD700", "#FFE135", "#F0E68C", "#FFFF00", # Yellow range
  "#ADFF2F", "#9ACD32", "#7FFF00", "#7CFC00", "#32CD32", # Yellow-green to lime
  "#00FF00", "#00FA9A", "#00FF7F", "#3CB371", "#2E8B57", # Green to sea green
  "#20B2AA", "#40E0D0", "#48D1CC", "#00CED1", "#00BFFF", # Turquoise to blue
  "#1E90FF", "#4169E1", "#4682B4", "#5F9EA0", "#6495ED", # Mid-blues
  "#6A5ACD", "#7B68EE", "#8A2BE2", "#9370DB", "#BA55D3", # Blue-violet to orchid
  "#DA70D6", "#DDA0DD", "#EE82EE", "#FF00FF", "#FF69B4", # Magenta to pink
  "#FF1493", "#DB7093", "#C71585", "#D02090", "#800080"  # Deep pinks to purple
)



# Set your working directory
setwd("/Users/ceylan/Desktop/CS361_S25/final-project-kellen/datafolder")

# Read in your data
initial_data <- read.table("munged_basic.dat", header = TRUE)

# Add equ_prop and factor Treatment column
initial_data <- cbind(initial_data,
                      equ_prop = initial_data$EQU / initial_data$total_orgs,
                      Treatment = as.factor(initial_data$treatment))

# Subsets
early <- subset(initial_data, update < 20)
just_middle <- subset(initial_data, treatment == 0.5)

# Replace NaNs with 0s
zeroed <- initial_data
zeroed[is.na(zeroed)] <- 0

# Line plot for equ_prop over time
ggplot(data = early, aes(x = update, y = equ_prop, group = Treatment, colour = Treatment)) +
  ylab("EQU Proportion") +
  xlab("Updates") +
  stat_summary(aes(color = Treatment, fill = Treatment), fun.data = "mean_cl_boot", geom = c("smooth"), se = TRUE) +
  theme(panel.background = element_rect(fill = 'white', colour = 'black')) +
  theme(panel.grid.major = element_blank(), panel.grid.minor = element_blank()) +
  guides(fill = FALSE) +
  scale_colour_manual(values = colors) +
  scale_fill_manual(values = colors) +
  xlim(0, 20)

# Data at final update
update_1 <- subset(initial_data, update == 999)

# Bar chart of EQU per rep
ggplot(data = update_1, aes(x = factor(rep), y = factor(equ_prop), fill = factor(rep))) +
  geom_col() +
  scale_fill_manual(values = colors) +
  labs(x = "Seed", y = "Final Update EQU Proportion", fill = "Seed") +
  theme(axis.text.x = element_text(angle = 45, hjust = 1)) +
  theme_minimal() +
  theme(legend.position = "none") 
  

# Boxplot of EQU proportions across replicates
ggplot(data = update_1, aes(x = Treatment, y = equ_prop, fill = Treatment)) +
  geom_boxplot() +
  scale_fill_manual(values = colors) +
  labs(x = "Treatment", y = "EQU Proportion at Update 999") +
  theme_minimal()

# Wilcoxon test for EQU proportion instead of coop
wilcox.test(
  subset(update_1, treatment == 1.0)$equ_prop,
  subset(update_1, treatment == 0.5)$equ_prop
)
