========================================================================
    CONSOLE APPLICATION : RoboticsLibrary Project Overview
========================================================================


RoboticsLibrary.cpp
    File for testing and showing how classes operate.  Also serves as an examples program.



/////////////////////////////////////////////////////////////////////////////
Logic.cpp, Logic.h
Library of Functions - Implemented:
1) Pulse (One Shot) - Use to detect rising or falling edges of a boolean signal.  Use when you only want an event to happen once per trigger, but the trigger remains true for more than one itteration, such as a button press.
2) Toggle - Used to toggle a signal based on rising or falling edge of another boolean.  Use to maintain something as true/fall on a button press.
3) Schmidt - Implements a schmidt trigger on an analog signal.
4) DeadZone (overloaded) - If value is within the deadzone, is coerced to become zero instead.
5) Sample and Hold - Retains a value unless told to update.
6) Stable - Does not change until the T/F value has been consistent for a period of time
7) De-bounce - Does not switch until the output has been set for a consisten period of time, then can switch immediately on next signal
8) Debounce or Delay - Use Stable
9) Timer - Simple timer to determie when a certain amount of time has elapsed
10) Bool Confirm Check - Requires True for X time followed by a single false followed by True for Y Time.  If no 2nd true for Z time, times out.
11) Bump Counter - 
12) HL Time to Dute Cycle and Period - Converts High Time and Low Time to a Duty Cycle and Period values
13) Signal Pulse - See Pulse (OneShot)
14) UpDownCounter -  Boolean Input versio of Bump Counter
15) Larger/Smaller - Returns Min/Max based on flag input
16) Pulse Stretch and Delay - This Boolean Pulse shaper detects the rising edge (R-E) of the signal at IN.  OUT will goTrue after DELAY seconds for a time of WIDTH seconds. 
17) Tollerance Check - Checks if a variable is within between two thresholds, returns TF if it is and the Absolute distances to each
18) Square Wave Generator - Takes Period, Duty Cycle, and Enable.  Outputs T/F as needed.

Not Planning to Implement:
19) Number of Bools True - Counts number of bools true in an array and returns
20) Logic Gate Combination - Logic Operations on Arrays
21) Single Button Detect - Need explanation of Intent.  Priority determination?
22) JK Latch - Implements a JK Flip Flop
23) ValueChangeTracker

Next Up:
1) Map Functions
2) Drive Code
3) Math
4) Motion Control
5) Pneumatics
6) Sensors
7) QCP




/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////
