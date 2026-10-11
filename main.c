#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    CELSIUS = 1,
    FAHRENHEIT,
    KELVIN
} TemperatureUnit;

static int read_line(const char *prompt, char *buffer, size_t size)
{
    size_t length;
    int ch;

    printf("%s", prompt);
    if (fgets(buffer, (int)size, stdin) == NULL) {
        return 0;
    }

    length = strlen(buffer);
    if (length > 0 && buffer[length - 1] == '\n') {
        buffer[length - 1] = '\0';
    } else {
        while ((ch = getchar()) != '\n' && ch != EOF) {
            /* Clear the rest of an overlong input line. */
        }
    }

    return 1;
}

static int read_integer(const char *prompt, int min, int max, int *result)
{
    char input[64];
    char *end;
    long value;

    while (read_line(prompt, input, sizeof(input))) {
        errno = 0;
        value = strtol(input, &end, 10);

        while (isspace((unsigned char)*end)) {
            end++;
        }

        if (errno != 0 || end == input || *end != '\0') {
            printf("Please enter a whole number.\n");
            continue;
        }

        if (value < min || value > max) {
            printf("Choose a number from %d to %d.\n", min, max);
            continue;
        }

        *result = (int)value;
        return 1;
    }

    return 0;
}

static const char *unit_name(TemperatureUnit unit)
{
    switch (unit) {
        case CELSIUS:
            return "Celsius";
        case FAHRENHEIT:
            return "Fahrenheit";
        case KELVIN:
            return "Kelvin";
        default:
            return "Unknown";
    }
}

static const char *unit_symbol(TemperatureUnit unit)
{
    switch (unit) {
        case CELSIUS:
            return "\u00b0C";
        case FAHRENHEIT:
            return "\u00b0F";
        case KELVIN:
            return "K";
        default:
            return "?";
    }
}

static double absolute_zero(TemperatureUnit unit)
{
    switch (unit) {
        case CELSIUS:
            return -273.15;
        case FAHRENHEIT:
            return -459.67;
        case KELVIN:
            return 0.0;
        default:
            return 0.0;
    }
}

static int read_temperature(TemperatureUnit unit, double *result)
{
    char input[128];
    char *end;
    double value;

    while (read_line("Temperature: ", input, sizeof(input))) {
        errno = 0;
        value = strtod(input, &end);

        while (isspace((unsigned char)*end)) {
            end++;
        }

        if (errno != 0 || end == input || *end != '\0' || !isfinite(value)) {
            printf("That doesn't look like a valid number. Try again.\n");
            continue;
        }

        if (value < absolute_zero(unit)) {
            printf("That is below absolute zero for %s. Try again.\n", unit_name(unit));
            continue;
        }

        *result = value;
        return 1;
    }

    return 0;
}

static double to_celsius(double value, TemperatureUnit unit)
{
    switch (unit) {
        case CELSIUS:
            return value;
        case FAHRENHEIT:
            return (value - 32.0) * 5.0 / 9.0;
        case KELVIN:
            return value - 273.15;
        default:
            return value;
    }
}

static double from_celsius(double value, TemperatureUnit unit)
{
    switch (unit) {
        case CELSIUS:
            return value;
        case FAHRENHEIT:
            return value * 9.0 / 5.0 + 32.0;
        case KELVIN:
            return value + 273.15;
        default:
            return value;
    }
}

static double convert_temperature(double value, TemperatureUnit from, TemperatureUnit to)
{
    if (from == to) {
        return value;
    }

    return from_celsius(to_celsius(value, from), to);
}

static void print_header(void)
{
    puts("\n----------------------------------");
    puts("       TEMPERATURE CONVERTER");
    puts("----------------------------------");
    puts("  Celsius  |  Fahrenheit  |  Kelvin\n");
}

static void print_units(void)
{
    puts("  1. Celsius (\u00b0C)");
    puts("  2. Fahrenheit (\u00b0F)");
    puts("  3. Kelvin (K)");
}

static int convert_flow(void)
{
    int from_choice;
    int to_choice;
    double value;
    double converted;
    TemperatureUnit from;
    TemperatureUnit to;

    puts("\nConvert from:");
    print_units();
    if (!read_integer("Choose a unit (1-3): ", 1, 3, &from_choice)) {
        return 0;
    }

    from = (TemperatureUnit)from_choice;
    printf("\nEnter a value in %s.\n", unit_name(from));
    if (!read_temperature(from, &value)) {
        return 0;
    }

    puts("\nConvert to:");
    print_units();
    if (!read_integer("Choose a unit (1-3): ", 1, 3, &to_choice)) {
        return 0;
    }

    to = (TemperatureUnit)to_choice;
    converted = convert_temperature(value, from, to);

    puts("\n----------------------------------");
    printf("  %.2f %s  ->  %.2f %s\n", value, unit_symbol(from),
           converted, unit_symbol(to));
    puts("----------------------------------\n");

    return 1;
}

int main(void)
{
    int choice;

    print_header();

    for (;;) {
        puts("  1. Convert temperature");
        puts("  0. Exit\n");

        if (!read_integer("Your choice: ", 0, 1, &choice)) {
            break;
        }

        if (choice == 0) {
            break;
        }

        if (!convert_flow()) {
            break;
        }
    }

    puts("\nSee you next time!\n");
    return 0;
}
