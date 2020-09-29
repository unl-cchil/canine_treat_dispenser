ReadMe file
Created on 2020-09-29 by Jeffrey R. Stevens (jeffrey.r.stevens@gmail.com)

**********************************************************
If you use the data, please cite the following:
Arce, W. & Stevens, J.R. (2020). Developing a computer-controlled treat dispenser for canine operant conditioning. Journal of Open Hardware.
**********************************************************

Summary: We tested two treat dispensers 100 times each with 10 tests of increasing dispensing targets from 1 to 10 treats. Each row of the data file gives the expected and actual number of treats dispensed for a single trial.

License:
All materials presented here are released under the Creative Commons Attribution 4.0 International Public License (CC BY 4.0). You are free to:
    Share — copy and redistribute the material in any medium or format
    Adapt — remix, transform, and build upon the material for any purpose, even commercially.
Under the following terms:
    Attribution — You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
    No additional restrictions — You may not apply legal terms or technological measures that legally restrict others from doing anything the license permits.

Data files:
arce_stevens_2020_data.csv
 dispenser - dispenser number (1 or 2)
 trial - trial number
 expected - expected number of treats dispensed
 actual - actual number of treats dispensed

R code:
 arce_stevens_2020_rcode.R - code for running computations

Instructions to reproduce results:
 To reproduce these results, open arce_stevens_2020_rcode.R and ensure that all packages mentioned at the top of the script are installed.  Once all packages are installed, run the script in R using "source("arce_stevens_2020_rcode.R")".
 
