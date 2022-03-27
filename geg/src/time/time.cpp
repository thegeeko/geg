#include "time.hpp"

#include <chrono>
#include <ctime>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::nanoseconds;
using std::chrono::system_clock;
using std::chrono::steady_clock;
using std::chrono::time_point;
using std::chrono::high_resolution_clock;
using std::chrono::duration;

namespace Geg {
		static bool timerInit = false; // check if this the first time
		static time_point<high_resolution_clock> startTime; // high reslotion timer for the engine
		static float lastFrameTime; // to calculate delta time

		static long time; // time in s since time since epoch 
		static long timeMs; // same as above but in ms
		static double gegTime; // high reslotion since the beginning of the engine
		static double gegTimeMs; // same as above but in ms
		static float deltaTime;


		void Time::updateTime() {
			if (!timerInit) {
				startTime = high_resolution_clock::now();
				lastFrameTime = 0;
				timerInit = true;
			}

			auto msSinceEpoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

			time = msSinceEpoch / 1000;
			timeMs = msSinceEpoch;

			duration<double> durationSinceBeginning = high_resolution_clock::now() - startTime;
			double nsSinceBeginning = duration_cast<std::chrono::nanoseconds>(durationSinceBeginning).count();

			gegTime = nsSinceBeginning / 1000000000;
			gegTimeMs = nsSinceBeginning / 1000000;
			deltaTime = gegTime - lastFrameTime;

			lastFrameTime = gegTime;

			if (Geg::Input::isKeyPressed(GEG_KEY_SPACE)) {
				GEG_CORE_INFO("----------- TIME ------------");
				GEG_CORE_TRACE("Time : {}", std::to_string(Time::getTime()));
				GEG_CORE_TRACE("TimeMs : {}", Time::getTimeMs());
				GEG_CORE_TRACE("EngineTime : {}", std::to_string(Time::getGegTime()));
				GEG_CORE_TRACE("EngineTimeMs : {}", Time::getGegTimeMs());
				GEG_CORE_TRACE("EngineDeltaTime : {}", Time::getDeltaTime());
			}
		}

		const long& Time::getTime() { return time; };
		const long& Time::getTimeMs() { return timeMs; };
		const double& Time::getGegTime() { return gegTime; };
		const double& Time::getGegTimeMs() { return gegTimeMs; };
		const float& Time::getDeltaTime() { return deltaTime; };

}		 // namespace Geg::Time
