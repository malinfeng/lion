#pragma once
#include <sys/time.h>


namespace lion
{
	class TimesTamp
	{
	public:
		TimesTamp(int microSecond, bool fromNow = true);
		
		~TimesTamp();


		long long usSinceEpoch() const { return _usSinceEpoch; }


		void addtime_us(int ms);

		long long overNow_us() const;
		long long now_us()const;


		static const int kMicroSecondsPerSecond = 1000 * 1000;
	private:
		long long _usSinceEpoch;
	};

	inline bool operator<(const TimesTamp lhs, const TimesTamp rhs)
	{
		return lhs.usSinceEpoch() < rhs.usSinceEpoch();
	}

	inline bool operator==(const TimesTamp lhs, const TimesTamp rhs)
	{
		return lhs.usSinceEpoch() == rhs.usSinceEpoch();
	}
}



