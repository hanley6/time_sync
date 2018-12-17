//
//	Passive time synchronization attempts to synchronize sensor measurements to a 
//  master clock using the fact that latency tends to vary and sensor clocks tend 
//  to drift relative to the master clock. 
//
//  David Hanley
//
//  passive_time_sync.h
//
//  Passive time synchronization header file for the April Lab's passive
//  time synchronization approach. This approach is described in:
//  E. Olson. "A Passive Solution to the Sensor Synchronization Problem." 
//  IEEE/RSJ International Conference on Intelligent Robots and Systems (IROS). 
//  Taipei, Taiwan, Oct, 2010. 
//
//  Usage:
//  
//

/*---------------- Include Guard ---------------*/
#ifndef passive_time_sync_H
#define passive_time_sync_H
/*-------------- End Include Guard -------------*/

/*------------------ Includes ------------------*/

/*---------------- End Includes ----------------*/

/*----------------- Namespaces -----------------*/
using namespace std;
/*--------------- End Namespaces ---------------*/

/*------------------ Globals -------------------*/
/*---------------- End Globals -----------------*/

/*------------------- Classes ------------------*/
class passive_time_sync
{
    public:
        // Default constructor initializes all time syncronization style
        passive_time_sync();

        // Parameterized constructor to create time syncronization object
        // Inputs:   alpha_1_input = The extent to which the sensor's clock may be slow
        //           alpha_2_input = The extent to which the sensor's clock may be fast
        //           f0_input = Set bound of clock drift when time difference between 
        //                      measurements is zero.
        // Outputs: Assigns Class's private parameters
        passive_time_sync(double alpha_1_input, double alpha_2_input, double f0_input);

        // Default destructor eliminates all object parameter
        ~passive_time_sync();

        // Get Batch Sync uses a batch of sensor time stamps and time stamps from a 
        // master clock and produces a batch of synchronized sensor time stamps.
        // Inputs: sens_time_stamps = a vector of sensor time stamps
        //         master_time_stamps = a vector of master clock time stamps
        // Outputs: corrected sensor time measurements in terms of the master clock time.
        vector<double> get_batch_sync(vector<double> sens_time_stamps, vector<double> master_time_stamps);

        // Get Online Sync is called every time the master computer receives a sensor 
        // measurement. Sensor measurement time stamps are synced with the master clock.
        // Inputs: sens_time_stamps = a sensor time stamp
        //         master_time_stamps = a master clock time stamp
        // Outputs: corrected sensor time stamp
        double online_sync(double sens_time_stamps, double master_time_stamps);
    private:
        // Set of sensor measurement times
        double p_online; 

        // Set of master clock times at which 
        // sensor measurement times are received
        double q_online;

        // The extent to which the sensor's clock may be slow
        double alpha_1;

        // The extent to which the sensor's clock may be fast
        double alpha_2;

        // The initial clock drift bound when the time difference 
        // between measurements is zero.
        double f0;

        // A flag to identify if the online synchronization has begun
        bool start_online;

        // This is the drift model for the clock
        // Input:  dp = change in time between sensor time stamps
        // Output: Upper bound of the change in offset 
        double clock_func(double dp);
};
/*----------------- End Classes ----------------*/

/*---------------- Include Guard ---------------*/
#endif
/*-------------- End Include Guard -------------*/