gbromgen
========

gbromgen is an auxiliary post-processing tool for libstdgb meant to make it
easier to organize a Game Boy ROM image. It takes input from a simple
JSON-encoded specification and outputs a runnable binary image created by
altering the output of SDCC's `makebin` program.

It requires SDCC (for `makebin`) and Python 3 to run.

Usage
-----

    $ gbromgen [options] spec

`spec` is the specification file. Use the option `-h` or `--help` for
information about the command line arguments.

Specification File
------------------

The spec file is encoded as a JSON object. All fields of this object are
optional except for `hex`, which is the code for your program (in `ihx`
format).

### Basic configuration

A simple spec file might resemble the following:

``` json
{
    "name": "HELLO",
    "hex": "hello.ihx",
    "output": "hello.gb"
}
```

The `name` field is a short name to be stored on the ROM. Some programs use it
to store the program's version information as well.

The `hex` field is a file path relative to the location of the spec file which
serves as the input for processing. This file will be passed to SDCC's
`makebin` to be converted to binary before `gbromgen` processes it
further. This hex file is usually generated by `sdcc` after finishing
compilation.

The `output` field is a file path relative to the location of the spec file
where the final executable ROM file will be placed.

### Header level fields

In addition to `name`, `hex`, and `output`, the following top-level fields may
be used:

- `const-fields`: (object) Definition of constant integer fields to be written
  into the ROM (see below).
- `mbc`: (string) Specification of the cartridge type (see below).
- `ram-banks`: (integer) The number of RAM banks on the cartridge.
- `rom-banks`: (integer) The number of ROM banks to be written.
- `tilesets`: (array of object) Defined tilesets for the ROM (see below).
- `vblank`: (string) Name of a function to be executed at the vblank interrupt.

### Constant integer fields

You can define constant integer fields in your C code and assign them values in
the spec. This includes pointers. This field in the spec is an object with
string keys and unsigned integer values, as such:

``` json
{
    "my_field": 120,
    "my_other_field": 0x30
}
```

### Cartridge types

If your cartridge is not a simple ROM, you may specify what hardware it has (as
is allowable by the Game Boy spec). The following combinations are valid:

- `mbc1`
- `mbc1+ram`
- `batt+mbc1+ram`
- `mbc2`
- `batt+mbc2`
- `ram`
- `batt+ram`
- `mmm01`
- `mmm01+sram`
- `batt+mmm01+sram`
- `batt+mbc3+timer`
- `batt+mbc3+ram+timer`
- `mbc3`
- `mbc3+ram`
- `batt+mbc3+ram`
- `mbc5`
- `mbc5+ram`
- `batt+mbc5+ram`
- `mbc5+rumble`
- `mbc5+rumble+sram`
- `batt+mbc5+rumble+sram`

### Tilesets

One of the most tedious part of Game Boy development is encoding your graphics
in the Game Boy format and embedding them in the binary. Tools have existed
which generate assembler and C code for embedding, but this poses some
problems:

1. SDCC cannot be told to store data in ROM banks; it is limited to 16k if
   using switchable banks and 32k if using a normal ROM.
2. The image you drew is not linked to the generated graphics. Every time you
   want to change it, you must regenerate the encoded binary and reinsert it
   into your code.
3. Your binary graphics data are now taking up space in your source code
   files. This not only inflates the disk footprint of your code base, it
   clutters your code with data that are basically meaningless when in the
   programming mindset of reading and interpreting source code.

gbromgen addresses these problems by allowing you to point to images containing
tilesets and associate the image with some variables in your code. Currently,
there are three fields that need to be defined in your spec for each tileset:

- `img`: (string) relative file path to the image file
- `data`: (string) the name of a `const uint8_t (* const)[GB_BYTES_PER_TILE]`
  in your code which will be set to wherever gbromgen puts the converted data.
- `bank`: (string or integer) If string, the name of a `const uint8_t` in your
  code which will be set to the bank number into which gbromgen puts your
  converted data. If integer, this forces gbromgen to assign this tileset to
  that bank number. If the bank number is 0 or more than the number of banks,
  it will be skipped entirely.

Your spec definition might look something like this:

``` json
{
    ...

    "tilesets": [
        {
            "img": "normal-tiles.png",
            "data": "NORMAL_TILES",
            "bank": 1
        },
        {
            "img": "dark-tiles.png",
            "data": "DARK_TILES",
            "bank": "DARK_TILES_BANK"
        }
    ]
}
```

In your code, you might put placeholders as such:

``` c
const uint8_t (* const NORMAL_TILES)[GB_BYTES_PER_TILE] = NULL;
const uint8_t (* const DARK_TILES)[GB_BYTES_PER_TILE] = NULL;
const uint8_t NORMAL_TILES_BANK = 1;
const uint8_t DARK_TILES_BANK = 0;
```
