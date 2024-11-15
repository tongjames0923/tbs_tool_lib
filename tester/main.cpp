//
// Created by abstergo on 24-11-11.
//
#include <time_utils.hpp>
#include <iostream>
#include <log/loggers/BuiltInLogger.h>
#include <match/match_macro.h>

using namespace time_utils;
using namespace std;
using namespace BuiltInLoggers;

ConsoleLogger *consoleLogger = new ConsoleLogger("console");

LoggerWrapper<LogLevel::INFO> logger{consoleLogger};

LoggerWrapper<LogLevel::TRACE> tlogger{consoleLogger};

#define LOGGER_WRAPPER tlogger

#include <log/log_macro.h>

#include <concurrency/LockAdapter.h>
#include <concurrency/sync_point/SyncPoint.h>

using namespace tbs::concurrency;

#include <concurrency/adapters.h>
#include <latch>
#include <guard.h>

#define N 10

class A {
private:
	int a;

public:
	A(const int &i = 0) : a(i) {
	}

	int get() const {
		return a;
	}

	~A() {
		LOG_INFO("destructor called");
	}
};

TimedMutexLockAdapter l;
atomic_bool flag = false;

sync_point::SyncPoint<5> sp;

atomic_int cnt = 0;
int main(int argc, char *argv[]) {
	thread t2([]() {
		LOG_ERROR("thread 2 wait for 50");
		sp.wait_to_predicate([&]() { return cnt >= N / 2; });
		LOG_ERROR("thread 2 wait done for 50");
	});
	t2.detach();
	for (int i = 0; i < N; i++) {
		thread t([i]() {
			LOG_INFO("thread {} start", i);
			LOG_WARN("flag is {}", ++cnt);
			sp.wakeup();
		});
		t.detach();
	}

	LOG_WARN("waiting for flag");
	// LOG_WARN("flag is {}", sp.getFlag());
	sp.wait_to_predicate([&]() { return cnt >= N; });
	LOG_WARN("flag end is {}",cnt.load());
	return 0;
}
