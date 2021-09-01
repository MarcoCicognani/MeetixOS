;/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
;* MeetiX OS By MeetiX OS Project [Marco Cicognani & D. Morandi]                       *
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

; ############################################################################
;
;                                  NOTE
;
;   At this point, no stack is a available, don't use PUSH or POP!
;
;   This file is compiled to the ramdisk and loaded by the SMP mechanism to
;   start up APs.
;
;   The kernel prepares the three values for page directory, stack location
;   and startup location.
;
; ############################################################################


; Loaded by the kernel to this location
org 0x1000

; Initial setup
BITS 16
startup:
	; Load the GDT
    lgdt [gdtPointer]

    ; Enable protected mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax

	; Far-JMP into protected mode code
    jmp 0x8:protectedStart


; Protected mode
BITS 32
protectedStart:
	; Code segments were set by far JMP, set data segments
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

	; Lock all cores
	acquireLock:
    lock bts dword [interlock], 0
    jc acquireLock

	    ; Set page directory
	    mov eax, [0x500]
	    mov cr3, eax

		; Enable paging
	    mov eax, cr0
	    or eax, 0x80000000
	    mov cr0, eax

	    ; Set "global pages" flag
	    mov eax, cr4
	    or eax, 80
	    mov cr4, eax

		; Load stack from stack array
		mov eax, [0x508]		; current index
		shl eax, 2				; multiply by 4 (size of one entry)
		mov esp, [0x50C + eax]	; move entry to ESP
		mov ebp, esp

		; Increment AP counter
		inc dword [0x508]

	; Release lock
    lock btr dword [interlock], 0

	; Jump to kernel
	call [0x504]

	; AP should never exit, just for safety
	hang:
	hlt
	jmp hang


; Inter-core synchronization
interlock:
	dd 0x00000000


; Pointer to the GDT
gdtPointer:
	dw 24
	dd gdt

; Basic setup GDT
gdt:
	; null descriptor
	dw 0x0000
	dw 0x0000
	dw 0x0000
	dw 0x0000

	; code descriptor
	dw 0xFFFF
	dw 0x0000
	dw 0x9800
	dw 0x00CF

	; data descriptor
	dw 0xFFFF
	dw 0x0000
	dw 0x9200
	dw 0x00CF
