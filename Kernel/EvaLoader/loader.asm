;/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
;* MeetiX OS By MeetiX OS Project [Marco Cicognani]                                    *
;*                                                                                     *
;*         DERIVED FROM THE GHOST OPERATING SYSTEM                                     *
;*         This software is derived from the Ghost operating system project,           *
;*         written by Max Schlüssel <lokoxe@gmail.com>. Copyright 2012-2017            *
;*         https://ghostkernel.org/                                                    *
;*         https://github.com/maxdev1/ghost                                            *
;*                                                                                     *
;* This program is free software; you can redistribute it and/or                       *
;* modify it under the terms of the GNU General Public License                         *
;* as published by the Free Software Foundation; either version 2                      *
;* of the License, or (char *argumentat your option) any later version.                *
;*                                                                                     *
;* This program is distributed in the hope that it will be useful,                     *
;* but WITHout ANY WARRANTY; without even the implied warranty of                      *
;* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                       *
;* GNU General Public License for more details.                                        *
;*                                                                                     *
;* You should have received a copy of the GNU General Public License                   *
;* along with this program; if not, write to the Free Software                         *
;* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA      *
;* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * */


BITS 32

; # MULTIBOOT
section .multiboot
	MODULEALIGN		equ		1 << 0
	MEMINFO			equ 	1 << 1
	FLAGS			equ		MODULEALIGN | MEMINFO
	MAGIC			equ		0x1BADB002
	CHECKSUM		equ		-(MAGIC + FLAGS)

	; Fill constants into memory (dwords)
	align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM



; # CODE
section .text

	; Entry point for GRUB
	global loader

	; Initialization method
	extern initializeLoader

	; Create the initial loader stack
	STACKSIZE equ 0x1000

	; Calls the initialization routines
	loader:
		; Set the stack
		mov esp, stack + STACKSIZE
		mov ebp, esp

		; We don't want interrupts until the kernel is ready
		cli

		; Call the loader
		push eax ; Magic number
		push ebx ; Multiboot information pointer
	    call initializeLoader

	; Hang the system after execution
	hang:
		hlt
	    jmp hang



; # DATA
section .bss

	; Align the location of the following res-commands
	align 4

	; Reserves space for the initial kernel stack
	stack:
		resb STACKSIZE
