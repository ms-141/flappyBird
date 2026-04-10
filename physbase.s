		xdef _set_video_base

_set_video_base:
		movem.l	d0-d7/a0-a6,-(sp)
		clr.l	-(sp)
		move.w	#$20,-(sp)	
		trap	#1		
		add.l	#6,sp
		
		move.l	d0,old_ssp
		
		move.l	#$FF8201,d6
		move.l	d6,a0
		movep.w	(a0),d1		;*video_base_high in low byte / *video_base_mid in high byte
		move.l	4(sp),d3  		;address of frame buffer
		move.l	d3,d2
		lsr.l	#8,d2     		;shift address of frame buffer
		lsr.l	#8,d2
		move.b	d2,d1			;move shifted value into low byte (video_base_high value)
		;swap	d1				;*video_base_mid now in low byte
		lsl.l	#8,d1
		lsr.l	#8,d3
		move.b	d3,d1				;move shifted value into high byte (video_base_mid value)
		;swap	d1					;*video_base_high in low byte / *video_base_mid in high byte
		
		movep.w	d1,(a0)
		
		move.l	old_ssp(pc),-(sp)	; return to user mode, restoring
		move.w	#$20,-(sp)		;   system stack pointer
		trap	#1
		addq.l	#6,sp
		
		movem.l	(sp)+,d0-d7/a0-a6
		rts
		
		
old_ssp	ds.l	1
		
		