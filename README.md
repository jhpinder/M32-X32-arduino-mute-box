# M32-X32-arduino-mute-box

This is a project that facilitates an eight channel "mute box" that communicates over MIDI to the M32 or X32 console.
It is meant to be an easy way for individuals with little to no audio engineering experience to operate the mute/unmute functions during a worship service.

`faderLookup` is an array representing the console faders 1-17 and maps which of those will control which local mute light.  
`outputFaderNumberLookup` is an array representing which console fader will be muted when the local mute buttons are pressed.  

Currently this is only set up to control channels 1-17 but could easily be expanded by increasing the size of `faderLookup`.

<br>

---

_If you use any part of this code in a non-profit setting, please credit [@jhpinder](https://www.github.com/jhpinder)._  
_If you wish to use this code in a for-profit setting, please contact [@jhpinder](https://www.github.com/jhpinder) for licensing._
