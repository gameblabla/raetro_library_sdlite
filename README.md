# SDLite 1.2.x - Redux for Embedded Systems

**SDLite** is a fork of [SDL](https://www.libsdl.org/) 1.2.15 (*legacy*) originally created by **Sam Lantinga** but now **redux for embedded systems**. This version is stripped down, heavily optimized for MIPS/ARM-based CPUs equipped with SIMD capabilities and is intended for experienced developers, deployment with a root filesystem and/or integrated into a SDK/Toolchain.

> **NOTES:**
>
> - SDLite 1.2.x is compatible with SDL 1.2.15 and can be used as a drop-in replacement with exception of the new features and functions removed.
>
> **WARNINGS:**
>
> - **Linux and 2D Framebuffer is and will be the only OS and back-end supported in this version**
> - The [author stated](https://wiki.libsdl.org/SourceCode) that there is no intention to ever release an official 1.2.16 build, nor to ever work on 1.2 again. The last official build is from 2012. So, If your hardware has any kind of 3D hardware acceleration support, the best course of action is to use **SDL 2.0** which is still being actively developed and can be downloaded [here](https://www.libsdl.org/download-2.0.php).
> - Patches for the official build is not compatible and would have to be applied manually.
> - Not all ARM Cortex processors have a NEON unit and old ARM processors may have a SIMD unit not compliant with NEON or ARMv6.

## About the Project

SDLite main goal the is curation of related content, organization and optimization of the SDL 1.2 library and it's dependencies for resource-constrained embedded systems while trying to breathe life back into this now ~~obsolete/legacy~~ *retro* library that still has great value for easy development on those devices and can yield great performance when properly optimized.

The project was born out of a desire to develop a rich graphical user interface for the **MiSTer FPGA** project which is implemented on the Terasic DE-10 Nano board, a CPU-FPGA SoC hybrid with an ARM Cortex A9 processor and no GPU, (*Unless you implement one yourself in HDL*). Combined with an obsession for optimization, it was clear that the proper implementation of ARM's SIMD extension **NEON** could improve the multimedia user experience by accelerating audio and video encoding/decoding, user interfaces and 2D graphics. 

The project now is been organized into one clean codebase with enhancements, patches, fixes and optimizations for **RISC** processors (**ARM/MIPS**) so other developers and communities can also take advantage of the library and it's features.

![RISC is good](https://i.imgur.com/EfgOPAi.png)

## New Features of SDLite 1.2.x

- Build System rewrite from scratch.
- Auto-Detect compiler assembler capabilities.
- Triple Buffering and Threaded V-sync.
- Assembly optimizations with ARMv6 SIMD instructions and NEON intrinsics.
- Run application without root privilege.
- Improved joystick detection.
- BGR->RGB swizzling.
- SDL_BlitScaled.
- Power Management status.
- Filesystem paths.
- `clock_gettime()`enforced if detected instead of `gettimeofday()`.

## Removed from This Version

- All Platforms with exception of Linux
- OpenGL.
- Window Management. (X11 has no power here!!!)
- All Video drivers. (except Framebuffer and KMS/DRM)
- All Audio drivers. (except ALSA, OSS and PulseAudio)
- All Input drivers.  (except Linux EV)
- Assembly Routines, Hermes and Altivec Blitters.
- GNU pth. (GNU Portable Threads)

## Status and Roadmap

- [x] SDLite 1.2.x - Redux for Embedded Systems
- [x] Optimize and Clean up the Remaining SDL Libraries (**In Progress**)
  - [x] SDLite_image
  - [ ] SDLite_mixer
  - [x] SDLite_net
  - [x] SDLite_ttf
  - [x] SDLite_gfx
- [x] Make sure each library work with the latest version of their dependencies
- [ ] Modernize and Update Old Documentation. (In Progress)
- [ ] OS Mesa Support (In Progress)
- [ ] OpenGL ES/ES2 Support
- [ ] Interactive CLI to Automate the Installation and Deployment of all Libraries and Dependencies
- [x] Buildroot Project Package Config
- [ ] Integer Scaler
- [ ] Implement Scale2x Project
- [ ] Optimization for Ingenic MIPS XBurst SIMD Instruction Set
- [ ] Fixed Point Libraries for Processors Without an FPU

> The development is co-ordinated via [the GitHub project page](https://github.com/TinyRetroLabs/SDLite_Core).

## Versioning

To avoid a pit of despair and dependency hell with version lock and/or version promiscuity, and prevent the project from easily and safely move forward, SDLite will adopt a version format similar of X.Y.Z, where X will be fixed at 1.2(Major.Minor), Y will represent the current Year in the format YY (Micro), and Z will be represented by the current Month and Date of the build in the format MMDD (Patch).

Since the last official build version was 1.2.15, SDLite 1.2.x will pick up from version 1.2.20, 'patch' version only affects the package and libtool versioning, so internally the build will be for example 1.2.20.0101, but not the SDL version which still reads 1.2.20. In addition all libraries will be re-versioned and follow the same model, requiring a version of SDL equal or greater than 1.2.20 so they can't be installed with an old official version of SDL.

## Contributing

External contributions of any and all forms are welcome and encouraged. If you’ve found a bug or have a suggestion, please don’t hesitate to detail these in the official issue tracker. For those looking to get their hands dirty and contribute code (the best kind of contribution!), please see CONTRIBUTING.md for more details.

## Credits and acknowledgment

- Sam Lantinga
- Alexey Melnikov
- Ryan C. Gordon
- Paul Cercueil
- João H. Spies
- Martin Donlon
- Alan Steremberg
- Ozkan Sezer
- Daniel Silsbee
- Mattias Engdegård
- Julian Peterson
- Ken Jordan
- Maxim Sobolev
- Wesley Poole
- Michael Vance
- Andreas Umbach
- Andreas Hofmeister
- To all who support the project, report bugs and help spread the word
- And the hundreds of people who have contributed make this library awesome! <3

## Legal

**Rætro and SDLite** or any of its maintainers are in no way associated with or endorsed by Simple DirectMedia Layer.

**Simple DirectMedia Layer** (**SDL**) is a cross-platform software development library designed to provide a hardware abstraction layer for computer multimedia hardware components created by *Sam Lantinga*

## License

2019-2020 © Marcus Andrade - SDLite 1.2.x and all related projects is under the [GNU GPLv3 license](https://www.gnu.org/licenses/gpl-3.0.html), so it can be combined with [LGPLv2.1](https://www.gnu.org/licenses/gpl-faq.html#compat-matrix-footnote-7) and compatible with the zlib and MIT license.

1997-2012 © Sam Lantinga - SDL 1.2 and older are available under the [GNU LGPL license](http://www.gnu.org/licenses/old-licenses/lgpl-2.1.en.html).