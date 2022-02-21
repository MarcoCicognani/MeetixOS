![banner](https://www.meetixos.org/images/Logo.png)

[![CI](https://github.com/MarcoCicognani/MeetixOS/actions/workflows/BuildAll.yml/badge.svg)](https://github.com/MarcoCicognani/MeetixOS/actions/workflows/BuildAll.yml)

# About

A hobby operating system for `x86` computers written in modern [C++20](https://en.cppreference.com/w/cpp/20) .<br>
It gains the power of the `UNIX` philosophy, but experiments new designs at the same time.

<img src="https://www.meetixos.org/images/MeetixOS.png">

# Prebuilt Images

[MeetixOS Nightly](https://github.com/MarcoCicognani/MeetixOS/actions/workflows/BuildAll.yml?query=is%3Asuccess+branch%3Amaster) :
Go to the latest job, then download `MeetixOS` located under Artifacts\
**You MUST be logged in to GitHub to download the image**

Follow [BuildInstructions.md](Documents/BuildInstructions.md) to build the system from scratch

# Run MeetixOS

For `QEMU` run with:

```shell
$ qemu-system-i386 -enable-kvm -cpu host -smp 2 -m 512M -serial stdio -cdrom MeetixOS.iso
```

**KVM strongly recommended**

# Current Project Direction

After a very long hiatus, with side projects and various incomplete re-implementation of the MeetixOS (in `C++`
and `Rust`), I decided to return to the original MeetixOS's codebase then progressively rewrite and improve each
existing component and write new ones.

Then I've decided to follow the fashion and implement near all the necessary code from myself to increase the fun and
the learning effect.

## Current Status

Currently, I'm working on `LibTC`, the Template Collection Library.

This template library is inspired by the [SerenityOS/AK](https://github.com/SerenityOS/serenity/tree/master/AK) and
the [SkiftOS/LibUtils](https://github.com/skiftOS/skift/tree/main/userspace/libs/libutils) libraries.

The goal is to make it usable in all the OS contexts, from the kernel to the applications, as substitute of
the `LibStdC++`.

## What's Next

After the completion of `LibTC` and after having tested it thoroughly, the next step is to start rewriting the kernel.

The idea is to make it a monolithic Unix-Like kernel which exposes standard UNIX filesystem features
like `/MeetiX/Devices` (`/dev`), `/MeetiX/Tasks` (`/proc`) and `/MeetiX/Runtime` (`/var`).

It could be a good idea to expose custom object-oriented system-calls (using Handle oriented call, like `Windows`,
wrapped into C++ RAII objects) which can be wrapped easily by the `LibC` functions to expose a standard UNIX/POSIX
system call interface for future ports.

## No More GUI

The GUI is temporarily disabled since the old WindowServer is not stable enough, and because I want to develop a new
Compositor system (probably after the new kernel), which allow by default to draw via canvas to a shared buffer.


