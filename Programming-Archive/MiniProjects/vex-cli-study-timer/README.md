# Vex CLI Study Timer ⏱

A simple command-line study timer written in C for Linux.

I built this project to practice working with CLI arguments, project structure, and Linux system calls.

The timer uses a custom argument parser and calls the Linux `nanosleep` syscall directly using inline assembly instead of using standard sleep functions.

## Features

- Custom CLI argument parser
- Study/break session management
- Direct Linux syscall usage
- Modular code structure

## Project Structure

```

vex-cli-study-timer/
├── README.md
├── sounds/
│   ├── Alarm.wav
│   ├── tone.c
│   └── tone.h
└── src/
    ├── main.c
    ├── cli/
    │   ├── cli.c
    │   └── cli.h
    └── timer/
        ├── alarm.c
        ├── alarm.h
        ├── internals/
        ├── print.c
        ├── print.h
        ├── timer.c
        └── timer.h
````

## Build

```bash
make
````

or

```bash
gcc src/main.c src/cli/cli.c src/timer/timer.c src/timer/print.c src/timer/alarm.c sounds/tone.c -lm -o vex-timer
````

## Usage

```bash
./vex-timer -s 4 -t 25 -b 5
```

Example:

```
4 study sessions
25 minutes study time
5 minutes break time
```

## Notes

This project was mainly created as a learning exercise to understand how command-line programs interact with Linux at a lower level.

---

Built by E-Vex

