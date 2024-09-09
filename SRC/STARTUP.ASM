extrn		DriverMain:proc

public		start

DGROUP          GROUP _TEXT, _DATA

_TEXT		segment byte public use16 'CODE'
		assume cs:DGROUP, ds:DGROUP
		org 0000h

start:
		jmp     DriverMain
    
    db 0x0a, 0x0d
    db "Waaas guggst du???", 0x0a, 0x0d, 0
    db "Guckst du blöd in Hex-Editor???", 0x0a, 0x0d, 0
    dd 0xbadc0ded

_TEXT		ends
		end	start
