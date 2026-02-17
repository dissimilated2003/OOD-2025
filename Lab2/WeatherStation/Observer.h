#pragma once

#include <set>
#include <map>
#include <functional>
#include <iostream>
#include <string>
#include <climits>

/*
Шаблонный интерфейс IObserver. Его должен реализовывать класс,
желающий получать уведомления от соответствующего IObservable
Параметром шаблона является тип аргумента,
передаваемого Наблюдателю в метод Update
*/
template <typename T>
class IObservable;

template <typename T>
class IObserver
{
public:
	virtual void Update(T const& data, IObservable<T>& subject) = 0;

	virtual ~IObserver() = default;
};

/*
Шаблонный интерфейс IObservable. Позволяет подписаться и отписаться на оповещения, а также
инициировать рассылку уведомлений зарегистрированным наблюдателям.
*/
template <typename T>
class IObservable
{
public:
	virtual ~IObservable() = default;

	virtual void RegisterObserver(IObserver<T>& observer) = 0;
	virtual void NotifyObservers() = 0;
	virtual void RemoveObservers(IObserver<T>& observer) = 0;
};

// Реализация интерфейса IObservable
template <class T>
class CObservable : public IObservable<T>
{
public:
	void RegisterObserver(IObserver<T>& observer, int priority = 0)
	{
		m_observers.emplace(priority, &observer);
	}

	void RegisterObserver(IObserver<T>& observer) override
	{
		RegisterObserver(observer, 0);
	}

	void NotifyObservers() override
	{
		// задание 2 - копируются наблюдатели и мы работаем с копией наблюдателей
		// если кто-то удалит себя из оригинальной - работа с копией не сломается
		T data = GetChangedData();
		auto observersCopy = m_observers;

		for (auto it = observersCopy.rbegin(); it != observersCopy.rend(); ++it)
		{
			it->second->Update(data, *this);
		}
	}

	void RemoveObservers(IObserver<T>& observer) override
	{
		for (auto it = m_observers.begin(); it != m_observers.end(); )
		{
			if (it->second == &observer)
			{
				it = m_observers.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

protected:
	// Классы-наследники должны перегрузить данный метод, 
	// в котором возвращать информацию об изменениях в объекте
	virtual T GetChangedData() const = 0;

private:
	std::multimap<int, IObserver<T>*> m_observers;
};	

template <typename T>
class Statistic
{
public:
	void Update(T value)
	{
		if (m_min > value)
		{
			m_min = value;
		}

		if (m_max < value)
		{
			m_max = value;
		}

		m_acc += value;
		++m_countAcc;
	}

	void Display(const std::string& name) const
	{
		std::cout << "Max " << name << " " << m_max << "\n";
		std::cout << "Min " << name << " " << m_min << "\n";
		std::cout << "Average " << name << " " << (m_acc / m_countAcc) << "\n";
	}

	void Reset()
	{
		m_min = std::numeric_limits<T>::infinity();
		m_max = -std::numeric_limits<T>::infinity();
		m_acc = 0;
		m_countAcc = 0;
	}

private:
	T m_min = std::numeric_limits<T>::infinity();
	T m_max = -std::numeric_limits<T>::infinity();
	T m_acc = 0;
	unsigned m_countAcc = 0;
};