import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import CONF_ID

lunos_ns = cg.esphome_ns.namespace("lunos_dac_fan")
LunosDACFan = lunos_ns.class_("LunosDACFan", cg.Component, i2c.I2CDevice)

CONF_BOOT_SPEED = "boot_speed"
CONF_BOOT_OSCILLATION = "boot_oscillation"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(LunosDACFan),
    cv.Optional(CONF_BOOT_SPEED, default=3): cv.int_range(min=0, max=8),
    cv.Optional(CONF_BOOT_OSCILLATION, default=True): cv.boolean,
}).extend(cv.COMPONENT_SCHEMA).extend(i2c.i2c_device_schema(0x58))
