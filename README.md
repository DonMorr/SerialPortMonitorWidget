# SerialPortMonitorWidget
Cross Platform Serial Port Monitor Widget

A Simple System Tray Icon widget for listing the serial ports connected to the computer.

Tested on Windows 10 and MacOS High Sierra

![Alt text](images/v1.0_mac.png?raw=true "MacOsx Screenshot")

Instructions to create a MacOS Bundle:
--------------------------------------
// Once build is complete (either using commmand line or QT Creator), go to build directory and run these:

macdeployqt SerialPortMonitor.app

install_name_tool -add_rpath "@executable_path/../Frameworks" SerialPortMonitor.app/Contents/MacOS/SerialPortMonitor

You should now be able to run your app bundle from Finder.

Instructions to create a deployable directory in Windows:
--------------------------------------
TODO


Instructions to create a deployable directory in Linux:
--------------------------------------
TODO
