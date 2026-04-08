CC      = gcc
CFLAGS  = -std=c17 -Wall -Wextra -Wpedantic -O2
LDFLAGS = -lm

TARGET  = exoplanet-orbit-engine

SRCS =  main.c \
        core/physics/kepler.c \
        core/physics/energy.c \
        core/physics/temperature.c \
        core/physics/planck.c \
        core/physics/tidal.c \
        core/physics/habitability.c \
        core/physics/stellar.c \
        simulation/orbit.c \
        simulation/report.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
