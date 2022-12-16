# My Personal Build of dwm

This build of dwm has been expanded upon to match my personal, opinionated preferences — suckless philosophy be damned.

# Patches

- [Status2d](https://dwm.suckless.org/patches/status2d/): Allows colors and rectangle drawing in your dwm status bar 
- [Statuscmd](https://dwm.suckless.org/patches/statuscmd/): Allows the ability to execute shell commands based on the mouse button and position when clicking the status bar (integrated with my build of [dwmblocks](github.com/RedPenguin88/dwmblocks))
- [Swallow](https://dwm.suckless.org/patches/swallow/): Adds "window swallowing", i.e. running graphical programs from the terminal will "swallow" the terminal window. Upon closing the graphical program, the terminal is restored to the current position.
- [Vanity Gaps](https://github.com/bakkeby/patches/blob/master/dwm/dwm-vanitygaps-6.3.diff): Adds configurable gaps between windows differentiating between outer, inner, horizontal and vertical gaps.

# Dependencies

- dwmblocks: Recommended, but can be omitted with some modification
- Swallow Dependencies
    - libxcb
    - Xlib-libxcb
    - xcb-res
