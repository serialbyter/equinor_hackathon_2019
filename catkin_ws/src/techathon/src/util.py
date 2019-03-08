#!/usr/bin/env python


def parse_map(msg):
    num_rows = msg.dim[0].size
    num_col = msg.dim[1].size
    world_map = bytearray(msg.data)
    world_map = [int(x) for x in world_map]
    print(world_map)

    return world_map
