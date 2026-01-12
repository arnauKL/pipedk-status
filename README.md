# `pipedk`: Minimal Status Bar for Linux

**`pipedk`** is a custom status bar program designed for [`dwl`](https://codeberg.org/dwl/dwl) (and other window managers) to minimise resource usage and learn C on linux.

## Motivation

I started this project because [`slstatus`](https://tools.suckless.org/slstatus/) wasn't displaying audio information properly on my setup, and [`dwmblocks`](https://github.com/torrinfail/dwmblocks) felt unnecessarily complex for what I needed. 

Initially, I planned to use `inotify` for real-time file watching, but learned that `/sys` filesystem files are virtual and don't trigger inotify events. This sadly led me to a polling approach.

## Performance vs slstatus

| Metric | pipedk | slstatus | Improvement |
|:--------|:--------|:----------|:-------------|
| CPU Time | 18.53ms | 30.54ms | 39% faster |
| CPU Cycles | 899K | 983K | 9% fewer |
| Instructions | 389K | 401K | 3% fewer |
| Memory Usage | ~1.6MB RSS | ~1.8MB RSS | 10% less |

>Benchmarked using `perf stat` over 60-second intervals

## Features

- **Zero dynamic allocation**: Static buffers.
- **In-place string updates**: Super janky but works, avoids string concatenation and extra buffers.
- **Extensibility**: Bare-bones module system.

## Installation and usage

```bash
git clone https://github.com/arnauKL/pipedk-status.git
cd pipedk-status
make
sudo make install  # Optional: installs to /usr/local/bin
```

`pipedk` outputs status information to `stdout`, designed to be piped into a wm:
```bash
# For dwl (requires dwl-bar patch)
pipedk | dwl
```
