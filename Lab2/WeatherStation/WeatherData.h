#pragma once

#include "Observer.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
//C++ standard draft

struct WeatherInfo
{
	double temperature = 0;
	double humidity = 0;
	double pressure = 0;
};

class Display : public IObserver<WeatherInfo>
{
public:
	Display(
		IObservable<WeatherInfo>& in, 
		IObservable<WeatherInfo>& out) 
		: 
		m_indoorStation(in), 
		m_outdoorStation(out) 
	{}

private:
	void Update(WeatherInfo const& data, IObservable<WeatherInfo>& subject) override
	{
		std::string location = (&subject == &m_indoorStation) ? "Inside" : "Outside";
		std::cout << "[" << location << "] Current Temp " << data.temperature << std::endl;
		std::cout << "[" << location << "] Current Humidity " << data.humidity << std::endl;
		std::cout << "[" << location << "] Current Pressure " << data.pressure << std::endl;
		std::cout << "----------------" << std::endl;
	}

	IObservable<WeatherInfo>& m_indoorStation;
	IObservable<WeatherInfo>& m_outdoorStation;
};



class StatsDisplay : public IObserver<WeatherInfo>
{
public:
	StatsDisplay(
		IObservable<WeatherInfo>& in, 
		IObservable<WeatherInfo>& out)
		: 
		m_indoorStation(in), 
		m_outdoorStation(out) 
	{}

private:
	void DisplayStats()
	{
		std::cout << "\n=== Indoors Stats ===\n" << std::endl;
		m_tempStatsIn.Display("temperature");
		m_humidityStatsIn.Display("humidity");
		m_pressureStatsIn.Display("pressure");

		std::cout << "\n=== Outdoors Stats ===\n" << std::endl;
		m_tempStatsOut.Display("temperature");
		m_humidityStatsOut.Display("humidity");
		m_pressureStatsOut.Display("pressure");
	}

	// простите меня грешного
	void Update(WeatherInfo const& data, IObservable<WeatherInfo>& subject) override
	{
		if (&subject == &m_indoorStation)
		{
			m_tempStatsIn.Update(data.temperature);
			m_humidityStatsIn.Update(data.humidity);
			m_pressureStatsIn.Update(data.pressure);
		}
		else
		{
			m_tempStatsOut.Update(data.temperature);
			m_humidityStatsOut.Update(data.humidity);
			m_pressureStatsOut.Update(data.pressure);
		}

		DisplayStats();
	}

	Statistic<double> m_tempStatsIn{}, m_tempStatsOut{};
	Statistic<double> m_humidityStatsIn{}, m_humidityStatsOut{};
	Statistic<double> m_pressureStatsIn{}, m_pressureStatsOut{};
		
	IObservable<WeatherInfo>& m_indoorStation;
	IObservable<WeatherInfo>& m_outdoorStation;
};



class WeatherData : public CObservable<WeatherInfo>
{
public:
	double GetTemperature() const { return m_temperature; } // Температура в градусах Цельсия
	double GetHumidity() const { return m_humidity; }		// Относительная влажность (0...100)
	double GetPressure() const { return m_pressure; }		// Атмосферное давление (в мм.рт.ст)

	void MeasurementsChanged() { NotifyObservers(); }

	void SetMeasurements(double temp, double humidity, double pressure)
	{
		m_humidity = humidity;
		m_temperature = temp;
		m_pressure = pressure;
		MeasurementsChanged();
	}

protected:
	WeatherInfo GetChangedData() const override
	{
		WeatherInfo info;
		info.temperature = GetTemperature();
		info.humidity = GetHumidity();
		info.pressure = GetPressure();
		return info;
	}
private:
	double m_temperature = 0.0;
	double m_humidity = 0.0;
	double m_pressure = 760.0;
};