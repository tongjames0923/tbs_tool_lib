//
// Created by abstergo on 24-11-15.
//

#ifndef TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_SYNC_POINT_SYNCPOINT_H
#define TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_SYNC_POINT_SYNCPOINT_H

#include <atomic>
#include <time_utils.hpp>
#include <defs.h>
#include <functional>
#include <condition_variable>
#include <utility>

namespace tbs::concurrency::sync_point {
using __predic_function = bool (*)();

using __predic_functional = std::function<bool()>;

class SyncPoint {
private:
	std::atomic_int _flag{0};
	std::mutex _mutex;
	std::condition_variable _condition;
	void wait(unsigned long long ms,
	          __predic_functional predic,
	          CONST bool &flagCheck,
	          CONST int &target = -1,
	          CONST bool &timeLimited = true) {
		std::unique_lock<std::mutex> lock(_mutex);
		if (timeLimited) {
			_condition.wait_for(lock,
			                    time_utils::ms(ms),
			                    [&]() { return predic() || (flagCheck && getFlag() >= target); });
		} else {
			_condition.wait(lock,
			                [&]() {
				                return predic() || (flagCheck && getFlag() >= target);
			                });
		}
	}

public:
	void
	wait_util(CONST time_utils::ms &to, __predic_function f) {
		wait_util(to, [&]() { return f(); });
	}

	void wait_util(CONST time_utils::ms &timeout, __predic_functional f) {
		wait(timeout.count(), std::move(f), false);
	}

	void reset() {
		_flag = 0;
	}

	void wait_flag(CONST int &target) {
		wait_flag(target, [&]() { return false; });
	}

	void wait_flag(CONST int &target, __predic_function f) {
		wait_flag(target, [&]() { return f(); });
	}

	void wait_flag(CONST int &target, __predic_functional f) {
		wait(0, std::move(f), true, target, false);
	}

	void wait_flag(CONST int &target, CONST time_utils::ms &m) {
		wait_flag(target, m, [&]() { return false; });
	}

	void wait_flag(CONST int &target, CONST time_utils::ms &ms, __predic_function f) {
		wait_flag(target, ms, [&]() { return f(); });
	}

	void wait_flag(CONST int &target, CONST time_utils::ms &ms, __predic_functional f) {
		wait(ms.count(), std::move(f), true, target);
	}

	int accumulateFlag(const int &delta) {
		const int k = ++_flag;
		_condition.notify_one();
		return k;
	}

	int getFlag() const {
		return _flag;
	}
};
}

#endif // TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_SYNC_POINT_SYNCPOINT_H
