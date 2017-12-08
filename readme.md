
Hacked Nordic NRF SDK 14.2 ... examples/ble_peripheral/ble_app_template/pca1040/s132/armgcc/

Log
-

Follow tutorial "Nordic Eclipse GCC"
Later will follo tutorial "Custom Service and Characteristics"

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

(app flashes, advertises, and connects but no characteristics?)

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






