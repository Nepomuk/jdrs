# JDRS - Jülich Digital Readout System

Developed in Forschungszentrum Jülich for testing ASIC under development. The main contributers are:
Simone Esch, André Goerres, Marius C. Mertens

This is the basic version of the readout system without specifications to a certain ASIC. Consider this README as a very basic documentation, a more in-depth PDF will be published when it's ready.


## Contents of the System

This system contains basically of three parts:
* Firmware (for the ML605 board)
* Software (that runs on the readout PC)
* MRF (a framework providing data handling functions)


### Firmware

This part contains the firmware, written in VHDL, that is running the FPGA on the development board. It can access to connected devices and transfer the gathered data over an Ethernet line using the UDP protocol.

### Software

The software is the counterpart on the PC side, connecting to the board and providing a GUI for easier use.

### MRF - MVD Readout Framework

Originally developed for testing components of PANDA's MVD, it doesn't have the restriction to work only there. It provides basic functions and classes to access the board, its registers, and through that also connected devices.


## To Do
There are some things that haven't been done so far. Following an unordered list for future developers.

* Make the single register transfer flexible in length (`ethernet_core_wrapper.vhd`).
  Right now it is fixed to 32 bits while the registers itself are variable.

* Make the MRF ready for variable word sizes for registers and bulk data.

* Include a sent package counter into the bulk replys and check for data loss within the MRF.

* Extend the bulk transfer to process larger requests by sending more than one reply package (`ethernet_core_wrapper.vhd` and MRF).
  Right now a request is truncated after reaching the maximum size that fit into a IP/UDP package. For performance reasons it would be beneficial to automatically send following packages until the requested word count is reached.
  **Note:** An additional `what_to_do` indicator should be used to mark an incomplete answer.

* A method to fill the DAQ fifo from the software. This can become handy for debugging the system and to simulate data from an ASIC without having the ASIC connected. Ideally this could handle a bulk transfer to the PC as well, but this is not really mandatory.

* Dublicate the register to handle two connected test devices (`register_control.vhd`).

* Include a performance check routine in the software to get the maximum transfer rate.

* Reading a single word from the DAQ fifo (via single register read) retrieves the old word in the output because the fifo is not set as first word fall through. This needs to be fixed, either reconfigure the fifo (then the bulk transfer has to be looked at, probably the request for the next word has to be shifted) or put some work on single register read.
