#pragma once

#include "esphome/core/component.h"
#include "esphome/components/fan/fan.h"
#include "DFRobot_GP8403.h"
#include <memory>

namespace esphome {
namespace lunos_dac {

class LunosDAC : public Component {
 public:
  void setup() override;
  void dump_config() override;

  void set_speed_level(uint8_t level, bool oscillating);
  void set_default_speed(float speed) { default_speed_ = speed; }
  void set_default_oscillation(bool oscillation) { default_oscillation_ = oscillation; }
  void set_address(uint8_t address) { address_ = address; }

  float get_default_speed() const { return default_speed_; }
  bool get_default_oscillation() const { return default_oscillation_; }

 protected:
  float default_speed_{0.25f};
  bool default_oscillation_{true};
  uint8_t address_{0x58};
  std::unique_ptr<DFRobot_GP8403> dac_;
};

class LunosDACFan : public fan::Fan {
 public:
  void setup() override;
  void dump_config() override;
  fan::FanTraits get_traits() override;
  void control(const fan::FanCall &call) override;

  void set_device(LunosDAC *device) { parent_ = device; }
  void set_speed_count(uint8_t count) { speed_count_ = count; }

 protected:
  LunosDAC *parent_;
  uint8_t speed_count_{8};
  bool oscillating_{true};
};

}  // namespace lunos_dac
}  // namespace esphome
