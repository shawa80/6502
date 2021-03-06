_dis_portB = $6000
_dis_portA = $6001
_dis_DDRB = $6002
_dis_DDRA = $6003

_timer1_T1C_L = $6004
_timer1_T1C_H = $6005
_timer1_T1L_L = $6006
_timer1_T1L_H = $6007
_timer1_T2C_L = $6008
_timer1_T2C_H = $6009

_SR = $600A
_ACR = $600B
_PCR = $600C
_IFR = $600D
_IER = $600E
_ORA = $600F


;VIA 2
_via2_portB = $4000
_via2_portA = $4001
_via2_DDRB = $4002
_via2_DDRA = $4003

_via2_timer1_T1C_L = $4004
_via2_timer1_T1C_H = $4005
_via2_timer1_T1L_L = $4006
_via2_timer1_T1L_H = $4007
_via2_timer1_T2C_L = $4008
_via2_timer1_T2C_H = $4009

_via2_SR = $400A
_via2_ACR = $400B
_via2_PCR = $400C
_via2_IFR = $400D
_via2_IER = $400E
_via2_ORA = $400F


r0  = $00
r1  = $01
r2  = $02
r3  = $03
r4  = $04
r5  = $05
r6  = $06
r7  = $07
r8  = $08
r9  = $09
r10 = $0A
r11 = $0B
r12 = $0C
r13 = $0D
r14 = $0E
r15 = $0F
r16 = $10
r17 = $11
r18 = $12
r19 = $13
r20 = $14
r21 = $15
r22 = $16
r23 = $17
r24 = $18
r25 = $19
r26 = $1A
r27 = $1B
r28 = $1C
r29 = $1D
r30 = $1E
r31 = $1F

btmp0 = $20  ;20 - 23
btmp1 = $24  ;24 - 27
btmp2 = $28  ;28 - 2b
btmp3 = $2c  ;2c - 2f

sp  = $30
sp0 = $30
sp1 = $31


_clk_hr = $34
_clk_min = $35
_clk_sec = $36
_clk_jiff = $37 

;todo this is hex, act like it
_com_index = $39	;two bytes
_com_size = $41		;two bytes

;$0032 - $00ff   ; open zero page
;$0100 - $01ff ; hardware stack

cStackL = $0200
cStackH = $0600

_com_buf = $0602
_com_buf_start = $0602
_com_buf_px    = $060A
_com_buf_end   = $0E02 ;2k of space

_led_px = $0E03  ;400 bytes for 200 char pointers
_led_px_end = $0F93


  .org $8000

reset:
  ldx #$ff
  txs


  ; init c stack
  lda #00
  sta sp0
  lda #06
  sta sp1

main:

  jsr _start

loop:
  jmp loop

_sys_irqEnable:
  cli
  rts

_sys_irqDisable:
  sei
  rts

  .include test.asm


nmi:
irq:
  ;jsr _irqHandler
  bit _timer1_T1C_L
  inc _clk_jiff

  rti

  .org $fffa
  .word nmi
  .word reset
  .word irq

