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

* The bulk transfer has a sent package counter in the header. This needs to be checked for data loss within the MRF.

* Extend the bulk transfer to process larger requests by sending more than one reply package (`ethernet_core_wrapper.vhd` and MRF).
  Right now a request is truncated after reaching the maximum size that fit into a IP/UDP package. For performance reasons it would be beneficial to automatically send following packages until the requested word count is reached.
  **Note:** An additional `what_to_do` indicator should be used to mark an incomplete answer.

* A method to fill the DAQ fifo from the software. This can become handy for debugging the system and to simulate data from an ASIC without having the ASIC connected. Ideally this could handle a bulk transfer from the PC as well, but this is not really mandatory.

* Dublicate the register to handle two connected test devices (`register_control.vhd`).

* Include a performance check routine in the software to get the maximum transfer rate.

* Include spill detection, which can be deactivated easily.

* Make it possible to run the system with an external clock. Make sure, the configuration of the MMCM is not messed up by this. Right now the configuration (125 MHz, same as ethernet clock) is the same as the input clock of the MMCM.

* Get FairMQ transfer things from Simone's version of the readout system. With this, the read bulk data is transformed into a FairMQ stream which can be sent to multiple receivers at once (useful to write data on two PC in parallel to have a backup immediately and look at a subset of the data for a live monitor).

* Clean up the software and get a bit more structure inside. Right now everything is combined in the mainwindow.cpp/.h but this gets messy very soon. Also, there are still a lot of topix references in the software.

* Write some proper documentation of the software.
