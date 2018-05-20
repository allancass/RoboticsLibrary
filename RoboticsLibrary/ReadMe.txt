========================================================================
    CONSOLE APPLICATION : RoboticsLibrary Project Overview
========================================================================


RoboticsLibrary.cpp
    File for testing and showing how classes operate.  Also serves as an examples program.



/////////////////////////////////////////////////////////////////////////////
Logic.cpp, Logic.h

Library of Functions

Pulse (One Shot)
Use to detect rising or falling edges of a boolean signal.  Use when you only want an event to happen once per trigger, but the trigger remains true for more than one itteration, such as a button press.

Toggle
Used to toggle a signal based on rising or falling edge of another boolean.  Use to maintain something as true/fall on a button press.

Schmidt
Implements a schmidt trigger on an analog signal.

DeadZone (overloaded)

Sample and Hold

De-bounce or Delay/Stable

Timer

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////
