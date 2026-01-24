#include "RP2040FanHardware.h"


RP2040FanHardware::RP2040FanHardware() {
}

RP2040FanHardware::~RP2040FanHardware() {
    stopDirectionTimer();
}

void RP2040FanHardware::init(uint8_t s1_pin, uint8_t s2_pin, uint8_t sw_pin) {
    pinMode(sw_pin, OUTPUT);
    pinMode(s1_pin, OUTPUT);
    pinMode(s2_pin, OUTPUT);
    
    analogWriteFreq(pwmFreqHz);
    analogWriteResolution(10);
}

void RP2040FanHardware::setPWM(uint8_t pin, int16_t value) {
    analogWrite(pin, value);
}

void RP2040FanHardware::setDigital(uint8_t pin, bool value) {
    digitalWrite(pin, value ? HIGH : LOW);
}

bool RP2040FanHardware::staticDirectionCallback(struct repeating_timer *t) {
    auto hw = static_cast<RP2040FanHardware*>(t->user_data);
    if (hw && hw->_directionCallback) {
        hw->_directionCallback();
    }
    return true;
}

void RP2040FanHardware::startDirectionTimer(long intervalMs, std::function<void()> callback) {
    if (_directionTimerActive) {
        cancel_repeating_timer(&_directionTimer);
    }
    _directionCallback = callback;
    _directionTimerActive = true;
    add_repeating_timer_ms(intervalMs, staticDirectionCallback, this, &_directionTimer);
}

void RP2040FanHardware::stopDirectionTimer() {
    if (_directionTimerActive) {
        cancel_repeating_timer(&_directionTimer);
        _directionTimerActive = false;
    }
}

int64_t RP2040FanHardware::staticTimeoutCallback(alarm_id_t id, void *user_data) {
    auto hw = static_cast<RP2040FanHardware*>(user_data);
    if (hw) {
        hw->_oneShotTimerActive = false;
        if (hw->_oneShotCallback) {
            hw->_oneShotCallback();
        }
    }
    return 0;
}

void RP2040FanHardware::startOneShotTimer(long delayMs, std::function<void()> callback) {
    stopOneShotTimer();
    _oneShotCallback = callback;
    _oneShotTimerActive = true;
    _oneShotAlarmID = add_alarm_in_ms(delayMs, staticTimeoutCallback, this, false);
}

void RP2040FanHardware::stopOneShotTimer() {
    if (_oneShotTimerActive) {
        cancel_alarm(_oneShotAlarmID);
        _oneShotTimerActive = false;
    }
}