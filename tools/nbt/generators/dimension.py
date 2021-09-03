#!/usr/bin/env python3

import nbt as NBT

data = NBT.RootCompound(
    name="",
    value={
        "piglin_safe": NBT.TagByte(0),
        "natural": NBT.TagByte(1),
        "ambient_light": NBT.TagFloat(0.5),
        "infiniburn": NBT.TagString("minecraft:infiniburn_overworld"),
        "respawn_anchor_works": NBT.TagByte(0),
        "has_skylight": NBT.TagByte(1),
        "bed_works": NBT.TagByte(1),
        "effects": NBT.TagString("minecraft:overworld"),
        "has_raids": NBT.TagByte(1),
        "min_y": NBT.TagInt(0),
        "height": NBT.TagInt(256),
        "logical_height": NBT.TagInt(256),
        "coordinate_scale": NBT.TagDouble(1.0),
        "ultrawarm": NBT.TagByte(0),
        "has_ceiling": NBT.TagByte(0),
    }
)

if __name__ == "__main__":
    NBT.write(data, "dimension.nbt")
