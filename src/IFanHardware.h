#pragma once
#include <stdint.h>
#include <functional>

/**
 * @brief Interface for hardware specific operations required by the Fan class.
 * This allows decoupling the logic from the actual hardware (GPIO, Timers),
 * enabling unit testing with mocks.
 */
class IFanHardware {
public:
    virtual ~IFanHardware() = default;

    /**
     * @brief Initialize the hardware pins.
     * 
     * @param s1_pin Pin for S1 PWM
     * @param s2_pin Pin for S2 PWM
     * @param sw_pin Pin for Switch
     */
    virtual void init(uint8_t s1_pin, uint8_t s2_pin, uint8_t sw_pin) = 0;

    /**
     * @brief Set PWM duty cycle for a specific pin.
     * 
     * @param pin The GPIO pin number.
     * @param value The PWM value (0-1024 typically, depending on resolution).
     */
    virtual void setPWM(uint8_t pin, int16_t value) = 0;

    /**
     * @brief Set digital output for a specific pin.
     * 
     * @param pin The GPIO pin number.
     * @param value true for HIGH, false for LOW.
     */
    virtual void setDigital(uint8_t pin, bool value) = 0;

    /**
     * @brief Start a repeating timer for direction switching.
     * 
     * @param intervalMs Interval in milliseconds.
     * @param callback Function to call when timer expires.
     */
    virtual void startDirectionTimer(long intervalMs, std::function<void()> callback) = 0;

    /**
     * @brief Stop the repeating direction timer.
     */
    virtual void stopDirectionTimer() = 0;

    /**
     * @brief Start a one-shot timer.
     * 
     * @param delayMs Delay in milliseconds.
     * @param callback Function to call when timer expires.
     */
    virtual void startOneShotTimer(long delayMs, std::function<void()> callback) = 0;

    /**
     * @brief Stop the one-shot timer if it is running.
     */
    virtual void stopOneShotTimer() = 0;
};
