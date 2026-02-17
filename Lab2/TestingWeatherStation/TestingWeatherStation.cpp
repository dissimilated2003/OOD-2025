#define CATCH_CONFIG_MAIN

#include "../WeatherStation/WeatherData.h"
#include "../WeatherStation/Observer.h"

#include "../../../catch2/catch.hpp"

// самоотписывающийся чел
class CSelfRemovingObserver : public IObserver<WeatherInfo>
{
public:
	CSelfRemovingObserver(IObservable<WeatherInfo>& subject) : m_subject(subject) {}

	~CSelfRemovingObserver()
	{
		m_subject.RemoveObservers(*this);
	}

	void Update(const WeatherInfo& data, IObservable<WeatherInfo>& subject) override
	{
		m_updateCount++;
		if (m_updateCount == 1)
		{
			m_subject.RemoveObservers(*this); 
		}
	}

private:
	IObservable<WeatherInfo>& m_subject;
	int m_updateCount{};
};

// просто считающий обновления наблюдатель
class CCounterObserver : public IObserver<WeatherInfo>
{
public:
	int GetUpdateCount() const
	{
		return m_updateCount;
	}

	void Update(const WeatherInfo& data, IObservable<WeatherInfo>& subject) override
	{
		m_updateCount++;
	}

private:
	int m_updateCount{};
};

TEST_CASE("Self removing observer do not break notifying")
{
	WeatherData wd{};
	CSelfRemovingObserver selfRemover(wd);
	CCounterObserver counter{};
	wd.RegisterObserver(counter, 1);

	REQUIRE_NOTHROW(wd.SetMeasurements(25, 65, 760)); //этот вызов не должен упасть
	REQUIRE(counter.GetUpdateCount() == 1);

	REQUIRE_NOTHROW(wd.SetMeasurements(24, 64, 761));
	REQUIRE(counter.GetUpdateCount() == 2);
}

TEST_CASE("Testing several self deleting observers")
{
	WeatherData wd{};
	CSelfRemovingObserver selfrem1(wd);
	CSelfRemovingObserver selfrem2(wd);

	CCounterObserver counter{};
	wd.RegisterObserver(counter, 1);

	REQUIRE_NOTHROW(wd.SetMeasurements(23, 70, 768));
	REQUIRE(counter.GetUpdateCount() == 1);

	REQUIRE_NOTHROW(wd.SetMeasurements(25, 69, 759));
	REQUIRE(counter.GetUpdateCount() == 2);
}

TEST_CASE("Priority observer")
{
	WeatherData wd{};
	std::vector<int> updateOrder;

	class CPriorityObserver : public IObserver<WeatherInfo>
	{
	public:
		CPriorityObserver(std::vector<int>& order, int id) : m_order(order), m_id(id) {}

		void Update(const WeatherInfo& data, IObservable<WeatherInfo>& subject) override
		{
			m_order.push_back(m_id);
		}
	private:
		int m_id{};
		std::vector<int>& m_order;
	};

	CPriorityObserver low(updateOrder, 1);
	CPriorityObserver high(updateOrder, 2);
	CPriorityObserver mid(updateOrder, 3);

	wd.RegisterObserver(low, 10);
	wd.RegisterObserver(high, 100);
	wd.RegisterObserver(mid, 50);

	wd.SetMeasurements(25, 75, 760);
	REQUIRE(updateOrder ==			std::vector<int>{2, 3, 1});
	REQUIRE_FALSE(updateOrder ==	std::vector<int>{3, 2, 1});
	REQUIRE_FALSE(updateOrder ==	std::vector<int>{1, 2, 3});
}

//петрович врубай насос
TEST_CASE("Mock realisation of multi-subscription")
{
	WeatherData indoor;
	WeatherData outdoor;

	class MockDisplay : public IObserver<WeatherInfo>
	{
	public:
		MockDisplay(
			IObservable<WeatherInfo>& in,
			IObservable<WeatherInfo>& out)
			:
			indoor(in),
			outdoor(out)
		{}

		void Update(const WeatherInfo& data, IObservable<WeatherInfo>& subj) override
		{
			lastLocation = (&subj == &indoor) ? "indoor" : "outdoor";
		}
	
		std::string lastLocation{};
		IObservable<WeatherInfo>& indoor;
		IObservable<WeatherInfo>& outdoor;
	};

	MockDisplay mock{ indoor, outdoor };
	indoor.RegisterObserver(mock, 1);
	outdoor.RegisterObserver(mock, 1);

	indoor.SetMeasurements(24.5, 67.8, 761.1);
	REQUIRE(mock.lastLocation == "indoor");
	outdoor.SetMeasurements(23.4, 68.3, 759.2);
	REQUIRE(mock.lastLocation == "outdoor");

	indoor.SetMeasurements(24.5, 67.8, 761.1);
	REQUIRE(mock.lastLocation == "indoor");
	outdoor.SetMeasurements(23.4, 68.3, 759.2);
	REQUIRE(mock.lastLocation == "outdoor");
}