#! /usr/bin/env python3
##
##  gbromgen - generate full ROM images for the Game Boy
##  Copyright (C) 2016 Delwink, LLC
##
##  Redistributions, modified or unmodified, in whole or in part, must retain
##  applicable notices of copyright or other legal privilege, these conditions,
##  and the following license terms and disclaimer.  Subject to these
##  conditions, each holder of copyright or other legal privileges, author or
##  assembler, and contributor of this work, henceforth "licensor", hereby
##  grants to any person who obtains a copy of this work in any form:
##
##  1. Permission to reproduce, modify, distribute, publish, sell, sublicense,
##  use, and/or otherwise deal in the licensed material without restriction.
##
##  2. A perpetual, worldwide, non-exclusive, royalty-free, gratis, irrevocable
##  patent license to make, have made, provide, transfer, import, use, and/or
##  otherwise deal in the licensed material without restriction, for any and
##  all patents held by such licensor and necessarily infringed by the form of
##  the work upon distribution of that licensor's contribution to the work
##  under the terms of this license.
##
##  NO WARRANTY OF ANY KIND IS IMPLIED BY, OR SHOULD BE INFERRED FROM, THIS
##  LICENSE OR THE ACT OF DISTRIBUTION UNDER THE TERMS OF THIS LICENSE,
##  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR
##  A PARTICULAR PURPOSE, AND NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS,
##  ASSEMBLERS, OR HOLDERS OF COPYRIGHT OR OTHER LEGAL PRIVILEGE BE LIABLE FOR
##  ANY CLAIM, DAMAGES, OR OTHER LIABILITY, WHETHER IN ACTION OF CONTRACT,
##  TORT, OR OTHERWISE ARISING FROM, OUT OF, OR IN CONNECTION WITH THE WORK OR
##  THE USE OF OR OTHER DEALINGS IN THE WORK.
##

from argparse import Action, ArgumentParser
from gbimg import Tileset
from json import load
from os import getpid, makedirs, remove
from os.path import abspath, dirname, exists, join
from shutil import copy, rmtree
from subprocess import call
from sys import argv, stderr, stdin
from tempfile import gettempdir

__title__ = 'gbromgen'
__version__ = '0.0.0'
__author__ = 'David McMackins II'
__version_info__ = '''{} {}
Copyright (C) 2016 Delwink, LLC
License COIL 0.6: Copyfree Open Innovation License v0.6
<http://copyfree.org/content/standard/licenses/coil/license.txt>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.

Written by {}'''.format(__title__, __version__, __author__)

VALID_CART_TYPES = {
    'rom': 0x00,

    'mbc1': 0x01,
    'mbc1+ram': 0x02,
    'batt+mbc1+ram': 0x03,

    'mbc2': 0x05,
    'batt+mbc2': 0x06,

    'ram': 0x08,
    'batt+ram': 0x09,

    'mmm01': 0x0B,
    'mmm01+sram': 0x0C,
    'batt+mmm01+sram': 0x0D,

    'batt+mbc3+timer': 0x0F,
    'batt+mbc3+ram+timer': 0x10,
    'mbc3': 0x11,
    'mbc3+ram': 0x12,
    'batt+mbc3+ram': 0x13,

    'mbc5': 0x19,
    'mbc5+ram': 0x1A,
    'batt+mbc5+ram': 0x1B,
    'mbc5+rumble': 0x1C,
    'mbc5+rumble+sram': 0x1D,
    'batt+mbc5+rumble+sram': 0x1E
}

ROM_BANKS = (2, 4, 8, 16, 32, 64, 128)
RAM_BANKS = (0, -1, 1, 4, 16)
ROMBANK_SIZE = 1024 * 16

CART_TYPE_ADDR = 0x147
COMP_ADDR = 0x14D
ROMBANKS_ADDR = 0x148
RAMBANKS_ADDR = 0x149
VBLANK_ADDR = 0x40
SWITCHABLE_ROM_ADDR = 0x4000

class UnsignedIntegerField:
    def __init__(self, value, size):
        self._value = int(value)
        self._size = int(size)

    def __bytes__(self):
        b = []
        for i in range(self._size):
            shift = i * 8
            mask = 0xFF << shift
            b.append((self._value & mask) >> shift)

        return bytes(b)

