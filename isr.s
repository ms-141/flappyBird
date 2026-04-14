    ;File: isr.s
		;
  		;Team Members: Muji Shah, Gurshan Chera, Declan McCarthy
  		;Date Created: April 4, 2026
		;
  		;Description:
  		;This file replaces the associated VBL TOS isr.		
		
		;----- Function: _vbl_isr -----
		;
 		;PURPOSE: Custom code that is associated with the VBL isr when dealing with vertical blanks.
          ;This file replaces the associated VBL TOS isr.
		;
 		;INPUT: None
		;
 		;OUTPUT: None

    xdef _vbl_isr
    xref _do_VBL_ISR

_vbl_isr:
    movem.l	d0-d7/a0-a6,-(sp)	; save full context for interrupted code safety
    jsr	_do_VBL_ISR
    movem.l	(sp)+,d0-d7/a0-a6
    rte
