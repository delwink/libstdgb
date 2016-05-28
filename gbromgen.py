#! /usr/bin/env python3
##
##  gbromgen - generate full ROM images for the Game Boy
##  Copyright (C) 2016 Delwink, LLC
##
##  This program is free software: you can redistribute it and/or modify
##  it under the terms of the GNU Affero General Public License as published by
##  the Free Software Foundation, version 3 only.
##
##  This program is distributed in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##  GNU Affero General Public License for more details.
##
##  You should have received a copy of the GNU Affero General Public License
##  along with this program.  If not, see <http://www.gnu.org/licenses/>.
##

from argparse import Action, ArgumentParser
from json import load
from os import getpid, makedirs, remove
from os.path import abspath, exists, join
from shutil import copy, rmtree
from subprocess import call
from sys import argv, stderr, stdin
from tempfile import gettempdir

__title__ = 'gbromgen'
__version__ = '0.0.0'
__author__ = 'David McMackins II'
__version_info__ = '''{} {}
Copyright (C) 2016 Delwink, LLC
License AGPLv3: GNU AGPL version 3 only <http://gnu.org/licenses/agpl.html>.
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

def get_mbc_type(mbc_str):
    specs = [s.lower() for s in mbc_str.split('+')]
    specs.sort()
    return VALID_CART_TYPES['+'.join(specs)]

def warn(*args):
    print(__title__ + ': warning:', *args, file=stderr)

def clean_gb():
    try:
        if exists(gb_path):
            remove(gb_path)
    except NameError:
        pass

def fail(*args):
    clean_gb()
    exit(__title__ + ': error:', *args, file=stderr)

def print_bank_info(n, written, verbose=False):
    if verbose:
        s = 'rom bank {} uses {} bytes ({}%)'
        s = s.format(n, written, format((written / ROMBANK_SIZE) * 100, '.1f'))
        print(__title__ + ':', s)

def flush_bank(outfile, written):
    while written < ROMBANK_SIZE:
        # This block fills the unused parts of the ROM bank with failure code.
        # It fills all spaces of 2 bytes with a STOP command, which will kill
        # the device. Ideally, there are an even number of bytes so that all
        # slack space is taken up by this command. In the event of an odd
        # number, a RET command is inserted in attempt to return execution to
        # some legitimate position in the game code. This way, the chance of an
        # accidental jump which causes data corruption is minimized. Better to
        # completely halt execution than to corrupt a save file.
        if written != ROMBANK_SIZE - 1:
            outfile.write(b'\x10\x00')
            written += 2
        else:
            outfile.write(b'\xC9')
            written += 1

    return written

def write_bank(n, outfile, verbose=False):
    size = 0

    # TODO: determine write schedule and actually write

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

    RELATIVE_PATH = abspath(args.spec)

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
                if size >= ROMBANK_SIZE * 2:
                    fail('ROM size exceeds 32k')

                while size < ROMBANK_SIZE * 2:
                    outfile.write(b'\xFF')
                    size += 1

                exit(0)
            elif size >= ROMBANK_SIZE:
                fail('Game code exceeds space in ROM bank 0')

            flush_bank(outfile, size)

            banks_written = 1
            while banks_written < spec['rom-banks']:
                write_bank(banks_written, outfile, args.verbose)
                banks_written += 1
    except Exception as e:
        clean_gb()
        raise
    finally:
        rmtree(TEMPDIR)

if __name__ == '__main__':
    main()
