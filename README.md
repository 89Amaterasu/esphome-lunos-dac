# ESPHome Lunos 5/UNI-FT & 5/SC-FT Controller

This ESPHome external component provides a native interface to control **Lunos 5/UNI-FT** and **5/SC-FT** universal control units using a DFRobot GP8403 I2C DAC.

## 🛠 Hardware
- **Microcontroller:** Wemos D1 Mini
- **DAC:** [DFRobot GP8403](https://www.dfrobot.com/product-2612.html)
- **Lunos Controller:** 5/UNI-FT or 5/SC-FT

## 🔌 Wiring
| Wemos D1 Mini | GP8403 DAC | Lunos 5/UNI-FT / 5/SC-FT |
| :--- | :--- | :--- |
| **D1 (SCL)** | SCL | - |
| **D2 (SDA)** | SDA | - |
| **GND** | GND | **GND (Common Ground)** |
| - | **CH0** | **Input E1** (0-10V Signal) |



## 💻 Installation

Add this repository to your ESPHome configuration.

```yaml
external_components:
  - source:
      type: git
      url: [https://github.com/89Amaterasu/esphome-lunos-dac.git](https://github.com/89Amaterasu/esphome-lunos-dac.git)
      ref: main
    components: [ lunos_dac_fan ]

i2c:
  sda: 4  # GPIO4 / D2 on D1 Mini
  scl: 5  # GPIO5 / D1 on D1 Mini
  scan: true

fan:
  - platform: lunos_dac_fan
    name: "Lunos Ventilation"
    address: 0x58            # I2C address for the GP8403 DAC
    boot_speed: 3            
    boot_oscillation: true
