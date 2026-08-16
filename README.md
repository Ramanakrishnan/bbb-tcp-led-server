# BeagleBone Black — Remote LED Control Server

A simple TCP server written in C that runs on a BeagleBone Black and lets a
remote client toggle the onboard USR0 LED over the network.

## What it does

- Listens on TCP port `5000`
- Accepts a connection from a client
- Reads a single byte (`"1"` or `"0"`) sent by the client
- Writes that value to `/sys/class/leds/beaglebone:green:usr0/brightness`,
  turning the LED on or off accordingly
- Loops to accept the next connection

## Hardware / OS

- Board: BeagleBone Black (Rev D)
- OS: Debian 12 IoT image (official beagleboard.org release), booted from SD card
- LED controlled: onboard USR0 (green), via the Linux sysfs LED class interface

## Build

```bash
gcc led_server.c -o led_server
```

## Run

Root is required since writing to `/sys/class/leds/...` needs elevated
permissions:

```bash
sudo ./led_server
```

The board's default trigger for USR0 (`heartbeat`) will override manual
writes unless disabled first:

```bash
echo none | sudo tee /sys/class/leds/beaglebone:green:usr0/trigger
```

## Test it

From another machine on the same network as the board (e.g. over the
board's default USB gadget network at `192.168.7.2`):

```bash
echo -n "1" | nc 192.168.7.2 5000   # LED ON
echo -n "0" | nc 192.168.7.2 5000   # LED OFF
```

## What I learned building this

- Socket programming fundamentals in C: `socket()`, `bind()`, `listen()`,
  `accept()`, `recv()`
- Controlling hardware from userspace via the Linux sysfs interface
- Buffered I/O gotcha: `fprintf()` writes aren't guaranteed to reach the
  file until it's flushed/closed — forgetting `fclose()` silently dropped
  writes to the LED file
- LED sysfs `trigger` files can override manual brightness writes if a
  kernel-driven trigger (like `heartbeat`) is still active
