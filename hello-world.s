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


PORTB = $6000
PORTA = $6001
DDRB = $6002
DDRA = $6003
T1C_L = $6004
T1C_H = $6005
T1L_L = $6006
T1L_H = $6007
T2C_L = $6008
T2C_H = $6009
SR = $600A
ACR = $600B
PCR = $600C
IFR = $600D
IER = $600E
ORA = $600F

; A, X, Z are not preserved on calls

REG_B  = $02  ; general purpose regs
REG_BL = $02
REG_BH = $03

REG_C  = $04
REG_CL = $04
REG_CH = $05

REG_D  = $06
REG_DL = $06
REG_DH = $07

REG_E  = $08
REG_EL = $08
REG_EH = $09

REG_R  = $0A  ; reg for return values
REG_RL = $0A
REG_RH = $0B

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

;$0032 - $00ff   ; open zero page
;$0100 - $01ff ; hardware stack

cStackL = $0200
cStackH = $0600

_vtest = $33


_clk_hr = $34
_clk_min = $35
_clk_sec = $36
_clk_jiff = $37 

_com_index = $39	;two bytes
_com_size = $41		;two bytes
_com_buf = $43
_com_buf_end = $45


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


  .include test.asm


nmi:
irq:
;  pha
;  bit T1C_L
;  jsr _clk_tick
;  pla
  jsr _irqHandler
  rti

  .org $fffa
  .word nmi
  .word reset
  .word irq
