#pragma once

#include "IFanHardware.h"
#include <Arduino.h>
#include "pico/stdlib.h"

class RP2040FanHardware : public IFanHardware {
public:
    RP2040FanHardware();
    virtual ~RP2040FanHardware();

    void init(uint8_t s1_pin, uint8_t s2_pin, uint8_t sw_pin) override;
    void setPWM(uint8_t pin, int16_t value) override;
    void setDigital(uint8_t pin, bool value) override;
    void startDirectionTimer(long intervalMs, std::function<void()> callback) override;
    void stopDirectionTimer() override;
    void startOneShotTimer(long delayMs, std::function<void()> callback) override;
    void stopOneShotTimer() override;

private:
    static bool staticDirectionCallback(struct repeating_timer *t);
    static int64_t staticTimeoutCallback(alarm_id_t id, void *user_data);

    struct repeating_timer _directionTimer;
    bool _directionTimerActive = false;
    std::function<void()> _directionCallback;
    std::function<void()> _oneShotCallback;
    alarm_id_t _oneShotAlarmID = 0;
    bool _oneShotTimerActive = false;
    
    // PWM frequency from original Fan.h
    const uint16_t pwmFreqHz = 10000; // 10kHz
};
