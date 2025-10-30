#include "StarFrameRateMonitor.hpp"
#include "StarTime.hpp"
#include "StarLogging.hpp"

namespace Star {

  // Frame rate and tick rate
  FrameRateApproacher::FrameRateApproacher(Uint64 targetFrameRate, Uint64 targetTickRate)
    : m_targetFrameRate(targetFrameRate), m_targetTickRate(targetTickRate) {
      Uint64 currentTime = SDL_GetPerformanceCounter();
      m_frameEndTime = currentTime;
      m_lastTickTime = currentTime;

      m_timeResolution = SDL_GetPerformanceFrequency();
      m_frameDuration = m_timeResolution / targetFrameRate;
      m_tickDuration = m_timeResolution / targetTickRate;

      // Pre measure sleep duration
      measureSystemSleepDuration();
    }

  Uint64 FrameRateApproacher::targetFrameRate() const {
    return m_targetFrameRate;
  }

  void FrameRateApproacher::setTargetFrameRate(Uint64 targetFrameRate) {
    m_targetFrameRate = targetFrameRate;
    m_frameDuration = m_timeResolution / targetFrameRate;
  }

  Uint64 FrameRateApproacher::targetTickRate() const {
    return m_targetTickRate;
  }

  void FrameRateApproacher::setTargetTickRate(Uint64 targetTickRate) {
    m_targetTickRate = targetTickRate;
    m_tickDuration = m_timeResolution / targetTickRate;
  }

  bool FrameRateApproacher::startFrame() {
    Uint64 currentTime = SDL_GetPerformanceCounter();

    m_nextFrameTime = currentTime + m_frameDuration;

    if (m_lastTickTime + m_tickDuration <= currentTime) {
      m_lastTickTime = currentTime;
      return true;
    }
    return false;
  }

  Sint64 FrameRateApproacher::endFrame() {
    Uint64 currentTime = SDL_GetPerformanceCounter();

    m_frameEndTime = currentTime;
    Uint64 timeRemainingUnchecked = m_nextFrameTime - currentTime;
    
    Sint64 timeRemaining = 0;

    if (timeRemainingUnchecked >= SDL_MAX_UINT64 - (m_timeResolution * 60 * 60)) {
      Uint64 timeRemainingUnwrapped = SDL_MAX_UINT64 - timeRemainingUnchecked;
      timeRemaining = -((Sint64)timeRemainingUnwrapped);
    } else {
      timeRemaining = timeRemainingUnchecked;
    }

    if (timeRemaining > (Sint64)m_timeResolution) {
      timeRemaining = (Sint64)m_timeResolution;
      Logger::info("Frame remaining too long, overriding to 1s {}", m_timeResolution);
    }

    return timeRemaining;
  }

  double FrameRateApproacher::renderRate() {
    return avgFrameTime;
  }

  double FrameRateApproacher::scaleRate() {
    return (double)m_targetTickRate / (double)m_targetFrameRate;
  }

  // Sleep related
  void FrameRateApproacher::measureSystemSleepDuration() {
    for (size_t i = 0; i < 10; i++)
    {
      sleepMs(1);
    }
  }

  void FrameRateApproacher::sleep(Uint64 time) {
    Uint64 timeRemaining = time;
    
    while (timeRemaining > m_frameDuration)
    {
      Uint64 timeTook = sleepMs(1);
      timeRemaining -= timeTook;
    }

    spinlock(timeRemaining);
  }

  Uint64 FrameRateApproacher::sleepMs(Uint64 timeMs) {
    Uint64 currentTime = SDL_GetPerformanceCounter();
    Thread::sleep(timeMs);
    Uint64 newTime = SDL_GetPerformanceCounter();

    Uint64 timeTook = newTime - currentTime;
    if (timeTook > m_systemSleepDuration) {
      m_systemSleepDuration = timeTook;
    }
    return timeTook;
  }

  void FrameRateApproacher::spinlock(Uint64 time) {
    Uint64 startTime = SDL_GetPerformanceCounter();
    Uint64 endTime = startTime + time;
    while (true)
    {
      Uint64 currentTime = SDL_GetPerformanceCounter();
      if (currentTime >= endTime) {
        break;
      }
    }
  }
}
