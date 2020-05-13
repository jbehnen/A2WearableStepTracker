#ifndef STEP_COUNTER_H
#define STEP_COUNTER_H

#include <vector>

/**
* Counts steps based on acceleration magnitudes and the
* timestamps associated with each one.
*   
* By Julia Behnen
* 
* Adapted from class resource StepTracker-Exercises.ipynb
*/ 
class StepCounter {
  private:
    const int _minDistanceBetweenPeaksMs;
	  const int _minPeakHeight;
    const int _bufferWindowSize;
    const int _smoothingWindowSize; 
  
    int _steps;
    int _initialValueCount;
    long _lastPeakTimestamp;
    int _bufferedAccelerationIndex;
    int _totalBufferMagnitude;
    std::vector<int> _bufferedAccelerationMags;
    std::vector<long> _bufferedAccelerationTimestamps;
    
    int wrapIndex(int i);
    
  public:
    // Constructor
    StepCounter(
      const int minDistanceBetweenPeaksMs, 
      const int minPeakHeight, 
      const int bufferWindowSize,
      const int smoothingWindowSize);
	
    /*
     * Updates the state of the class based on the acceleration
     * and timestamp. 
     */
    void update(int accelerationMag, long timestamp);
  
    /*
     * Returns the number of total steps taken based on the most
     * recent call to update();
     */
    int getSteps();
};

#endif