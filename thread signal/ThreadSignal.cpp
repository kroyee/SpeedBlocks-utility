#include "ThreadSignal.hpp"

void ThreadSignal::wait() {
	std::unique_lock<std::mutex> lock(m_mutex);
	m_cond.wait(lock, [&]{return flag;});
}

void ThreadSignal::notify_one() {
	{
		std::lock_guard<std::mutex> lg(m_mutex);
		flag = true;
	}
	m_cond.notify_one();
}

void ThreadSignal::notify_all() {
	{
		std::lock_guard<std::mutex> lg(m_mutex);
		flag = true;
	}
	m_cond.notify_all();
}

void ThreadSignal::reset() {
	std::lock_guard<std::mutex> lg(m_mutex);
	flag = false;
}
