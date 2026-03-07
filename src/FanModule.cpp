#include "FanModule.h"
#include "IFanHardware.h"


const std::string FanModule::name() { return "FanModule"; }

const std::string FanModule::version() {
  return std::to_string(FAN_ModuleVersion);
}

// void FanModule::setup() {}

void FanModule::setup(bool configured) {
  if(ParamFAN_StatusLED == 1) {
    _fan1Hw.setDigital(STATUS_LED_PIN, true);
  } else {
  _fan1Hw.setDigital(STATUS_LED_PIN, false);
  }

  _channel[0] = new FanChannel(0, _fan1);
  _channel[1] = new FanChannel(1, _fan2);

  for (int i = 0; i < FAN_ChannelCount; i++) {
    _channel[i]->setup(configured);
  }
}

void FanModule::loop() {
  if (!openknx.afterStartupDelay())
    return;

  bool anyFanRunning = false;
  for (int i = 0; i < FAN_ChannelCount; i++) {
    _channel[i]->loop();
    if (_channel[i]->getFanSpeed() > 0) {
      anyFanRunning = true;
    }
  }

  if (ParamFAN_StatusLED == 2) {
    _fan1Hw.setDigital(STATUS_LED_PIN, anyFanRunning);
  } else {
    _fan1Hw.setDigital(STATUS_LED_PIN, false);
  }
}

void FanModule::processInputKo(GroupObject &ko) {
  for (int i = 0; i < FAN_ChannelCount; i++) {
    _channel[i]->processInputKo(ko);
  }
}

// void FanModule::loop(bool configured)
// {
//     for(int i = 0; i < FAN_ChannelCount; i++)
//     {
//         channel[i]->loop(configured);
//     }
// }

void FanModule::processAfterStartupDelay() {
  for (int i = 0; i < FAN_ChannelCount; i++) {
    _channel[i]->resetFan();
  }
}

bool FanModule::sendReadRequest(GroupObject &ko) {
  // ensure, that we do not send too many read requests at the same time
  if (delayCheck(readRequestDelay, 300)) // 3 per second
  {
    // we handle input KO and we send only read requests, if KO is uninitialized
    if (!ko.initialized())
      ko.requestObjectRead();
    readRequestDelay = delayTimerInit();
    return true;
  }
  return false;
}

FanModule openknxFanModule;