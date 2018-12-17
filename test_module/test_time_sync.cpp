//
//	Passive time synchronization attempts to synchronize sensor measurements to a 
//  master clock using the fact that latency tends to vary and sensor clocks tend 
//  to drift relative to the master clock. 
//
//  David Hanley
//
//  test_time_sync.cpp 
//
//  Test time synchronization implementation file for the April Lab's 
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
#include <boost/random/normal_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>
#include "passive_time_sync/passive_time_sync.h"
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
int main()
{
    // Initialize variables
    int N = 1000;
    vector<double> master_clock(N);
    vector<double> sensor_clock(N);
    vector<double> true_time(N);
    double A = 10.0;
    double dt = 0.01;
    double average = 0.0;
    double online_average = 0.0;
    double online_output;

    // This is the underlying integer random number generator
    boost::mt19937 igen;
    // The second template parameter is the actual floating point
    // distribution that the user wants
    boost::variate_generator<boost::mt19937, boost::normal_distribution<> >gen(igen,boost::normal_distribution<>(0.0,0.001));
    boost::variate_generator<boost::mt19937, boost::normal_distribution<> >gen2(igen,boost::normal_distribution<>(0.0,1.0));

    // Iterate through time to generate all time stamps
    for (int i = 0; i < N; i++)
    {
        if (i == 0)
        {
            true_time[i] = 0.0;
        }
        else
        {
            true_time[i] = true_time[i-1] + dt;
        }
        // Add latency
        master_clock[i] = true_time[i] + abs(gen2());
        // Add offset
        sensor_clock[i] = true_time[i] + A; 
        // Add clock drift
        A = A + gen();
    }

    // Create a clock synchronization object
    passive_time_sync test1(0.05,0.05,0.0);

    // Batch synchronization
    vector<double> output;
    output = test1.get_batch_sync(sensor_clock, master_clock);

    // perform online synchronization, compute average error, and compute 
    // batch error average 
    for (int i = 0; i < N; i++)
    {
        online_output = test1.online_sync(sensor_clock[i], master_clock[i]);
        online_average = (true_time[i] - online_output) + online_average;
        average = (true_time[i] - output[i]) + average;
    }
    average = average/((double)true_time.size());
    online_average = online_average/((double)true_time.size());

    // Output Results
    cout << "----------------------------------------" << endl;
    cout << "        Batch Average Sync Error        " << endl;
    cout << "----------------------------------------" << endl;
    cout << abs(average) << " seconds" << endl;
    cout << "----------------------------------------" << endl;
    cout << "      End Batch Average Sync Error      " << endl;
    cout << "----------------------------------------" << endl;
    cout << "----------------------------------------" << endl;
    cout << "        Online Average Sync Error       " << endl;
    cout << "----------------------------------------" << endl;
    cout << abs(online_average) << " seconds" << endl;
    cout << "----------------------------------------" << endl;
    cout << "      End Online Average Sync Error     " << endl;
    cout << "----------------------------------------" << endl;

    return 0;
}
/*-----------------------------------------------------------------------------*/
/*------------------------------- End Functions -------------------------------*/
/*-----------------------------------------------------------------------------*/ 