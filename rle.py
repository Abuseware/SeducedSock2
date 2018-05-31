#!/usr/bin/env python

TERMINAL_WIDTH = 80
TERMINAL_HEIGHT = 25

import sys


def encode(input_string):
    count = 1
    prev = ''
    lst = []
    for character in input_string:
        if character != prev:
            if prev:
                entry = (prev, count)
                lst.append(entry)
                # print lst
            count = 1
            prev = character
        else:
            count += 1
    else:
        entry = (character, count)
        lst.append(entry)
    return lst


def decode(lst):
    q = ""
    for character, count in lst:
        q += character * count
    return q


if __name__ == "__main__":
    width = 0
    height = 0

    with open(sys.argv[1]) as rf:
        for line in rf:
            if len(line) > width:
                width = len(line)
            if line != '\n':
                height += 1

        rf.seek(0)

        x = int((TERMINAL_WIDTH / 2) - (width / 2))
        y = int((TERMINAL_HEIGHT / 2) - (height / 2))

        with open((sys.argv[1].split('.')[0] + '.h'), 'w') as wf:
            wf.write("unsigned int logo_x = %i;\n" %(x))
            wf.write("unsigned int logo_y = %i;\n" %(y))
            wf.write("unsigned int logo_w = %i;\n" %(width))
            wf.write("unsigned int logo_h = %i;\n" %(height))
            wf.write("unsigned char logo[%i][%i] = {\n" %(height, width))
            for line in rf:
                wf.write("\t{ ")
                for char in line:
                    if char == "\n": continue
                    wf.write("0x%X, " %(ord(char)))
                if len(line) < width:
                    for x in range(width - len(line)):
                        wf.write("0x20, ")
                wf.write("},\n")
            wf.write("};")
