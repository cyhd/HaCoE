# HaCoE
Haptic perception measurement software for research

Purpose

This software has been developped to study haptic perception and haptic communication using haptic arms and force sensors.
HaCoE log all informations in XML files. You can set-up one or two haptic arms, with or without force sensors. 
HaCoE has a control interface, you can display visual feedback if required for your experiment (but with additional development).
You can analyse your data into R.

Supported haptic arms

- Haption Desktop
- Entact W5D
- Sensable Phantom

Supported force sensor

- ATI nano 17

Installation

- install QT 5.5 (install 32 bits win32 mscv10.0 build) 
- install QWT 6.1.2
- install the OpenHaptics 3.4 for the Sensable Omni
- create environment variables:
      - QTDIR pointing to the QT installation directory
      - QWTDIR pointing to QWT installation directory
      - OH_SDK_BASE to point to OpenHaptics
- build with Visual C++ 2010 express


