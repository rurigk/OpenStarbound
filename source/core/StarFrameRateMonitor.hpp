#pragma once

#include "StarList.hpp"

namespace Star {
    class FrameRateApproacher {
    public:
        FrameRateApproacher(unsigned int targetFrameRate, unsigned int targetTickRate);

        double targetFrameRate() const;
        void setTargetFrameRate(unsigned int targetFrameRate);

        double targetTickRate() const;
        void setTargetTickRate(unsigned int targetTickRate);

        bool startFrame();
        double endFrame();

        double renderRate();
        double scaleRate();

    private:
        double m_targetFrameRate;
        double m_targetTickRate;

        double m_frameStartTime;
        double m_frameEndTime;
        double m_nextFrameTime;

        double m_lastTickTime;
        double m_tickCount = 0;

        double avgFrameTime;
    };
};
