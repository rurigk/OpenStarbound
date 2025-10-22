#include "StarFrameRateMonitor.hpp"
#include "StarTime.hpp"
#include "StarLogging.hpp"

namespace Star {

  // Frame rate and tick rate assumes its a number without decimals ie 60.0 120.0
  FrameRateApproacher::FrameRateApproacher(unsigned int targetFrameRate, unsigned int targetTickRate)
    : m_targetFrameRate(targetFrameRate), m_targetTickRate(targetTickRate) {
      // static_cast<double>(10000000000000001LL)
      double currentTime = Time::monotonicTime();
      m_frameEndTime = currentTime;
      m_lastTickTime = currentTime;
    }

  double FrameRateApproacher::targetFrameRate() const {
    return m_targetFrameRate;
  }

  void FrameRateApproacher::setTargetFrameRate(unsigned int targetFrameRate) {
    m_targetFrameRate = targetFrameRate;
  }

  double FrameRateApproacher::targetTickRate() const {
    return m_targetTickRate;
  }

  void FrameRateApproacher::setTargetTickRate(unsigned int targetTickRate) {
    m_targetTickRate = targetTickRate;
  }

  bool FrameRateApproacher::startFrame() {
    double frameDuration = 1.0f / m_targetFrameRate;
    double tickDuration = 1.0f / m_targetTickRate;
    double currentTime = Time::monotonicTime();

    m_frameStartTime = currentTime;
    m_nextFrameTime = m_frameStartTime + frameDuration;

    if (m_lastTickTime + tickDuration <= currentTime) {
      // double avgTickTime = currentTime - m_lastTickTime;
      // Logger::info("TTM: {}", avgTickTime * 1000);
      m_lastTickTime = currentTime;
      return true;
    }

    // if (m_tickCount >= m_targetTickRate - 1.0f) {
    //   m_tickCount = 0.0f;
    //   return true;
    // }

    // m_tickCount += 1.0f;
    return false;
  }

  double FrameRateApproacher::endFrame() {
    double currentTime = Time::monotonicTime();
    
    avgFrameTime = currentTime - m_frameEndTime;
    // Logger::info("FPS: {}", m_targetFrameRate / (avgFrameTime * m_targetFrameRate));

    m_frameEndTime = currentTime;
    double timeRemaining = m_nextFrameTime - currentTime;
    if (timeRemaining < 0.0f) {
      timeRemaining = 0.0f;
    }

    return timeRemaining;
  }

  double FrameRateApproacher::renderRate() {
    return avgFrameTime;
  }

  double FrameRateApproacher::scaleRate() {
    return m_targetTickRate / m_targetFrameRate;
  }

}
