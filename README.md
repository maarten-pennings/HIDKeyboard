# HIDKeyboard
A small extra keyboard for a PC

## Introduction
I was inspired by [Maker's Muse](https://www.youtube.com/watch?v=MeYuIWGqquE) to make my own private keyboard.
Components I selected
 - [Arduino Pro Micro](https://www.aliexpress.com/item/Free-Shipping-New-Pro-Micro-for-arduino-ATmega32U4-5V-16MHz-Module-with-2-row-pin-header/1871481789.html)
 - [Keyboard switches](https://www.aliexpress.com/item/10Pcs-3-Pin-KeyCaps-Mechanical-Keyboard-Switch-Blue-for-Cherry-MX-Switches-Keyboard-Replacement-Tester-Kit/32884614611.html)
 - [Keyboard key caps](https://www.aliexpress.com/item/PBT-Keycaps-DSA-1u-Blank-Printed-Keycaps-For-Gaming-Mechanical-Keyboard-20pcs/32908275436.html)
 - Housing to be printed with Cetus3D
 - [USB HID software library from NicoHood](https://github.com/NicoHood/HID), which actually comes _with_ the Arduino IDE
 - The Arduino [application](ProMicroKbd)
 
The Arduino application is written by me in such a way that it should be easy to configure. That is what this github project is about.

I am considering adding a virtual serial com port to the keyboard, so that the configuration can simply be written to the keyboard, meaning that a recompile is no longer needed.

## Result
  [Bottom](bottom.jpg)
  Bottom view with USB plug and bolts (that connect to [inserts](https://www.aliexpress.com/item/CNIM-Hot-M2-x-3mm-Brass-Cylinder-Knurled-Threaded-Round-Insert-Embedded-Nuts-100pcs/32876609027.html)
  
  [Arduino](arduino.jpg)
  The Arduino pro micro inside

  [Top](top.jpg)
  Top view, still without the key caps.

(end)
