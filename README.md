# Synaptix

## About
This is a fairly small configurable Linux application meant to bind input device events to key-based macros. 
I wrote this as a personal substitute for Razer Synapse (Synapse + Linux => Synaptix) to work with my ancient Razer Taipan mouse, but it should work with any input device. I open-sourced this project in case anyone else goes looking for something similar. It's far from perfect, but it does most of what I desired as-is. I'll still be renovating and developing it further in my free time though, so feel free to make suggestions or use it however you wish!

## Using Synaptix
1. Ensure the uinput module is loaded (lsmod | grep uinput)
    * If the module is not found, then load it (sudo modprobe uinput)
1. Build Synaptix
    * Create a 'bin' directory, and run 'cmake CMakeLists.txt'
1. Add desired configuration files to (~/.config/synaptix)
    * Example configurations are included in this repository
1. Run './bin/synaptix [arguments]'
    * ie, './bin/synaptix -l' to load the main configuration

*More detailed instructions will be provided at a later date as the project matures
