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

global _checkForCPUID
global _enableSSE

;
; bool checkForCPUID()
;
; Checks if the CPUID instruction is available. This is implemented in
; pure assembly, because C/C++ could cause race conditions due to the
; ability of the compiler to modify the EFLAGS when it needs to.
;
_checkForCPUID:
	pushfd
	pop eax
	mov ecx, eax
	xor eax, 0x200000
	push eax
	popfd
	pushfd
	pop eax
	xor eax, ecx
	shr eax, 21
	and eax, 1
	push ecx
	popfd
	ret

;
; void enableSSE()
;
; Prepares the necessary CPU flags to enable SSE instructions
_enableSSE:
	mov eax, cr0
	and ax, 0xFFFB
	or ax, 0x2
	mov cr0, eax
	mov eax, cr4
	or ax, 3 << 9
	mov cr4, eax
	ret