class VersionAction(Action):
    def __call__(self, parser, values, namespace, option_string):
        print(__version_info__)
        exit(0)

CLI = ArgumentParser(__title__)

# script meta
CLI.add_argument('-v', '--verbose', action='store_true',
                 help='Show more details')
CLI.add_argument('--version', action=VersionAction, nargs=0,
                 help='Show version information and exit')

# files
CLI.add_argument('spec', type=str,
                 help='A specification for the ROM image')

_num_runs = 0

def jp(addr, f):
    addr = (addr & 0xFF, (addr & 0xFF00) >> 8)
    f.write(b'\xC3') # jp instruction
    f.write(bytes(addr))

def reti(f):
    f.write(b'\xD9') # reti instruction

def get_complement(total):
    total += 25 # because reasons
    return (0x100 - (total & 0xFF)) % 0x100

def get_symbol_pos(noi_path, s):
    with open(noi_path) as noi:
        for line in noi:
            line = line.split()
            if line[0] == 'DEF' and line[1] == '_' + s:
                return int(line[2], 16)

    return None

def add_field(fields, field, pos):
    b = bytes(field)
    for i in range(len(b)):
        fields[pos + i] = b[i]

def get_mbc_type(mbc_str):
    specs = [s.lower() for s in mbc_str.split('+')]
    specs.sort()
    return VALID_CART_TYPES['+'.join(specs)]

def warn(*args):
    print(__title__ + ': warning: ', *args, file=stderr)

def fail(*args, rm=None):
    if rm and exists(rm):
        remove(rm)

    exit(__title__ + ': error: ' + ' '.join(args))

def print_bank_info(n, written, verbose=False):
    if verbose:
        s = 'rom bank {} uses {} bytes ({}%)'
        s = s.format(n, written, format((written / ROMBANK_SIZE) * 100, '.1f'))
        print(__title__ + ':', s)

def flush_bank(outfile, written):
    if (ROMBANK_SIZE - written) % 2 != 0:
        outfile.write(b'\x00')
        written += 1

    while written < ROMBANK_SIZE:
        # This block fills the unused parts of the ROM bank with failure code.
        # It fills all spaces of 2 bytes with a STOP command.
        outfile.write(b'\x10\x00')
        written += 2

    return written

def write_bank(n, outfile, data, verbose=False):
    size = 0

    for d in data:
        outfile.write(d.data)
        size += len(d.data)

    print_bank_info(n, size, verbose)
    flush_bank(outfile, size)

