# `pipedk`: Minimal Status Bar for Linux

> A lightweight, performance-focused status bar implementation written in C

**`pipedk`** is a custom status bar program designed for [`dwl`](https://codeberg.org/dwl/dwl) (and other window managers) that prioritizes simplicity, performance, and minimal resource usage.

## Motivation

I started this project because [`slstatus`](https://tools.suckless.org/slstatus/) wasn't displaying audio information properly on my setup, and [`dwmblocks`](https://github.com/torrinfail/dwmblocks) felt unnecessarily complex for what I needed. 

Rather than patching existing tools, I decided to build something from scratch, both to solve my problem and as an opportunity to dive deeper into C programming and Linux systems.

Initially, I planned to use `inotify` for real-time file watching, but learned that `/sys` filesystem files are virtual and don't trigger inotify events. This limitation sadly led me to a polling approach.

## Performance vs slstatus

| Metric | pipedk | slstatus | Improvement |
|:--------|:--------|:----------|:-------------|
| CPU Time | 18.53ms | 30.54ms | 39% faster |
| CPU Cycles | 899K | 983K | 9% fewer |
| Instructions | 389K | 401K | 3% fewer |
| Memory Usage | ~1.6MB RSS | ~1.8MB RSS | 10% less |

*Benchmarked using `perf stat` over 60-second intervals*

## Features

- **Zero dynamic allocation**: Static buffers for maximum efficiency
- **In-place string updates**: Direct memory manipulation instead of string concatenation
- **Cache-friendly**: Contiguous memory layout for optimal CPU cache usage
- **Extensibility**: Module system.

## Usage

`pipedk` outputs status information to `stdout`, designed to be piped into a wm:

```bash
# For dwl (requires dwl-bar patch)
pipedk | dwl
```


## Installation

```bash
git clone https://github.com/arnauKL/pipedk-status.git pipedk
cd pipedk
make
sudo make install  # Optional: installs to /usr/local/bin
```

## Technical Notes

Initially planned to use `inotify` for real-time updates, but discovered that `/sys` filesystem files are virtual and don't trigger inotify events. This led to the current polling-based approach using a pre-defined template string:

`pipedk` uses in-place string updates:

```c
// Pre-allocated status string
static char status_bar[] = "00.00W | BAT0: 00% | 00/00 - 00:00";

// Modules point directly into the string
static struct module_ptr modules[] = {
    {status_bar + 0,  5, update_power},    // "00.00W"
    {status_bar + 15, 2, update_battery},  // "00%"
    // ...
};
```

This avoids:
- Dynamic memory allocation
- String concatenation operations
- Temporary buffers
- Complex formatting logic

## Learning Outcomes

- **Systems programming** in C
- **Linux filesystem interfaces** (`/sys`, `/proc`)
- **Performance optimization** techniques
- **Memory management** and cache efficiency
