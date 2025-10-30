#pragma once

#include "StarList.hpp"
#include "SDL3/SDL.h"

namespace Star {
    class FrameRateApproacher {
    public:
        FrameRateApproacher(Uint64 targetFrameRate, Uint64 targetTickRate);

        Uint64 targetFrameRate() const;
        void setTargetFrameRate(Uint64 targetFrameRate);

        Uint64 targetTickRate() const;
        void setTargetTickRate(Uint64 targetTickRate);

        bool startFrame();
        Sint64 endFrame();

        double renderRate();
        double scaleRate();

        void sleep(Uint64 time);

    private:
        void measureSystemSleepDuration();
        Uint64 sleepMs(Uint64 time);
        void spinlock(Uint64 time);
        // This are mostly informational
        Uint64 m_targetFrameRate;
        Uint64 m_targetTickRate;

        // Timer resolution for each second
        Uint64 m_timeResolution;
        // Time that a frame should take
        Uint64 m_frameDuration;
        // Time that a tick should take
        Uint64 m_tickDuration;

        // Timestamps for time measuring in frame rate
        Uint64 m_frameEndTime;
        Uint64 m_nextFrameTime;

        // Timestamps for time measuring in tick rate
        Uint64 m_lastTickTime;

        // This is the time the system sleep function took to wake up
        Uint64 m_systemSleepDuration;

        double avgFrameTime;
    };
};
