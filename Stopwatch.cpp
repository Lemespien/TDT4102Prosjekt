#include "Stopwatch.h"

void Stopwatch::start() {
    if (isRunning) return;
    startTime = std::chrono::steady_clock::now();
    isRunning = true;
}

double Stopwatch::stop() {
    if (!isRunning) return 1;
    std::chrono::time_point endTime = std::chrono::steady_clock::now();

    long durationInMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
    double durationInSeconds = double(durationInMicroseconds)/1000000.0;
    isRunning = false;
    return durationInSeconds;
}