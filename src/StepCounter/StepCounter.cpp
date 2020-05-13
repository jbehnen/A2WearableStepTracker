#include "StepCounter.h"

// Adapted from class resource StepTracker-Exercises.ipynb

StepCounter :: StepCounter(int minDistanceBetweenPeaksMs, 
                           int minPeakHeight, 
                           int bufferWindowSize,
                           int smoothingWindowSize) :
	_minDistanceBetweenPeaksMs(minDistanceBetweenPeaksMs), 
  _minPeakHeight(minPeakHeight),
  _bufferWindowSize(bufferWindowSize),
  _smoothingWindowSize(smoothingWindowSize)  {
  _steps = -1; // The algorithm likes to add a step at the start. 
  _initialValueCount = 0;  
  _lastPeakTimestamp = -1;
  _bufferedAccelerationIndex = 0;
  _totalBufferMagnitude = 0;
  _bufferedAccelerationMags = std::vector<int>(bufferWindowSize, 0);
  _bufferedAccelerationTimestamps =  std::vector<long>(bufferWindowSize, 0);
}

void StepCounter :: update(int accelerationMag, long timestamp) {
  _totalBufferMagnitude -= _bufferedAccelerationMags[_bufferedAccelerationIndex];
  _totalBufferMagnitude += accelerationMag;
  _bufferedAccelerationMags[_bufferedAccelerationIndex] = accelerationMag;
  _bufferedAccelerationTimestamps[_bufferedAccelerationIndex] = timestamp;
  
  // Detrend
  float averageMagnitude = ((float) _totalBufferMagnitude) / 
                           ((float) _bufferWindowSize);
  std::vector<float> detrendedAccerlationMags(_bufferWindowSize, 0);
  for (int i = 0; i < _bufferWindowSize; i++) {
    detrendedAccerlationMags[i] = _bufferedAccelerationMags[i] - averageMagnitude;
  }
   
  // Smooth
  std::vector<float> smoothedAccerlationMags(_bufferWindowSize, 0);
  for (int i = _bufferedAccelerationIndex;
       i < _bufferedAccelerationIndex + _bufferWindowSize - _smoothingWindowSize; 
       i++) {
    int index = wrapIndex(i);
    int smoothingTotal = 0;
    for (int j = index; j < index + _smoothingWindowSize; j++) {
      int jIndex = wrapIndex(j);
      smoothingTotal += detrendedAccerlationMags[jIndex];
    }
    smoothedAccerlationMags[index] = 
      ((float) smoothingTotal) / 
      ((float) _smoothingWindowSize);
  }

  // Detect peaks
  for (int i = _bufferedAccelerationIndex + 2;
       i < _bufferedAccelerationIndex + _bufferWindowSize; 
       i++) {
    int index = wrapIndex(i);
    int previousIndex = wrapIndex(i - 1);
    int nextIndex = wrapIndex(i + 1);
    int value = smoothedAccerlationMags[index];
    int previousValue = smoothedAccerlationMags[previousIndex];
    int nextValue = smoothedAccerlationMags[nextIndex];
    
    int forwardSlope = nextValue - value;
    int backwardSlope = value - previousValue;
    
    if (forwardSlope < 0 && backwardSlope > 0) {
      long peakVal = smoothedAccerlationMags[index];
      if (peakVal > _minPeakHeight) {
        long timeSinceLastDetectionMs = 
          _bufferedAccelerationTimestamps[index] - _lastPeakTimestamp;
        if (_lastPeakTimestamp == -1 || 
            timeSinceLastDetectionMs > _minDistanceBetweenPeaksMs) {
            _lastPeakTimestamp = _bufferedAccelerationTimestamps[index];
            _steps++;
        }
      }
    }
  }
  
  _bufferedAccelerationIndex = wrapIndex(_bufferedAccelerationIndex + 1);
}

int StepCounter :: getSteps() {
  return _steps;
}

// Private

int StepCounter :: wrapIndex(int i) {
  return i % _bufferWindowSize;
}