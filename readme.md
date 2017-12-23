
Hacked Nordic NRF SDK 14.2 ... examples/ble_peripheral/ble_app_template/pca1040/s132/armgcc/

Status: work in progress

Not a product, a template for testing other libraries.

History/Goals
=

The project has three mains() that follow the history.

Main1: ble_app_template augmented with custom service
-
One main started from Nordic example ble_app_template.  
I gradually inserted "#ifdef OLD" and replaced code with facade classes, keeping the app working as a BLE peripheral (but having no BT services.)  
Then I added a service and characteristic.  
I hacked out the board support (I don't want LED's and buttons.)  
I hacked out the peer manager (I don't want bonding.)  
I hacked out app_timer (its not reliable) and the "connections module" (don't need connection parameter negotiation?)  

In this form, the app appears on any BT sniffer.  

In the process of developing, I learned mostly about ordinary BT concepts and the Nordic SDK.  

Main2: Provisioner, spinning
-

The second main extracted the essentials from the first main (as modified.)  
Then I changed the app to be multiprotocol: supporting BT and another proprietary protocol.  
BT was used for "provisioning", i.e. configuring the app through a writeable BT characteristic.  
The provisioning is periodic: the app sleeeps mostly, and only advertises its provisioning for short durations.  
The provisioner (a BT central) is expected to poll (scan) for a long time to find a provisionable peripheral.  
The second main just spins (waits) between provisionings.

In this form, a sniffer might occasionally see the app, but the app doesn't stay awake long enough for a sniffer to write to a characteristic.  
(A special BT app for provisioning central is required, e.g. to be written using a FOSS project for BT using Swift on iOS.)

In the process of developing, I learned mostly about the complexities of developing multiprotocol using the Nordic SDK.
In hindsight, to develop a proprietary protocol, you need low-level API (HAL) to the hw.
But once you intend to use multiprotocol, you need to use higher-level API to cooperate with the Softdevice.


Main3: Provisioner, sleeping
-
The third main doesn't spin between provisionings, but uses a timer.  
I found that the libraries I was using for the proprietary protocol are incompatible with the Softdevice.  
The Softdevice blocks or restricts access to many devices (radio, low frequency clock, power/clock peripheral).  
To cooperate with the Softdevice, you must use "modules" from the Nordic SDK, instead of the HAL 
(which gives you unrestricted and non-cooperating access to device registers.)  
This main uses libraries in process of being changed to be SD compatible.  
In this form, the app is still a "provisioner" service.  
But it is ready for integration with a proprietary protocol that uses the SD compatible libraries (radioSoC and nRF5x.)


Project Structure
-

Originally created in the SDK directory.
When cloned to another directory location, the Makefile needs to be modified to point back into the SDK.

As hacked, it requires a linked resource to the /object directory of another github project: multiProtocol.

Now it should be self contained.

Project is a "makefile" project (Eclipse does not manage makefile.)  Using Makefile provided by Nordic, hacked.

Files modified from original:
    - main.c (now .cpp)
    - sdk_config.h
    - Makefile

What is changed
-

The original example has no services, but is ready to add services.  I followed the "Custom Service" tutorial to do so.

I switched to C++ for main.cpp, and added some facade classes (referenced from the multiProtocol repository.)  I am migrating all the original code to the facade classes.


Process and Testing
-

Make a small change, test, commit.

Use app "NRF Connect" on a BLE ready phone to test.  Expect:

    - device should appear as a BLE device, with one service, and a writeable characteristic.
    - device should be repeatably connectable and disconnectable
    - ??? device should never die (to be determined, I don't understand the sleep code?)

(Need a better description of the LED blinking.)

Log
-

Follow tutorials:
    - "Nordic Eclipse GCC"
    - "Custom Service and Characteristics" at novelbits.io
    - Nordic "BT low energy services, a beginner's tutorial."

Create new "makefile" proj per tutorial

Futz w/ make VERBOSE=1 in Eclipse settings per tutorial

Change Build>Global Toolchain Paths per tutorial (to a newer ARM GCC toolchain that I downloaded to ~/opt/...)

Edit components/toolchain/gcc/makefile.posix to point to new toolchain (shouldn't be necessary?)

(Build now works.)

Put under source code control (in directory ../s132/armgcc

Initial commit

Move main.c from two levels up and edit makefile PROJ_DIR var ../../.. => .

Change settings for auto discovery per tutorial.

Create flash build targets per tutorial.

(app flashes, advertises, and connects but no service or characteristics?)

Edit makefile REF_DIR to project multiProtocol/objects

Edit makefile main.c => main.cpp

Rename main.c

Edit makefile, add facade classes Service, etc.

Reference facade classes in main.cpp

Softlink from project dir to ../config/sdk_config.h

Edit .ld RAM 0x2100, 0xdf00


(not working, no services advertised.)

create facade class AdModule

(still not working) 

define OLD to restore original code in main.cpp

fix missing call to Uuid::init() (now it seems to work.)

sdk_config.h copied into project and hacked, instead of linked

C++ flag -std=c++14 in Makefile

(stopped working on another dev machine)

Changed -O0 in Makefile

(almost worked on another dev machine: scanned but crashed on connect

Reverted to OLD (original) and still crashes

Upgrade ARM tools to latest version 6.3.1 on other dev machine, now it seems to work again

Get debug config to work. Change debug Executable to arm-none-eabi-gdb, else errors on gdb console.
Enable RTT, disable UART in sdk_config.h

Hacked out board support, peer manager, and connections module.

Major work to use timers that are compatible with Softdevice.

Integrate with radioSoC and nRF5x libraries (now updated to be Softdevice compatible.)

Various flavors of main() to test multiprotocol.






