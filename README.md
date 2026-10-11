# Temperature Converter (C)

A small terminal app I made to practice C. Pick a starting unit, enter a temperature, and convert it to another unit.

### Features
- Convert between Celsius, Fahrenheit, and Kelvin
- Basic input checks (including temperatures below absolute zero)
- Simple menu that runs in the terminal

### Run it

With `make`:

```sh
make
./temp-converter
```

Or compile it directly with Clang or GCC:

```sh
cc -std=c11 -Wall -Wextra -Wpedantic main.c -o temp-converter
./temp-converter
```

Enter `0` from the main menu to exit.
