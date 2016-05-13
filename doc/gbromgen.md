gbromgen
========

gbromgen is an auxiliary post-processing tool for libstdgb meant to make it
easier to organize a Game Boy ROM image. It takes input from a simple
JSON-encoded specialization and outputs a runnable binary image created by
altering the output of SDCC's `makebin` program.

It requires SDCC (for `makebin`) and Python 3 to run.
