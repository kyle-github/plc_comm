- [1.1. WARNING - DISCLAIMER](#11-warning---disclaimer)
- [1.2. Get It](#12-get-it)
- [1.3. Features](#13-features)
  - [1.3.1. High Level Features](#131-high-level-features)
  - [1.3.2. Detailed Features](#132-detailed-features)
    - [1.3.2.1. PLC Support](#1321-plc-support)
    - [1.3.2.2. Platform Support](#1322-platform-support)
  - [1.3.3. Alternate Programming Languages](#133-alternate-programming-languages)
- [1.4. Code](#14-code)
  - [1.4.1. How to Get The Code](#141-how-to-get-the-code)
  - [1.4.2. Example Code](#142-example-code)
  - [1.4.3. API](#143-api)
- [1.5. Help Wanted](#15-help-wanted)
  - [1.5.1. How to Contribute](#151-how-to-contribute)
- [1.6. History](#16-history)
- [1.7. Contact and Support](#17-contact-and-support)
  - [1.7.1. lib\_comm Forum](#171-lib_comm-forum)
  - [1.7.2. GitHub](#172-github)
- [1.8. License](#18-license)
- [1.9. Attributions and Trademarks](#19-attributions-and-trademarks)
- [1.10. End Note](#110-end-note)


|   OS   | Architecture/CPU | Version |  64-bit   | 32-bit    |
|   --:  |         :-:      |   :-:   |    :-:    |    :-:    |
|Ubuntu  |   x86, Arm       | 18.04   | Supported | Supported |
|Windows |   x86, Arm       |10 (Server 19) | Supported | Supported |
|macOS   |   x86, Arm       |  11  | Supported | Not Supported |

![lib_comm CI](https://github.com/kyle_github/lib_comm/workflows/lib_comm%20CI/badge.svg?branch=release)

**lib_comm** is an open source C library for Linux, Windows and macOS using **EtherNet/IP** or **Modbus TCP** to read and write tags in PLCs.  The library has been in production since early 2012 and is used by multiple organizations for many tasks including controlling radio telescopes, large and precision manufacturing, controlling fitness equipment, food handling and many, many more.

Current Stable Version: **NONE**!

Old Stable Version: **NONE**

## 1.1. WARNING - DISCLAIMER

THIS LIBRARY IS NOWHERE NEAR USABLE.  There really is not much code here.   It all all just an experiment with a different API!

**DO NOT USE THIS!!!!!!**

Note: **PLCs control many kinds of equipment and loss of property, production or even life can happen if mistakes in programming or access are made.  Always use caution when accessing or programming PLCs!**

We make no claims or warrants about the suitability of this code for any purpose.

Be careful!

## 1.2. Get It

Do you know what you want already?  Download it from the [releases page](https://github.com/kyle_github/lib_comm/releases)!

## 1.3. Features

### 1.3.1. High Level Features

- EtherNet/IP and Modbus TCP support.
- Open source licensing under the MPL 2.0 or LGPL 2+.
- Pure C library for portability across Linux, Windows and macOS as well as 32-bit and 64-bit.
- Support for x86, ARM and MIPS, and probably others.
- Very stable API with almost no changes other than feature additions since 2012.
- Low memory use and very high performance and capacity.  Uses protocol-specific features to increase performance.
- Simple API with minimal use of language-specific data to enable easy wrapping in other languages.
- Extensive example programs showing use of all library features.
- Wrappers for higher level languages like C#/.Net, Julia etc.
- Free!

### 1.3.2. Detailed Features

#### 1.3.2.1. PLC Support

- support for Rockwell/Allen-Bradley ControlLogix(tm) PLCs via CIP-EtherNet/IP (CIP/EIP or EIP).
  - read/write 8, 16, 32, and 64-bit signed and unsigned integers.
  - read/write single bits/booleans.
  - read/write 32-bit and 64-bit IEEE format (little endian) floating point.
  - raw support for user-defined structures (you need to pull out the data piece by piece)
  - read/write arrays of the above.
  - multiple-request support per packet.
  - packet size negotiation with newer firmware (version 20+) and hardware.
  - tag listing, both controller and program tags.
- support for Rockwell/Allen-Bradley Micro 850 PLCs.
  - Support as for ControlLogix where possible.
- support for older Rockwell/Allen-Bradley such as PLC-5 PLCs (Ethernet upgraded to support Ethernet/IP), SLC 500 and MicroLogix with Ethernet via CIP.
  - read/write of 16-bit INT.
  - read/write of 32-bit floating point.
  - read/write of arrays of the above (arrays not tested on SLC 500).
- support for older Rockwell/Allen-Bradley PLCs accessed over a DH+ bridge (i.e. a LGX chassis with a DHRIO module) such as PLC/5, SLC 500 and MicroLogix.
  - read/write of 16-bit INT.
  - read/write of 32-bit floating point.
  - read/write of arrays of the above.
- extensive example code.  Including
  - tag listing.
  - setting up and handling callbacks.
  - logging data from multiple tags.
  - reading and writing tags from the command line.
  - getting and setting individual bits as tags.
- Support for Omron NX/NJ series PLCs as for Allen-Bradley Micro800.
- Support for Modbus TCP.

#### 1.3.2.2. Platform Support

- CMake build system for better cross-platform support on Windows, Linux and macOS.
  - Native CMake support is present in recent versions of Microsoft Visual Studio.
- Semantic versioning used and supported with specific library APIs for compatibility and feature checking.
- C library has no dependencies apart from libc (and pthreads on some platforms).
- Binary releases built for Ubuntu 18.04, macOS 10.15 and Windows 10.  All 64-bit, with 32-bit binary releases for Windows and Ubuntu.
- RaspberryPi supported. Both Linux and Windows IoT-based (some effort required to configure Visual Studio to build).

### 1.3.3. Alternate Programming Languages

The C library is designed for easy wrapping.  Wrappers for many other languages include the following:

- wrappers that are part of the GitHub lib_comm organization:
  - **.Net Core, .Net Framework, [lib_comm.NET](https://github.com/lib_comm/lib_comm.NET). Very popular!**
  - Go [goplctag](https://github.com/lib_comm/goplctag)
  - Java [libplctag4j](https://github.com/lib_comm/libplctag4j)
  - Julia, [PLCTag.jl](https://github.com/lib_comm/PLCTag.jl)
- Minimal wrappers included with the C source:
  -  C++
  -  Python
  -  Pascal
- Other wrappers on GitHub:
  - C#, [Corsinvest](https://github.com/Corsinvest/cv4ab-api-dotnet)
  - C#, [Mesta Automation](https://github.com/mesta1/lib_comm-csharp)
  - Labview, (see [lib_comm-labview](https://github.com/dirtyb15/lib_comm-labview))

## 1.4. Code

### 1.4.1. How to Get The Code

The code for the core library is at [lib_comm](https://github.com/lib_comm/lib_comm).   Stable code is on the default _release_ branch.   If you check out code from GitHub, it will default to the _release_ branch.

If you want pre-built binaries, we have them available on the [releases](https://github.com/lib_comm/lib_comm/releases) page.   Just pick the one you want and download the ZIP file for your system.   We have 32 and 64-bit builds for x86 Linux and Windows and 64-bit builds for x86-64 macOS.

Go to the main project at the [lib_comm organization](https://github.com/lib_comm) to see the other wrappers.   We are in a state of transition right now as we move more alternate language wrappers into the GitHub organization.

### 1.4.2. Example Code

Oh, wait, you want code!   There are many examples in the [examples](https://github.com/lib_comm/lib_comm/tree/release/src/examples) directory.

A good place to start is [simple.c](https://github.com/lib_comm/lib_comm/blob/release/src/examples/simple.c).

This code reads several 32-bit signed integers (DINT), updates them,
then writes them back out and rereads them from a tag named TestBigArray
in a Logix-class Allen-Bradley PLC.

The README file in the examples directory describes some of the more interesting ones.

### 1.4.3. API

Most of the functions in the API are for data access.   Direct support for single bits, 8-bit, 16-bit, 32-bit and
64-bit words (integer and floating point) are provided by the library.

See the [API](https://github.com/lib_comm/lib_comm/wiki/API) for more information.

## 1.5. Help Wanted

We need and welcome help with the following:

- bug reports!   We may not have your hardware so your bugs can help us make sure the library works in cases we cannot find!
- bug fixes.
- other protocols like Modbus, SBus etc.
- other platforms like Android, iOS etc.
- changes and additions for other PLCs.
- additional compilers.
- more language wrappers!
- patches and updates for existing language wrappers!
- Testing and more testing!

### 1.5.1. How to Contribute

We love contributions!   Many users have contributed wrappers, extra functionality and bug fixes over the years.   The library is much better for all the help that users have provided.   **We ask that your code contributions to the core library are under the same dual MPL/LGPL license.**

Testing is difficult for us as we do not have access to all the different hardware out there.   If you can, a great way to contribute is to test prereleases.  These are on the _prerelease_ branch!  We appreciate all the help we get from our users this way.

The easiest way to contribute to the core library is to raise a PR on GitHub.

Wrappers in other languages are generally split off into separate projects.  Those may have different licenses and contribution processes.  Please look at the documentation for the wrapper in question.

## 1.6. History

See the [wiki history page](https://github.com/lib_comm/lib_comm/wiki/History) for more details on how lib_comm was created and why we built it.

## 1.7. Contact and Support

There are two ways to ask for help or contact us.

### 1.7.1. lib_comm Forum

If you have general questions or comments about the
library, its use, or about one of the wrapper libraries, please join the Google group
[lib_comm](https://groups.google.com/forum/#!forum/lib_comm)!

The forum is open to all, but is by request only to keep the spammers down.  The traffic is fairly
light with usually a small number of emails per month.  It is our primary means for users to
ask questions and for discussions to happen.   Announcements about releases happen on the forum.

### 1.7.2. GitHub

If you find bugs or need specific features, please file them on [GitHub's issue tracker](https://github.com/lib_comm/lib_comm/issues) for
the main C library project.  Each individual wrapper project has its own issue tracker.

If needed, we will initiate private communication from there.

## 1.8. License

See the license files (LICENSE.MPL or LICENSE.LGPL) for our legal disclaimers of responsibility, fitness or
merchantability of this library as well as your rights with regards
to use of this library.  This code is **dual licensed** under the Mozilla Public License 2.0 (MPL 2.0) or the GNU
Lesser/Library General Public License 2 or later (LGPL 2+).

This dual license applies to the core C library.  Additional wrappers for other languages may be under different licenses.   Please see those projects for more information.

## 1.9. Attributions and Trademarks

PLC5, SLC 500, MicroLogix, Micro8X0, CompactLogix and ControlLogix are trademarks of Rockwell/Allen Bradley.
Windows and Visual Studio are trademarks of Microsoft.  Apple owns the trademark on macOS.

Please let us know if we missed some so that we can get all the attributions correct!

## 1.10. End Note

Have fun and let us know if this library is useful to you.  Please send test
cases if you run into bugs.  As PLC hardware is fairly expensive, we may not
be able to test out your test scenarios.  If possible, please send patches.
We do not ask that you transfer copyright over to us, but we do ask that you
make any submitted patches under the same licenses we use.  We will not
take any patches under the GPL license or licenses that are incompatible with the
MPL 2.0 license.

We hope you find this library as useful as we do!

- the lib_comm team
