
Hacked Nordic NRF SDK 14.2 ... examples/ble_peripheral/ble_app_template/pca1040/s132/armgcc/

work in progress

Project Structure
-

Originally created in the SDK directory.
As hacked, it requires a linked resource to the /object directory of another github project multiProtocol.
When cloned to another directory location, the Makefile needs to be modified to point back into the SDK.
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






