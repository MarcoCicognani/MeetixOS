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

global _loadGdt
global _loadTss

;
; void _loadGdt(uint32_t gdtPointerAddress)
;
; Mounts the global descriptor table
; and does a far jump back to the code
;
; Parameters:
;   gdtPointerAddress      ebp + 8
;
_loadGdt:
	; Prologue
	push ebp
	mov ebp, esp

	; Load GDT
	mov eax, [ebp + 8]
	lgdt [eax]
	; Switch to kernel data segment
	mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Far jump into kernel code segment
    jmp 0x08:flush

flush:
	; Epilogue
	pop ebp
    ret

;
; void _loadTss(uint16_t tssDescriptorIndex)
;
; Loads the TSS
;
; Parameters:
;   tssDescriptorIndex     ebp + 8
;
_loadTss:
	; Prologue
	push ebp
	mov ebp, esp

	mov ax, [ebp + 8]
	ltr ax

	; Epilogue
	pop ebp
	ret
