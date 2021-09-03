#!/usr/bin/env python3

import nbt as NBT

tag = NBT.RootCompound(
    name="root",
    value={
        "byte": NBT.TagByte(16),
        "short": NBT.TagShort(1234),
        "int": NBT.TagInt(123456789),
        "long": NBT.TagLong(12345678900),
        "float": NBT.TagFloat(123.456),
        "double": NBT.TagDouble(123.4567),
        "bytearray": NBT.TagByteArray(bytes([0xde, 0xad, 0xc0, 0xde])),
        "string": NBT.TagString("Hello, world!"),
        "list": NBT.TagList([
            NBT.TagString("This is a string!1"),
            NBT.TagString("This is a string!2"),
            NBT.TagString("Not the same len"),
        ]),
        "compound": NBT.TagCompound({
            "byte": NBT.TagByte(16),
            "short": NBT.TagShort(1234),
            "int": NBT.TagInt(123456789),
            "long": NBT.TagLong(12345678900),
            "float": NBT.TagFloat(123.456),
            "double": NBT.TagDouble(123.4567),
            "bytearray": NBT.TagByteArray(bytes([0xde, 0xad, 0xc0, 0xde])),
            "string": NBT.TagString("Hello, world!"),
            "list": NBT.TagList([
                NBT.TagString("This is a string!1"),
                NBT.TagString("This is a string!2"),
                NBT.TagString("Not the same len"),
            ]),
            "compound": NBT.TagCompound({
                "test-string": NBT.TagString("this is another test!!")
            })
        }),
        "int_array": NBT.TagIntArray([1, 2, 3, 4, 123456789]),
        "long_array": NBT.TagLongArray([1, 2, 3, 4, 123456789, 123456789000]),
    }
)

if __name__ == "__main__":
    NBT.write(tag, "nbttest.nbt")
