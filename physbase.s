	xdef _get_video_base
	
_get_video_base:
	move.w	#2,-(sp)
	trap	#14
	add.l	#2,sp
	rts