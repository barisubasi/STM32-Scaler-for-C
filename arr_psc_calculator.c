/*
===============================================================================
File: arr_psc_calculator.c
Author: Barış Subaşi
Date: 05.12.2023
Description: This C program calculates the prescaler (PSC) and Auto-Reload-
             Register (ARR) values for a given target frequency using a while
             loop and a specified error margin. The program is designed for
             use with STM32 microcontrollers using the TIM peripheral.

             The main function, calc_arr_psc(), iterates through possible
             PSC and ARR values to find the combination that results in a
             frequency close to the target frequency, within a defined error
             margin.

Compilation: This code requires a C compiler. It has been tested with GCC.

Usage: Compile and run the program. The calculated ARR and PSC values, along
       with other relevant information, will be printed to the console.

===============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TIM_BASE_CLOCK 84000000   // Your base clock max frequency
#define TARGET_F 168              // Target frequency (changeable)
#define DUTY 50.0                 // DUTY value
#define TARGET_ARR_MAX 65535      // Max ARR value for 16 bit
#define TARGET_PSC_MAX 65535      // Max PSC value for 16 bit

/*
-------------------------------------------------------------------------------
Function: calc_arr_psc
Description: This function searches for the optimal PSC and ARR values to
             achieve a target frequency with a specified error margin.

Parameters: None
Returns:    None
-------------------------------------------------------------------------------
*/
void calc_arr_psc() {

    float min = INT_MAX;  // Minimum value for deviation. Set from max integer value.
    int psc_u, arr_u;     // Wanted Prescaler and Auto-Reload-Register values.
    float freq;           // Calculated frequency value.
    int j = 0;            // A value for if statements set to 0.
    double error = 0.01;  // Error margin percentage value.
    double error_increase_rate = 0.002;  // Error increase rate for the while loop.

    // Main while loop for calculation. Continues until it finds the Prescaler and ARR values.
    while (1) {
        for (int psc = 0; psc <= TARGET_PSC_MAX; ++psc) {
            float x = (float)(TIM_BASE_CLOCK) / ((float)TARGET_F * (psc + 1)) - 1;
            // X value is for calculating the ARR value for the following if statement.
            if (TIM_BASE_CLOCK % (psc + 1) == 0 && x <= TARGET_ARR_MAX) {
                for (int arr = 0; arr <= TARGET_ARR_MAX; ++arr) {
                    // Left value is a multiplication for calculating error percentage and frequency values.
                    int left = (arr + 1) * (psc + 1);

                    // Pererror value is the percentage error between targeted frequency and calculated frequency.
                    float pererror = fabs(1 - ((float)TARGET_F / ((float)TIM_BASE_CLOCK / left))) * 100;

                    // If calculated "pererror" value is lesser than the set error value statement.
                    if (pererror <= error) {

                        float current_freq = (float)TIM_BASE_CLOCK / left;

                        // Delta value is the absolute difference between the target frequency and calculated frequency.
                        float delta = fabs((float)TARGET_F - current_freq);

                        // If delta value is lesser than min value statement.
                        if (delta < min) {
                            min = delta;          // Min value updates to calculated delta value.
                            psc_u = psc;          // Wanted PSC value.
                            arr_u = arr;          // Wanted ARR value.
                            freq = current_freq;  // Calculated frequency value.
                            j++;                  // J value increased by 1 for if statement.
                        }
                    }
                }
            }
        }
        // If "j" value increased, prints the values and breaks the while loop.
        if (j > 0) {
            printf("arr: %d\n", arr_u);
            printf("psc: %d\n", psc_u);
            printf("wanted freq:%dHz\n", TARGET_F);
            printf("freq: %fHz\n", freq);
            printf("Delta: %fHz\n", ((float)TARGET_F - freq));
            break;
        }
        // If "j" value doesn't increase, inform the user and increase the error margin percentage.
        else {
            printf("No results. The margin of error is increased.\nError margin= %c%f\n\n", 37, error);
            error = error + error_increase_rate;
        }
    }
}

/*
-------------------------------------------------------------------------------
Function: main
Description: The main function of the program. Calls the search_arr_psc
             function and returns 0 upon completion.

Parameters: None
Returns:    0
-------------------------------------------------------------------------------
*/
int main() {
    calc_arr_psc();
    return 0;
}
