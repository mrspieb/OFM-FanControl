#pragma once
#include <stdint.h>
#include <math.h>
#include <array>
#include <functional>
#include "IFanHardware.h"


class Fan {
public:
  enum VentilationMode {
    HeatRecovery = 0,
    SupplyAir = 1,
    ExhaustAir = 2,
  };

  enum OperatingMode {
    Off = 0,
    Manual = 1,
    Automatic = 2,
  };

  enum ControlMode {
    Threshold = 0,
    Adaptive = 1,
  };

  enum HumiditySensorMode {
    Relative = 0,
    Absolute = 1,
  };


  virtual ~Fan() = default;

  Fan(IFanHardware& hw);
  virtual void setVentilationMode(VentilationMode ventilationMode);
  virtual void setOperatingMode(OperatingMode operatingMode);
  virtual void setControlMode(ControlMode controlMode);
  void setFanSpeed(int16_t fanSpeed); // for speed changes from outside
  void setTimer(uint64_t secondsRemaining, std::function<void()> timerCallback);
  void stopTimer();
  void setSpeedChangeCallback(std::function<void(int16_t)> callback);
  
  bool setInsideHumdity(float insideRelHumidity);
  void setInsideTemperature(float insideTemperature);
  void setOutsideHumidity(float outsideRelHumidity);
  void setOutsideTemperature(float outsideTemperature);
  virtual int16_t getFanSpeed() = 0;
  static float getDewPoint(float relHumidity, float temperature);

  HumiditySensorMode humiditySensorMode = HumiditySensorMode::Relative;
  float thresholdHumidityOn = 60;
  float thresholdHumidityOff = 60;
  int16_t thresholdSpeed = 4;

protected:
  void changeFanSpeed(int16_t fanSpeed, bool force = false); //used for changes from within base or derived classes
  virtual void changeFanSpeedDelegate(int16_t fanSpeed) = 0; //specific speed change implementation in derived classes
  virtual void updateMode() = 0;
  void updateEnvironment();
  bool outsideAbsHumidityLower();
  void activateAutoMode();
  
  // Callbacks used by logic
  void onTimeoutTimer();

  IFanHardware& _hw;

  VentilationMode _ventilationMode = VentilationMode::HeatRecovery;
  OperatingMode _operatingMode = OperatingMode::Manual;
  ControlMode _controlMode = ControlMode::Threshold;
  
  bool _manualOverrideActive = false;
  const float _controlGain = 0.18; // TODO: determine proper gain value

  float _outsideRelHumidity = 0;
  float _insideRelHumidity = 0;
  float _outsideTemperature = 0;
  float _insideTemperature = 0;

  std::function<void()> _timerCallback;
  std::function<void(int16_t)> _speedChangeCallback;
};
