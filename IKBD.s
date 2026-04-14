		;File: IKBD.s
		;
  		;Team Members: Muji Shah, Gurshan Chera, Declan McCarthy
  		;Date Created: April 5, 2026
		;
  		;Description:
  		;This file replaces the associated IKBD TOS isr.		
		
		;----- Function: _ikbd_isr -----
		;
 		;PURPOSE: Custom code that is associated with the IKBD isr when reading keyboard and mouse input.
          ;This file replaces the associated IKBD TOS isr.
		;
 		;INPUT: None
		;
 		;OUTPUT: None

		xdef _ikbd_isr
		xref _do_IKBD_ISR
		
_ikbd_isr:
		movem.l	d0-d7/a0-a6,-(sp)
		jsr	_do_IKBD_ISR
		movem.l	(sp)+,d0-d7/a0-a6
    	rte
		