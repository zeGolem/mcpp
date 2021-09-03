#!/usr/bin/env python3

import struct
import os.path


class Tag():
    @property
    def type_id(self) -> int:
        return -1

    def serialize(self) -> bytes:
        return bytes()


class TagEnd(Tag):
    def __init__(self) -> None:
        return

    @property
    def type_id(self) -> int:
        return 0

    def serialize(self) -> bytes:
        return (0).to_bytes(1, 'big')


class TagByte(Tag):
    def __init__(self, value: int) -> None:
        if value > 127 or value < -128:
            raise ValueError()
        self.value = value

    @property
    def type_id(self) -> int:
        return 1

    def serialize(self) -> bytes:
        return self.value.to_bytes(1, 'big')


class TagShort(Tag):
    def __init__(self, value: int) -> None:
        if value > 32_767 or value < -32_768:
            raise ValueError()
        self.value = value

    @property
    def type_id(self) -> int:
        return 2

    def serialize(self) -> bytes:
        return self.value.to_bytes(2, 'big')


class TagInt(Tag):
    def __init__(self, value: int) -> None:
        if value > 2_147_483_647 or value < -2_147_483_648:
            raise ValueError()
        self.value = value

    @property
    def type_id(self) -> int:
        return 3

    def serialize(self) -> bytes:
        return self.value.to_bytes(4, 'big')


class TagLong(Tag):
    def __init__(self, value: int) -> None:
        self.value = value

    @property
    def type_id(self) -> int:
        return 4

    def serialize(self) -> bytes:
        return self.value.to_bytes(8, 'big')


class TagFloat(Tag):
    def __init__(self, value: float) -> None:
        self.value = value

    @property
    def type_id(self) -> int:
        return 5

    def serialize(self) -> bytes:
        return struct.pack(">f", self.value)


class TagDouble(Tag):
    def __init__(self, value: float) -> None:
        self.value = value

    @property
    def type_id(self) -> int:
        return 6

    def serialize(self) -> bytes:
        return struct.pack(">d", self.value)


class TagByteArray(Tag):
    def __init__(self, value: bytes) -> None:
        self.value = value

    @property
    def type_id(self) -> int:
        return 7

    def serialize(self) -> bytes:
        return TagInt(len(self.value)).serialize() + self.value


class TagString(Tag):
    def __init__(self, value: str) -> None:
        self.value = value

    @property
    def type_id(self) -> int:
        return 8

    def serialize(self) -> bytes:
        header = len(self.value).to_bytes(2, 'big')
        return header + bytes(self.value, 'utf-8')


class TagList(Tag):
    def __init__(self, value: list[Tag]) -> None:
        self.value = value

    @property
    def type_id(self) -> int:
        return 9

    def serialize(self) -> bytes:
        headers = self.value[0].type_id.to_bytes(1, 'big')
        headers += TagInt(len(self.value)).serialize()
        data = bytes()
        for tag in self.value:
            data += tag.serialize()
        return headers + data


class TagCompound(Tag):
    def __init__(self, value: dict[str, Tag]) -> None:
        self.value = value

    @property
    def type_id(self) -> int:
        return 10

    def serialize(self) -> bytes:
        data = bytes()
        for tag_name in self.value:
            tag = self.value[tag_name]
            data += tag.type_id.to_bytes(1, 'big')
            data += TagString(tag_name).serialize()
            data += tag.serialize()

        return data + TagEnd().serialize()


class TagIntArray(Tag):
    def __init__(self, value: list[int]) -> None:
        self.value = value

    @property
    def type_id(self) -> int:
        return 11

    def serialize(self) -> bytes:
        headers = TagInt(len(self.value)).serialize()
        data = bytes()
        for value in self.value:
            data += TagInt(value).serialize()
        return headers + data


class TagLongArray(Tag):
    def __init__(self, value: list[int]) -> None:
        self.value = value

    @property
    def type_id(self) -> int:
        return 12

    def serialize(self) -> bytes:
        headers = TagInt(len(self.value)).serialize()
        data = bytes()
        for value in self.value:
            data += TagLong(value).serialize()
        return headers + data


class RootCompound(TagCompound):
    def __init__(self, name: str, value: dict[str, Tag]) -> None:
        self.name = name
        super().__init__(value)

    def serialize(self) -> bytes:
        header = self.type_id.to_bytes(1, 'big')
        header += TagString(self.name).serialize()
        return header + super().serialize()


def write(root: RootCompound, out_file: str):
    path = os.path.join(os.path.dirname(__file__), '../out', out_file)
    with open(path, 'wb') as fp:
        fp.write(root.serialize())
