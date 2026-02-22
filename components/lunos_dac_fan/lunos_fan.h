#pragma once

#include "esphome/core/component.h"
#include "esphome/components/fan/fan.h"
#include "esphome/components/i2c/i2c.h"
#include "DFRobot_GP8403.h"

namespace esphome {
namespace lunos_dac_fan {

class LunosDACFan : public fan::Fan, public Component, public i2c::I2CDevice {
 protected:
  DFRobot_GP8403 dac_;
  int boot_speed_;
  bool boot_oscillation_;

  const uint16_t mv_osc[9] = {750, 1250, 1750, 2250, 2750, 3250, 3750, 4250, 4750};
  const uint16_t mv_sum[9] = {750, 6250, 6750, 7250, 7750, 8250, 8750, 9250, 9750};

 public:
  LunosDACFan() : dac_(&Wire, 0x58) {}

  void set_boot_speed(int speed) { this->boot_speed_ = speed; }
  void set_boot_oscillation(bool osc) { this->boot_oscillation_ = osc; }

  void setup() override {
    dac_ = DFRobot_GP8403(&Wire, this->address_);
    
    if (dac_.begin() != 0) {
      ESP_LOGE("lunos", "GP8403 DAC not found at address 0x%02X", this->address_);
    }
    dac_.setDACOutRange(dac_.eOutputRange10V);

    this->state = true;
    this->speed = this->boot_speed_;
    this->oscillating = this->boot_oscillation_;

    uint16_t boot_mv = this->oscillating ? mv_osc[this->speed] : mv_sum[this->speed];
    dac_.setDACOutVoltage(boot_mv, 0);
    this->publish_state();
  }

  fan::FanTraits get_traits() override {
    auto traits = fan::FanTraits();
    traits.set_oscillation(true);
    traits.set_speed(true);
    traits.set_supported_speed_count(8);
    return traits;
  }

  void control(const fan::FanCall &call) override {
    if (call.get_state().has_value()) this->state = *call.get_state();
    if (call.get_speed().has_value()) this->speed = *call.get_speed();
    if (call.get_oscillating().has_value()) this->oscillating = *call.get_oscillating();

    uint16_t voltage_mv = 750; 
    if (this->state && this->speed > 0) {
      voltage_mv = this->oscillating ? mv_osc[this->speed] : mv_sum[this->speed];
    }
    dac_.setDACOutVoltage(voltage_mv, 0);
    this->publish_state();
  }
};

}  // namespace lunos_dac_fan
}  // namespace esphome
