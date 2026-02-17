#include "Observer.h"
#include "WeatherData.h"
#include "CPriorityDisplay.h"

#include <iostream>

void TestRun()
{
    WeatherData indoorWD;
    WeatherData outdoorWD;

    Display display(indoorWD, outdoorWD);
    StatsDisplay stats(indoorWD, outdoorWD);

    indoorWD.RegisterObserver(display, 10);
    outdoorWD.RegisterObserver(display, 10);
    indoorWD.RegisterObserver(stats, 1);
    outdoorWD.RegisterObserver(stats, 1);

    indoorWD.SetMeasurements(23.0, 67.1, 761.2);
    outdoorWD.SetMeasurements(24.2, 68.4 , 762.1);
}


int main()
{
    RunTestingPriorities();
}
