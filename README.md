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

- install QT 5.5 (previously 4.8.x)
- install QWT 6.1.2 (previously 5.1)
- install the OpenHaptics 3.4 for the Sensable Omni
- install the Quanta network API https://www.evl.uic.edu/cavern/quanta/download.php 
- create environment variables:
      - QTDIR pointing to the QT installation directory
      - QWTDIR pointing to QWT installation directory
      - OH_SDK_BASE to point to OpenHaptics
      - QUANTADIR to point to Quanta
- build with Visual C++ 2010


