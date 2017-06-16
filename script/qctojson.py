#!/usr/bin/env python3
import json
import argparse
from distutils.util import strtobool

datatyperange = ('b', 'i', 'd', 's', 'S')
datamodelrange = ('s', 'v', 'b')

# helper
def loadtext(fname):
    """load text from file, remove comments and blank
    """
    with open(fname, 'r') as f:
        rawtext = f.read().split('\n')
    # remove comment & blank
    cmt = '#'
    rst = list()
    for line in rawtext:
        line = line.split(cmt)[0].strip()
        if line:
            rst.append(line)
    return tuple(rst)


def parse_config_file(cfgfile):
    """parse config file to get data structure
    """
    text = loadtext(cfgfile)
    data = dict()
    N = len(text)
    index = 0
    while index < N:
        line = tuple(i.strip() for i in text[index].split())
        if line[0].startswith('$'):
            tag = line[0][1:].lower()
            data[tag] = dict()
            index += 1
            while True:
                line = tuple(i.strip() for i in text[index].split())
                if line[0].lower() == '$end':
                    index += 1
                    break
                else:
                    name = line[0].lower()
                    dtype = line[1]
                    dmodel = line[2]
                    assert dtype in datatyperange
                    assert dmodel in datamodelrange
                    data[tag][name] = {
                            "value" : None,
                            "datatype" : dtype,
                            "datamodel" : dmodel
                            }
                index += 1
    return data


def parse_infile(infile, data):
    """parse input file to get value

        param infile: input file
        param data: data skeleton from config file
    """
    text = loadtext(infile)
    N = len(text)
    index = 0
    while index < N:
        line = tuple(i.strip() for i in text[index].split())
        if line[0].startswith('$'):
            tag = line[0][1:].lower()
            nowdict = data[tag]
            index += 1
            while True:
                line = tuple(i.strip() for i in text[index].split())
                if line[0].lower() == '$end':
                    index += 1
                    break
                # block data
                elif line[0].startswith('@'):
                    name = line[0][1:].lower()
                    assert name in nowdict
                    index += 1
                    value = list()
                    while True:
                        line = tuple(i.strip() for i in text[index].split())
                        index += 1
                        if line[0].lower() == '@end':
                            break
                        # for bool
                        if nowdict[name]['datatype'] == 'b':
                            line = tuple('1' if strtobool(i) else '0' for i in line)
                        value.append(line)
                # line data
                else:
                    name = line[0].lower()
                    assert name in nowdict
                    line = tuple(i.strip() for i in text[index].split())
                    value = line[1:]
                    index += 1
                    # for bool
                    if nowdict[name]['datatype'] == 'b':
                        value = tuple('1' if strtobool(i) else '0' for i in value)
                nowdict[name]['value'] = value
    return data


# END
