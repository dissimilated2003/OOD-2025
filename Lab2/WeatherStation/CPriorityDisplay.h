#pragma once

#include "WeatherData.h"
#include "Observer.h"

// этот класс просто для отладки, для следующей функции
class CPriorityDisplay : public IObserver<WeatherInfo>
{
public:
	CPriorityDisplay(const std::string& name, int priority) : m_name(name), m_priority(priority) {}

	int GetPriority() { return m_priority; }

private:
	void Update(const WeatherInfo& data, IObservable<WeatherInfo>& subject) override
	{
		std::cout << "[" << m_priority << "] " << m_name << " - temp: " << data.temperature << "\n";
	}

	std::string m_name{};
	int m_priority{};
};

// эта функция показывает, что оповещение наблюдателей происходит в порядке приоритета
// для вызова в main
void RunTestingPriorities()
{
	WeatherData wd{};

	CPriorityDisplay lowPriority{ "LOW", 10 };
	CPriorityDisplay highPriority{ "HIGH", 100 };
	CPriorityDisplay midPriority{ "MIDDLE", 50 };
	CPriorityDisplay lowestPriority{ "LOWEST", 1 };

	wd.RegisterObserver(lowestPriority, lowestPriority.GetPriority());
	wd.RegisterObserver(highPriority, highPriority.GetPriority());
	wd.RegisterObserver(lowPriority, lowPriority.GetPriority());
	wd.RegisterObserver(midPriority, midPriority.GetPriority());

	std::cout << "=== testing priorities ===\n";
	wd.SetMeasurements(23, 68, 761);
}