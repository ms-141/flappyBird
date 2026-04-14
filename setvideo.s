 		;File: setvideo.s
		;
  		;Team Members: Muji Shah, Gurshan Chera, Declan McCarthy
  		;Date Created: April 3, 2026
		;
  		;Description:
  		;This file replaces the set_screen TOS function call.
 
		;----- Function: _set_video_base -----
		;
 		;PURPOSE: Sets the frame buffer start address in the video base register.
          ;This replaces the TOS Setscreen system call.
          ;Note: The actual page flip occurs at the next VSYNC.
		;
 		;INPUT: Address(UINT16*): pointer to the new frame buffer start address
		;
 		;OUTPUT: None
		
		xdef _set_video_base

_set_video_base:
		; Read the argument (address) BEFORE any pushes change the stack.
		; At entry: sp+0=return_addr, sp+4=address argument (standard 68k C calling convention)
		move.l	4(sp),arg_addr	; save argument to memory before touching sp

		movem.l	d0-d7/a0-a6,-(sp) ; save scratch registers
		clr.l	-(sp)           ; Setsuper(0) — enter supervisor mode
		move.w	#$20,-(sp)
		trap	#1
		add.l	#6,sp
		move.l	d0,old_ssp      ; save old supervisor SP

		move.l	arg_addr(pc),d3	; retrieve the framebuffer address

		move.l	#$FF8201,d6
		move.l	d6,a0
		move.l	d3,d2
		lsr.l	#8,d2           ; shift right 16 → get high byte
		lsr.l	#8,d2
		movep.w	(a0),d1         ; read current video base regs (to preserve unused bits)
		move.b	d2,d1           ; put high byte (bits 23-16) into d1 low byte → $FF8201
		lsl.l	#8,d1
		lsr.l	#8,d3
		move.b	d3,d1           ; put mid byte (bits 15-8)  into d1 high byte → $FF8203
		movep.w	d1,(a0)         ; write both bytes via movep

		move.l	old_ssp(pc),-(sp) ; Setsuper(old_ssp) — return to user mode
		move.w	#$20,-(sp)
		trap	#1
		addq.l	#6,sp

		movem.l	(sp)+,d0-d7/a0-a6
		rts
		
		
old_ssp		ds.l	1
arg_addr	ds.l	1
		
		