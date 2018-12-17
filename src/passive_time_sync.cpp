//
//	Passive time synchronization attempts to synchronize sensor measurements to a 
//  master clock using the fact that latency tends to vary and sensor clocks tend 
//  to drift relative to the master clock. 
//
//  David Hanley
//
//  passive_time_sync.cpp 
//
//  Passive time synchronization implementation file for the April Lab's 
//  passive time synchronization approach. This approach is described in:
//  E. Olson. "A Passive Solution to the Sensor Synchronization Problem." 
//  IEEE/RSJ International Conference on Intelligent Robots and Systems (IROS). 
//  Taipei, Taiwan, Oct, 2010. 
//
//  Usage:
//  
//

/*-----------------------------------------------------------------------------*/
/*-------------------------------- Preamble -----------------------------------*/
/*-----------------------------------------------------------------------------*/
/*----------------- Defines --------------------*/
/*--------------- End Defines ------------------*/

/*------------------ Includes ------------------*/
#include <iostream>
#include <vector>
#include <stdlib.h> 
#include "passive_time_sync.h"
/*---------------- End Includes ----------------*/

/*---------------- Globals ---------------------*/

/*-------------- End Globals -------------------*/

/*------------------ Classes -------------------*/
/*---------------- End Classes -----------------*/

/*----------------- Namespaces -----------------*/
/*--------------- End Namespaces ---------------*/

/*------------------ Pragmas -------------------*/
/*---------------- End Pragmas -----------------*/

/*------------- Function Prototypes ------------*/
/*----------- End Function Prototypes ----------*/
/*-----------------------------------------------------------------------------*/
/*------------------------------ End Preamble ---------------------------------*/
/*-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------*/
/*--------------------------------- Functions ---------------------------------*/
/*-----------------------------------------------------------------------------*/
// Default constructor initializes all time syncronization style
passive_time_sync::passive_time_sync()
{
    // Default case assumes that the sensor clock could be up to 5 percent slower or 
    // faster than the master clock.
    alpha_1 = 0.05;
    alpha_2 = 0.05;

    // Set bound of clock drift when time difference between measurements 
    // is zero, to zero.
    f0 = 0.0;

    // Start Online Syncing
    start_online = true;
}

// Parameterized constructor to create time syncronization object
// Inputs:   alpha_1_input = The extent to which the sensor's clock may be slow
//           alpha_2_input = The extent to which the sensor's clock may be fast
//           f0_input = Set bound of clock drift when time difference between 
//                      measurements is zero.
// Outputs: Assigns Class's private parameters
passive_time_sync::passive_time_sync(double alpha_1_input, double alpha_2_input, double f0_input)
{
    // Set assumptions on how fast or slow the sensor clock may be relative to 
    // the master clock.
    alpha_1 = alpha_1_input;
    alpha_2 = alpha_2_input;

    // Set bound of clock drift when time difference between measurements is zero.
    f0 = f0_input;

    // Start Online Syncing
    start_online = true;
}

// Default destructor eliminates all object parameters
passive_time_sync::~passive_time_sync(void)
{

}

// Get Batch Sync uses a batch of sensor time stamps and time stamps from a 
// master clock and produces a batch of synchronized sensor time stamps.
// Inputs: sens_time_stamps = a vector of sensor time stamps
//         master_time_stamps = a vector of master clock time stamps
// Outputs: corrected sensor time measurements in terms of the master clock time.
vector<double> passive_time_sync::get_batch_sync(vector<double> sens_time_stamps, vector<double> master_time_stamps)
{
    // Initialize the relevant variables
    int N;
    N = sens_time_stamps.size();
    vector<double> synced_time(N);
    vector<double> A_offset(N);
    double p = sens_time_stamps.at(0);
    double q = master_time_stamps.at(0);

    // The forward pass
    for (int i = 0; i < N; i++)
    {
        // Check for the shortest latency time (accounting for clock drift) 
        if (sens_time_stamps.at(i) - master_time_stamps.at(i) - f0 >= p - q - clock_func(abs(sens_time_stamps.at(i) - p)))
        {
            p = sens_time_stamps.at(i);
            q = master_time_stamps.at(i);
            A_offset.at(i) = p - q - f0;
        }
        else
        {
            A_offset.at(i) = p - q - clock_func(abs(sens_time_stamps.at(i) - p));
        }
        // Fix the time offset
        synced_time.at(i) = sens_time_stamps.at(i) - A_offset.at(i);
    }

    // The backward pass
    for (int i = N-1; i >= 0; i--)
    {
        // Check for the shortest latency time (accounting for clock drift)
        if (sens_time_stamps.at(i) - master_time_stamps.at(i) - f0 >= p - q - clock_func(abs(sens_time_stamps.at(i) - p)))
        {
            p = sens_time_stamps.at(i);
            q = master_time_stamps.at(i);
            A_offset.at(i) = max(A_offset.at(i), p - q - f0);           
        }
        else
        {
            A_offset.at(i) = max(A_offset.at(i), p - q - clock_func(abs(sens_time_stamps.at(i) - p)));
        }
        // Fix the time offset
        synced_time.at(i) = sens_time_stamps.at(i) - A_offset.at(i);
    }

    // Return result
    return synced_time;
}

// Get Online Sync is called every time the master computer receives a sensor 
// measurement. Sensor measurement time stamps are synced with the master clock.
// Inputs: sens_time_stamps = a sensor time stamp
//         master_time_stamps = a master clock time stamp
// Outputs: corrected sensor time stamp
double passive_time_sync::online_sync(double sens_time_stamps, double master_time_stamps)
{
    // Initialize the relevant variables 
    double synced_time;
    double A_offset;

    // On the first call of the online sync, assign two variables some initial values
    if (start_online == true)
    {
        p_online = sens_time_stamps;
        q_online = master_time_stamps;
        start_online = false;
    }

    // Check for the shortest latency time (accounting for clock drift) 
    if (sens_time_stamps - master_time_stamps - f0 >= p_online - q_online - clock_func(abs(sens_time_stamps - p_online)))
    {
        p_online = sens_time_stamps;
        q_online = master_time_stamps;
        A_offset = p_online - q_online - f0;
    }
    else
    {
        A_offset = p_online - q_online - clock_func(abs(sens_time_stamps - p_online));
    }

    // Fix the time offset
    synced_time = sens_time_stamps - A_offset;

    // Return Result
    return synced_time;
}

// This is the drift model for the clock
// Input:  dp = change in time between sensor time stamps
// Output: Upper bound of the change in offset 
double passive_time_sync::clock_func(double dp)
{
    // Initialize output
    double output;
    
    // Choose the drift model for the clock
    if (alpha_1 >= alpha_2)
    {
        output = dp*alpha_1 + f0;
    }
    else
    {
        output = dp*alpha_2 + f0; 
    }

    return output;
}
/*-----------------------------------------------------------------------------*/
/*------------------------------- End Functions -------------------------------*/
/*-----------------------------------------------------------------------------*/ 