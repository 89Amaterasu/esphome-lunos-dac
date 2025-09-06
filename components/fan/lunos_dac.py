import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import fan
from esphome.const import CONF_ID

lunos_dac_ns = cg.esphome_ns.namespace("lunos_dac")
LunosDAC = lunos_dac_ns.class_("LunosDAC", cg.Component)
LunosDACFan = lunos_dac_ns.class_("LunosDACFan", fan.Fan, cg.Component)

CONF_LUNOS_DAC_ID = "lunos_dac_id"
CONF_SPEED_COUNT = "speed_count"

CONFIG_SCHEMA = fan.fan_schema(LunosDACFan).extend({
    cv.GenerateID(): cv.declare_id(LunosDACFan),
    cv.GenerateID(CONF_LUNOS_DAC_ID): cv.use_id(LunosDAC),
    cv.Optional(CONF_SPEED_COUNT, default=8): cv.int_range(1, 8),
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    parent = await cg.get_variable(config[CONF_LUNOS_DAC_ID])
    fan_obj = await fan.new_fan(config)
    cg.add(fan_obj.set_device(parent))
    cg.add(fan_obj.set_speed_count(config[CONF_SPEED_COUNT]))
    await cg.register_component(fan_obj, config)
