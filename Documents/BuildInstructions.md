# MeetixOS Build Instructions

## Prerequisites

Make sure you have all the following dependencies installed:

### Debian/Ubuntu/LinuxMint

```shell
sudo apt install build-essential gcc g++ cmake ninja-build libmpfr-dev libmpc-dev libgmp-dev qemu-system-x86 qemu-utils grub2-common xorriso patch wget
```

### ArchLinux/Manjaro

```shell
sudo pacman -S --needed base-devel gcc g++ cmake nasm ninja mpfr libmpc gmp qemu qemu-arch-extra grub2-common xorriso patch wget
```

## Build

### Toolchain

First you have to build the custom MeetixOS toolchain by running the following commands:

```shell
cd MeetixOS/Toolchain
./Build.sh
```

If the script fails you could restart it by typing:

```shell
./Build.sh --rebuild
```

If you are intentioned to do debugging you may want to keep the toolchain build file instead of deleting them, then you
could pass to the script `--skip-last-clean` to do that:

```shell
./Build.sh --skip-last-clean
```

After the completion of this script your host should be able to build MeetixOS

### MeetixOS

#### CLion

MeetixOS is a `CLion` project, then If you have the `CLion IDE` check the [use instructions](UseCLion.md).

#### Terminal

Otherwise, go to the `MeetixOS/Build` directory and type the following commands.

##### Debug

```shell
mkdir -p Debug
pushd Debug
cmake -GNinja -DCMAKE_TOOLCHAIN_FILE=Build/CMakeToolchain.txt -DCMAKE_BUILD_TYPE=Debug ../..
popd
```

##### Release

```shell
mkdir -p Release
pushd Release
cmake -GNinja -DCMAKE_TOOLCHAIN_FILE=Build/CMakeToolchain.txt -DCMAKE_BUILD_TYPE=Release ../..
popd
```

Then, lets build and run the system:

```shell
cd MeetixOS/Build/BUILD_TYPE_CHOSEN
ninja run
```

The last command will compile the code, pack a ramdisk image, build the ISO then run it via QEMU 
