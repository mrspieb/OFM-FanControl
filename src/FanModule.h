#pragma once

#include "MaicoPPB30.h"
#include "FanChannel.h"
#include "OpenKNX.h"
#include "hardware.h"
#include "knxprod.h"
#include "RP2040FanHardware.h"

class FanModule : public OpenKNX::Module {
public:
  void loop() override;
  // void setup() override;
  // Folgende Funktionen werden immer aufgerufen, egal ob konfiguriert oder
  // nicht Damit kann man die Info ob das Gerät konfiguriert ist selbst
  // auswerten void loop(bool configured) override;
  void setup(bool configured) override;

  // Wenn -D OPENKNX_DUALCORE verwendet wird
  // void loop1() override;
  // void setup2() override;
  // Gilt ebenso
  // void loop1(bool configured) override;
  // void setup1(bool configured) override;

  void processAfterStartupDelay() override;
  void processInputKo(GroupObject &ko) override;
  bool sendReadRequest(GroupObject &ko);

  const std::string name() override;
  const std::string version() override;

  // Wenn das Modul auch Daten im Flash speichern soll, werden folgenden
  // Methoden benötigt. void writeFlash() override; void readFlash(const
  // uint8_t* data, const uint16_t size) override; uint16_t flashSize()
  // override;
private:
  RP2040FanHardware _fan1Hw;
  MaicoPPB30 _fan1 = MaicoPPB30(_fan1Hw, FAN1_S1_PWM_PIN, FAN1_S2_PWM_PIN, FAN1_SW_PIN);
  
  RP2040FanHardware _fan2Hw;
  MaicoPPB30 _fan2 = MaicoPPB30(_fan2Hw, FAN2_S1_PWM_PIN, FAN2_S2_PWM_PIN, FAN2_SW_PIN);
  
  FanChannel *_channel[FAN_ChannelCount];
  uint32_t readRequestDelay = 0;
};

// Wir benutzen das, um in main besser auf das Modul zugreifen zu können
extern FanModule openknxFanModule;