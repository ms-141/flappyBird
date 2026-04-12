		xdef _ikbd_isr
		xref _do_IKBD_ISR
		
_ikbd_isr:
		movem.l	d0-d7/a0-a7,-(sp)
		