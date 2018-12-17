# Passive Time Sync
Written by: David Hanley

This passive time synchronization package attempts to sychronize sensor measurements to some master clock. The solution is "passive" because the synchronization can be performed without any cooperation from the sensor. Therefore, this approach is different from approaches like [PTP](http://dx.doi.org/10.1109/IEEESTD.2008.4579760) and [NTP](http://dx.doi.org/10.1109/26.103043). This can be run online or on data offline. The offline approach produces better results (though based on a given application, it may not be possible). The principle of operation here is that major time latencies between the sensor and the master clock vary with time. Therefore, if we look for the smallest latency, we can better synchronize the clock. Clock drift is also accounted for. Note that a sensor does not need a clock in order to use this synchronization procedure. See the docs directory for descriptions on how to use this package.

This time synchronization approach is based on:
E. Olson. "A Passive Solution to the Sensor Synchronization Problem." IEEE/RSJ International Conference on Intelligent Robots and Systems. Taipei, Taiwan, Oct., 2010. [doi](http://dx.doi.org/10.1109/IROS.2010.5650579)



