CC = gcc

CFLAGS = -Wall -Wextra -std=c11 -I.

LDFLAGS = -lm

SRC = \
src/main.c \
src/cli/cli.c \
src/timer/timer.c \
src/timer/print.c \
src/timer/alarm.c \
sounds/tone.c

TARGET = vex-timer

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)