#pragma once

#include "OpenKNX.h"
#include "knxprod.h"
#include "Fan.h"

class FanChannel : public OpenKNX::Channel
{
    private:
        const std::string name() override;
        Fan& _fan;
        void setOpMode(uint8_t opModeIdx);
        void setVentilationMode(uint8_t controlModeIdx);
        void setControlMode(uint8_t controlModeIdx);
        void setHumiditySensorMode(uint8_t humiditySensorModeIdx);

    public:
        FanChannel(uint8_t iChannelNumber, Fan& fan);
        void resetFan();
        void setup(bool configured) override;
        void processInputKo(GroupObject& ko);
        void timerCallback();
};