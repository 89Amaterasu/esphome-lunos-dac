import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import fan, i2c
from esphome.const import CONF_OUTPUT_ID
from . import lunos_ns, LunosDACFan, CONF_BOOT_SPEED, CONF_BOOT_OSCILLATION

CONFIG_SCHEMA = fan.fan_schema(LunosDACFan).extend({
    cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(LunosDACFan),
    cv.Optional(CONF_BOOT_SPEED, default=3): cv.int_range(min=0, max=8),
    cv.Optional(CONF_BOOT_OSCILLATION, default=True): cv.boolean,
}).extend(cv.COMPONENT_SCHEMA).extend(i2c.i2c_device_schema(0x58))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await cg.register_component(var, config)
    await fan.register_fan(var, config)
    await i2c.register_i2c_device(var, config)
    
    cg.add(var.set_boot_speed(config[CONF_BOOT_SPEED]))
    cg.add(var.set_boot_oscillation(config[CONF_BOOT_OSCILLATION]))
    cg.add_library("dfrobot/DFRobot_GP8403", None)
