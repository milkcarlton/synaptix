# Synaptix

## About
This is a fairly small configurable Linux application meant to bind input device events to key-based macros. 
I wrote this as a personal substitute for Razer Synapse (Synapse + Linux => Synaptix) to work with my ancient Razer Taipan mouse, but it should work with any input device. I open-sourced this project in case anyone else goes looking for something similar. It's far from perfect, but it does most of what I desired as-is. I'll still be renovating and developing it further in my free time though, so feel free to make suggestions or use it however you wish!

## Getting Started
1. Ensure the uinput module is loaded (lsmod | grep uinput)
1. Add configuration files to (~/.config/synaptix)
1. Run with 'sudo -E synaptix [arguments]'

*More detailed instructions will be provided at a later date as the project matures
