# `pipedk`: Custom Status Bar

>Project in development

**`pipedk`** is a simple custom program to manage my status bar in [`dwl`](https://codeberg.org/dwl/dwl).

## Motivation

Audio information wasn't displaying properly with [`slstatus`](https://tools.suckless.org/slstatus/), and [`dwm-blocks`](https://github.com/torrinfail/dwmblocks) felt overly complex for my needs.
This project is also an excuse to learn about linux, signals and practice writing C.

## Usage

`pipedk` outputs status bar information to `stdout`. This output should be piped into `dwl` (requires the [`dwl-bar`](https://codeberg.org/dwl/dwl-patches/src/branch/main/patches/bar) patch).
It should also work with `dwm` as long as it reads status text from `stdout`.

```bash
pipedk | dwl
```

## Todos

- [x] Battery info
- [ ] Sound (volume) info
- [ ] CPU info
- [ ] Use file watchers instead of polling at fixed interval
