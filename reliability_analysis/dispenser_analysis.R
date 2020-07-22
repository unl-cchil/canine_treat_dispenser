library(tidyverse)

data <- read_csv("dispenser_data.csv") %>% 
  mutate(diff = actual - expected,
         error = ifelse(diff == 0, 0 , 1))

dispenser_increment_mean <- data %>% 
  group_by(dispenser, expected) %>% 
  summarise(mean_dispensed = mean(actual),
    mean_error = sum(error))#,
            # mean_deviation = mean(abs(diff)))
table_means <- bind_cols(filter(dispenser_increment_mean, dispenser == 1), filter(dispenser_increment_mean, dispenser == 2))
names(table_means) <- c("dispenser1", "Expected Treats", "Actual One", "Errors One", "dispenser2", "exp2", "Actual Two", "Errors Two")
table_means <- select(table_means, "Expected Treats", "Actual One", "Actual Two", "Errors One", "Errors Two") %>% 
  mutate("Mean Actual" = (`Actual One` + `Actual Two`) / 2,
         "Mean Error" = (`Errors One` + `Errors Two`) / 2)
mean(table_means$`Errors One`) / 10 * 100
mean(table_means$`Errors Two`) / 10 * 100
mean(table_means$`Mean Error`) / 10 * 100

dispenser_mean <- data %>% 
  group_by(dispenser) %>% 
  summarise(mean_error = mean(error),
            mean_deviation = mean(abs(diff)))

mean_error_overall = mean(data$error)
mean_deviatin_overall = mean(abs(data$diff))