def main(args=argv[1:]):
    global _num_runs

    PID = str(getpid())
    STAMP = PID + '_' + str(_num_runs)
    _num_runs += 1

    TEMPDIR = join(gettempdir(), 'gbromgen-' + STAMP)

    args = CLI.parse_args(args)

    if args.spec != '-':
        with open(args.spec) as f:
            spec = load(f)
    else:
        spec = load(stdin)

    if type(spec) is not dict:
        fail('Input specification syntax error')

    RELATIVE_PATH = dirname(abspath(args.spec))

    if 'hex' not in spec:
        fail("'hex' file not specified in input specification")

    hex_path = join(RELATIVE_PATH, spec['hex'])
    if not hex_path.endswith('.ihx'):
        warn('Input file does not have Intel hex standard extension')
        base_file_name = hex_path
    else:
        base_file_name = hex_path[:-4]

    noi_path = base_file_name + '.noi'
    gb_path = spec['output'] if spec['output'] else base_file_name + '.gb'

    vblank = None
    if 'vblank' in spec:
        vblank = get_symbol_pos(noi_path, spec['vblank'])
        if vblank is None:
            warn('Could not locate vblank trigger function')

    fields = {}
    if 'const-fields' in spec:
        encoded = {}
        fields = spec['const-fields']
        for field in fields:
            name, value = (field, fields[field])
            pos = get_symbol_pos(noi_path, name)
            if pos is None:
                warn('Could not locate field {}'.format(name))
                continue

            value = int(value)
            if value < 0:
                warn('Negative const field not supported: {}'.format(value))
                continue

            byte = (value.bit_length() + 7) // 8

            byte_list = []
            for i in range(byte):
                shift = i * 8
                mask = 0xFF << shift
                byte_list.append((value & mask) >> shift)

            for i in range(len(byte_list)):
                encoded[pos + i] = byte_list[i]

        fields = encoded

    if spec['mbc']:
        mbc = get_mbc_type(spec['mbc'])
    else:
        mbc = get_mbc_type('rom')

    makedirs(TEMPDIR)

    if 'name' not in spec:
        spec['name'] = ''

    if 'rom-banks' not in spec:
        spec['rom-banks'] = 2

    if 'ram-banks' not in spec:
        spec['ram-banks'] = 0

    bank_data = []
    for _ in range(spec['rom-banks'] - 1):
        bank_data.append([])

    unalloc_data = []

    if 'tilesets' in spec:
        for tileset in spec['tilesets']:
            if 'bank' not in tileset:
                tileset['bank'] = None

            tileset = Tileset(tileset['img'], tileset['data'], tileset['bank'])
            if tileset.bank_num:
                bank_data[tileset.bank_num - 1].append(tileset)
            else:
                unalloc_data.append(tileset)

    for ud in unalloc_data:
        ulen = len(ud)
        allocated = False

        for i in range(len(bank_data)):
            b = bank_data[i]
            blen = 0
            for d in b:
                blen += len(d)

            if ROMBANK_SIZE - blen >= ulen:
                ud.set_bank_num(i + 1)
                b.append(ud)
                allocated = True
                break

        if not allocated:
            fail('No ROM space for ' + str(ud))

        del ud

    for b in bank_data:
        pointer = 0

        for d in b:
            pos = get_symbol_pos(noi_path, d.data_var)
            if pos:
                field = UnsignedIntegerField(SWITCHABLE_ROM_ADDR + pointer, 2)
                add_field(fields, field, pos)
            else:
                warn('Could not locate field {}'.format(d.data_var))

            if d.bank_var:
                pos = get_symbol_pos(noi_path, d.bank_var)
                if pos:
                    field = UnsignedIntegerField(d.bank_num, 1)
                    add_field(fields, field, pos)
                else:
                    warn('Could not locate field {}'.format(d.data_var))

            pointer += len(d)

    try:
        temp_gb = join(TEMPDIR, 'temp.gb')
        rc = call(['makebin', '-Z', '-p', '-yn', spec['name'], spec['hex'],
                   temp_gb])
        if rc:
            raise RuntimeError('makebin failed with rc={}'.format(rc))

        size = 0
        with open(temp_gb, 'rb') as infile, open(gb_path, 'wb') as outfile:
            comp_sum = 0
            c = infile.read(1)
            while c:
                if size == VBLANK_ADDR:
                    if vblank:
                        jp(vblank, outfile)
                        infile.read(2)
                        size += 3
                    else:
                        reti(outfile)
                        size += 1

                    c = infile.read(1)
                    continue

                if size == CART_TYPE_ADDR:
                    byte = mbc
                elif size == ROMBANKS_ADDR:
                    byte = ROM_BANKS.index(spec['rom-banks'])
                elif size == RAMBANKS_ADDR:
                    byte = RAM_BANKS.index(spec['ram-banks'])
                elif size == COMP_ADDR:
                    byte = get_complement(comp_sum)
                elif size in fields:
                    byte = fields[size]
                else:
                    byte = c[0]

                outfile.write(bytes([byte]))

                if 0x134 <= size < COMP_ADDR:
                    comp_sum += byte

                size += 1
                c = infile.read(1)

            print_bank_info(0, size, args.verbose)

            if mbc == 0x00:
                if size > ROMBANK_SIZE * 2:
                    fail('ROM size exceeds 32k', rm=gb_path)

                if size < ROMBANK_SIZE:
                    flush_bank(outfile, size)
                    flush_bank(outfile, 0)
                else:
                    flush_bank(outfile, size - ROMBANK_SIZE)

                exit(0)

            if size > ROMBANK_SIZE:
                fail('Game code exceeds space in ROM bank 0', rm=gb_path)

            flush_bank(outfile, size)

            for i in range(spec['rom-banks'] - 1):
                write_bank(i + 1, outfile, bank_data[i], args.verbose)
    except Exception as e:
        if exists(gb_path):
            remove(gb_path)
        raise
    finally:
        rmtree(TEMPDIR)

if __name__ == '__main__':
    main()
