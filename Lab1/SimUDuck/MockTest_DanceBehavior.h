#pragma once

#include "IDanceBehavior.h"

// mock-реализация - вместо настоящей работы запоминает факт вызова
class MockDanceBehavior : public IDanceBehavior
{
public:
	void Dance() const override // тут запоминает вызов
	{
		++m_danceCallCount;
		m_wasDanceCalled = true;
	}

	bool WasDanceCalled() const
	{
		return m_wasDanceCalled;
	}

	int GetCallCount() const
	{
		return m_danceCallCount;
	}

	void Reset()
	{
		m_wasDanceCalled = false;
		m_danceCallCount = 0;
	}

private:
	mutable bool m_wasDanceCalled{ false };
	mutable int m_danceCallCount{ 0 };
};