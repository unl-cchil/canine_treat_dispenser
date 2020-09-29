# Preamble ----------------------------------------------------------------
## ---
##
## Script name: arce_stevens_2020_rcode.R
##
## Purpose of script: This script analyzes the data investigating the reliability of a computer-controlled treat dispenser for canine operant conditioning.
##
## Author: Jeffrey R. Stevens (jeffrey.r.stevens@gmail.com)
##
## Date Created: 2020-07-22
##
## Date Finalized: 2020-09-29
##
## License: All materials presented here are released under the Creative Commons Attribution 4.0 International Public License (CC BY 4.0).
##  You are free to:
##   Share — copy and redistribute the material in any medium or format
##   Adapt — remix, transform, and build upon the material for any purpose, even commercially.
##  Under the following terms:
##   Attribution — You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
##   No additional restrictions — You may not apply legal terms or technological measures that legally restrict others from doing anything the license permits.
##
## ---
##
## Notes:
## Instructions: Place this file and the data files in the main directory.
##  At the R command prompt, type
## 	> source("arce_stevens_2020_rcode.R")
## 	This will run the script, adding all of the calculated variables to the workspace. 
##  If packages do not load properly, install them with install.packages("package_name").
##
## Data files:
## ---
## arce_stevens_2020_data.csv
##  dispenser - dispenser number (1 or 2)
##  trial - trial number
##  expected - expected number of treats dispensed
##  actual - actual number of treats dispensed
##
## ---


# Load library and input data ------------------------------------------------------------

library(tidyverse)  # load tidyverse for data input and processing

dispense_data <- read_csv("arce_stevens_2020_data.csv") %>% 
  mutate(diff = actual - expected,  # calculate difference between actual and expected number of treats dispensed
         error = ifelse(diff == 0, 0 , 1))  # flag any difference greater than 0 as an error


# Analyze data --------------------------------------------------------------

# Calculate mean number dispensed and mean number of errors for each dispenser and expected number of treats
dispenser_increment_mean <- dispense_data %>% 
  group_by(dispenser, expected) %>% 
  summarise(mean_dispensed = mean(actual),
    mean_error = sum(error))

# Create table of expected and actual treats dispensed and number of errors for each dispenser and overall
table_means <- pivot_wider(dispenser_increment_mean, id_cols = expected, names_from = dispenser, values_from = c("mean_dispensed", "mean_error"))
names(table_means) <- c("Expected Treats", "Actual One", "Actual Two", "Errors One", "Errors Two")
table_means <- mutate(table_means, "Mean Actual" = (`Actual One` + `Actual Two`) / 2,
         "Mean Error" = (`Errors One` + `Errors Two`) / 2)

# Calculate mean error rates for each dispenser and overall
overall_means <- table_means %>% 
  summarise(error1 = mean(`Errors One`) / 10 * 100,
         error2 = mean(`Errors Two`) / 10 * 100,
         error_overall = mean(`Mean Error`) / 10 * 100)
