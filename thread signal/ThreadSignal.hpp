#ifndef THREAD_SIGNAL
#define THREAD_SIGNAL

#include <mutex>
#include <condition_variable>

class ThreadSignal {
public:
	template <typename T>
	void wait_and_hold(T func) {
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cond.wait(lock, [&]{return flag;});

		func();
	}

	void wait();
	void notify_one();
	void notify_all();
	void reset();

private:
	std::mutex m_mutex;
	std::condition_variable m_cond;

	bool flag = false;
};

#endif
