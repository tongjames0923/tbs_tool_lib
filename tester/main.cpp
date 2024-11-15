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

#define N 200

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

sync_point::SyncPoint sp;

atomic_int count = 0;
int main(int argc, char *argv[]) {
	for (int i = 0; i < N; i++) {
		thread t([i]() {
			LOG_INFO("thread {} start", i);
			this_thread::sleep_for(time_utils::ms(i<20?100:300));
			LOG_WARN("flag is {}", sp.accumulateFlag(1));
		});
		t.detach();
	}
	LOG_WARN("waiting for flag");
	// LOG_WARN("flag is {}", sp.getFlag());
	sp.wait_flag(N, time_utils::ms(200));
	LOG_WARN("flag end is {}", sp.getFlag());
	return 0;
}
