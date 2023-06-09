# Synaptix

## About
Synaptix is a Linux application designed to bind device input events to other forms of input. 
I wrote this as a personal substitute for Razer Synapse (Synapse + Linux => Synaptix) to work with my ancient Razer Taipan mouse, but it should work with any input device. I am still developing this project in my free time, so leave any suggestions you have or use it however you wish!

## Overview
1. Ensure the uinput module is loaded (`lsmod | grep uinput`)
    * If the module is not found, then load it (`sudo modprobe uinput`)
1. Build Synaptix
    * Create a `bin` directory, and run `cmake CMakeLists.txt && make`
1. Add desired configuration files to (`~/.config/synaptix`)
    * Example configurations are included in this repository
1. Run the `synaptix` binary now located in the `bin` folder `./synaptix [arguments]`
    * Ex: `./bin/synaptix -l` to load the main configuration file

## Configuration
### Writing a macro file
1. Macro files should be placed in `~/.config/synaptix/macros`, and contain lines of three parts:
    * The name of the desired input (ie: `q`)
    * The state of the input (`1` to activate, `0` to release, or `2` to activate & release)
    * The delay in milliseconds until the next line of the macro is executed (ie: `16`)
    * Ex: `q 2   16`

1. The name of an input is derived from it's assignment in the list of [Linux UAPI input event codes](https://github.com/torvalds/linux/blob/master/include/uapi/linux/input-event-codes.h).
    * You can view the corresponding header file, or run `synaptix -g` to to have a list displayed in your terminal.
    * Keys have no prefix, while buttons are prefixed by `btn_` (ie: btn_middle)

### Binding a macro to an input device
1. Determine which event in `/dev/input` to use (ie, my keyboard: `/dev/input/event3`).
    * Symlinks under `by-id` may help make this more obvious.
    * Running `./synaptix -d [event path] -i (event type)` can also help by outputting event data to the terminal.
    * Note: event type should most often just be left as `1`, but other options can be found [here](https://github.com/torvalds/linux/blob/dbfa18c5d7695766f24c0c140204e1f8c921fb95/include/uapi/linux/input-event-codes.h#L39).

1. Use the command above to determine the desired activator code.
1. Add a line to `~/.config/synaptix/synaptix.conf` containing the following information:

    | Macro File    | Event Path        | Event Type | Code | Playback Mode |
    | ------------- | ----------------- | ---------- | ---- | ------------- |
    | MacroFile.do  | /dev/input/event3 | 1          | 30   | 2             |

    * Playback Mode:
        * 1 = play once and stop
        * 2 = repeat while held

1. Run `./synaptix -l`

### Recording a macro
1. Functionality is currently limited to recording only one device at a time.
1. Run `./synaptix -d [event path] -r (event type) -o (output file)` and follow the terminal instructions.
    * Ex: `./synaptix -d "/dev/input/event3" -r 1 -o "./recording.do"`
