import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID
from esphome.components import i2c

lunos_dac_ns = cg.esphome_ns.namespace("lunos_dac")
LunosDAC = lunos_dac_ns.class_("LunosDAC", cg.Component)

CONF_ADDRESS = "address"
CONF_DEFAULT_SPEED = "default_speed"
CONF_DEFAULT_OSCILLATION = "default_oscillation"

CONFIG_SCHEMA = cv.Schema({
    cv.Required(CONF_ID): cv.declare_id(LunosDAC),
    cv.Required(CONF_ADDRESS): cv.i2c_address,
    cv.Optional(CONF_DEFAULT_SPEED, default=0.25): cv.percentage,
    cv.Optional(CONF_DEFAULT_OSCILLATION, default=True): cv.boolean,
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    cg.add(var.set_address(config[CONF_ADDRESS]))
    cg.add(var.set_default_speed(config[CONF_DEFAULT_SPEED]))
    cg.add(var.set_default_oscillation(config[CONF_DEFAULT_OSCILLATION]))
