#include "lunos_dac.h"
#include "esphome/core/log.h"

namespace esphome {
namespace lunos_dac {

static const char *const TAG = "lunos_dac";

void LunosDAC::setup() {
  ESP_LOGCONFIG(TAG, "Initializing GP8403 DAC at address 0x%02X", address_);
  dac_ = std::make_unique<DFRobot_GP8403>(&Wire, address_);

  if (!dac_->begin()) {
    ESP_LOGE(TAG, "Failed to initialize GP8403 DAC at address 0x%02X", address_);
    mark_failed();
    return;
  }

  dac_->setDACOutRange(dac_->eOutputRange10V);
  ESP_LOGI(TAG, "DAC initialized successfully");

  uint8_t default_level = static_cast<uint8_t>(default_speed_ * 8);
  ESP_LOGD(TAG, "Applying default speed level %u with oscillation=%s",
           default_level, default_oscillation_ ? "true" : "false");
  set_speed_level(default_level, default_oscillation_);
}

void LunosDAC::dump_config() {
  ESP_LOGCONFIG(TAG, "LunosDAC:");
  ESP_LOGCONFIG(TAG, "  I2C Address: 0x%02X", address_);
  ESP_LOGCONFIG(TAG, "  Default speed: %.2f", default_speed_);
  ESP_LOGCONFIG(TAG, "  Default oscillation: %s", default_oscillation_ ? "true" : "false");
}

void LunosDAC::set_speed_level(uint8_t level, bool oscillating) {
  level = std::min(level, static_cast<uint8_t>(8));

  static const int mv_table_osc[9]    = {750, 1250, 1750, 2250, 2750, 3250, 3750, 4250, 4750};
  static const int mv_table_no_osc[9] = {750, 6250, 6750, 7250, 7750, 8250, 8750, 9250, 9750};

  int dac_mv = oscillating ? mv_table_osc[level] : mv_table_no_osc[level];
  dac_mv = std::max(0, std::min(dac_mv, 10000));

  ESP_LOGI(TAG, "Setting DAC output: %d mV (level %u, oscillating=%s)",
           dac_mv, level, oscillating ? "true" : "false");

  dac_->setDACOutVoltage(dac_mv, 0);
}

void LunosDACFan::setup() {
  ESP_LOGCONFIG(TAG, "Setting up LunosDACFan");
  ESP_LOGCONFIG(TAG, "  Speed levels: %u", speed_count_);
  ESP_LOGCONFIG(TAG, "  Initial oscillation: %s", oscillating_ ? "true" : "false");

  uint8_t default_level = static_cast<uint8_t>(parent_->get_default_speed() * speed_count_);
  parent_->set_speed_level(default_level, oscillating_);
}

void LunosDACFan::dump_config() {
  ESP_LOGCONFIG(TAG, "LunosDACFan:");
  ESP_LOGCONFIG(TAG, "  Speed levels: %u", speed_count_);
  ESP_LOGCONFIG(TAG, "  Oscillation: %s", oscillating_ ? "true" : "false");
}

fan::FanTraits LunosDACFan::get_traits() {
  auto traits = fan::FanTraits();
  traits.set_supported_speed_count(speed_count_);
  traits
