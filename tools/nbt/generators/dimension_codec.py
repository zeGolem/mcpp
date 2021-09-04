#!/usr/bin/env python3

import nbt as NBT

data = NBT.RootCompound(
    name="",
    value={
        "minecraft:dimension_type": NBT.TagCompound({
            "type": NBT.TagString("minecraft:dimension_type"),
            "value": NBT.TagList([
                NBT.TagCompound({
                    "name": NBT.TagString("minecraft:overworld"),
                    "id": NBT.TagInt(0),
                    "element": NBT.TagCompound({
                        "piglin_safe": NBT.TagByte(0),
                        "natural": NBT.TagByte(1),
                        "ambient_light": NBT.TagFloat(0.0),
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
                    })
                }),
            ]),
        }),
        "minecraft:worldgen/biome": NBT.TagCompound({
            "type": NBT.TagString("minecraft:worldgen/biome"),
            "value": NBT.TagList([
                NBT.TagCompound({
                    "name": NBT.TagString("minecraft:plains"),
                    "id": NBT.TagInt(1),
                    "element": NBT.TagCompound({
                        "precipitation": NBT.TagString("rain"),
                        "effects": NBT.TagCompound({
                            "sky_color": NBT.TagInt(7907327),
                            "water_fog_color": NBT.TagInt(329011),
                            "fog_color": NBT.TagInt(12638463),
                            "mood_sound": NBT.TagCompound({
                                "tick_delay": NBT.TagInt(6000),
                                "offset": NBT.TagDouble(2.0),
                                "sound": NBT.TagString("minecraft:ambient.cave"),
                                "block_search_extent": NBT.TagInt(8),
                            })
                        }),
                        "depth": NBT.TagFloat(0.125),
                        "temperature": NBT.TagFloat(0.8),
                        "scale": NBT.TagFloat(0.05),
                        "downfall": NBT.TagFloat(0.4),
                        "category": NBT.TagString("plains"),
                    }),
                }),
            ]),
        }),
    }
)

if __name__ == "__main__":
    NBT.write(data, "dimension_codec.nbt")
