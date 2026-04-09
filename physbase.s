		xdef	_set_video_base
	
_set_video_base:
		movem.l	d0-d7/a0-a6,-(sp)
		move.w	#2000,sr
		move.w	#$20,-(sp)	;   mode and to use the user stack
		trap	#1		;   until toggling back to user mod
		
		move.l	#$FF8201,d6
		movep.l	d6,0(a0)		;*video_base_high in low byte / *video_base_mid in high byte
		move.l	0(a0),d1
		move.l	4(sp),d3  		;address of frame buffer
		move.l	d3,d2
		lsr.l	#8,d2     		;shift address of frame buffer
		lsr.l	#8,d2
		move.w	d2,d1			;move shifted value into low byte (video_base_high value)
		swap	d1				;*video_base_mid now in low byte
		lsr.l	#8,d3
		move.w	d3,d1				;move shifted value into high byte (video_base_mid value)
		swap	d1					;*video_base_high in low byte / *video_base_mid in high byte
		
		add.l	#2,sp
		move.l	d0,old_ssp
		move.l	old_ssp(pc),-(sp)	; return to user mode, restoring
		move.w	#$20,-(sp)		;   system stack pointer
		trap	#1
		addq.l	#2,sp
		
		movem.l	(sp)+,d0-d7/a0-a6
		
		move.w	#0,-(sp)	; return to caller
		move.w	#$4C,-(sp)
		trap	#1
		
		
old_ssp	ds.l	1
		
		