#include "FanModule.h"


const std::string FanModule::name() { return "FanModule"; }

const std::string FanModule::version() {
  return std::to_string(FAN_ModuleVersion);
}

// void FanModule::setup()
// {
// }

void FanModule::setup(bool configured) {
  _channel[0] = new FanChannel(0, _fan1);
  _channel[1] = new FanChannel(1, _fan2);

  for (int i = 0; i < FAN_ChannelCount; i++) {
    _channel[i]->setup(configured);
  }
  // _fan1.setFanSpeed(FanSpeed::Level3);
  // _fan1.setTimer(10);
  // _fan2.setFanSpeed(FanSpeed::Level3);
}

void FanModule::loop() {
  if (!openknx.afterStartupDelay())
    return;
  for (int i = 0; i < FAN_ChannelCount; i++) {
    _channel[i]->loop();
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