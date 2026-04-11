
    xdef _vbl_isr
    xref _do_VBL_ISR

_vbl_isr:
    movem.l	d0-d7/a0-a6,-(sp)	; save full context for interrupted code safety
    jsr	_do_VBL_ISR
    movem.l	(sp)+,d0-d7/a0-a6
    rte
