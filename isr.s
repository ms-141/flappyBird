    xdef	_vbl_isr

_vbl_isr:
    movem.l	d0-d7/a0-a6,-(sp)	; save all registers
    move.l	#1,-(sp)
    jsr	_update_music
    addq.l	#4,sp
    pea	_model
    jsr	_handleBirdMovement
    addq.l	#4,sp
    pea	_model
    jsr	_handlePipeMovement
    addq.l	#4,sp
    pea	_model
    jsr	_handleBirdCollision
    addq.l	#4,sp
    pea	_model
    jsr	_handlePipeRespawn
    addq.l	#4,sp
    pea	_model
    jsr	_handleScoreIncrease
    addq.l	#4,sp
    move.w	#1,_render_request
    movem.l	(sp)+,d0-d7/a0-a6
    rte
