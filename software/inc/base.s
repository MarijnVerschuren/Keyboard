.macro dpin_clk clk src
ubfx \clk, \src, #0, #4
.endm

.macro dpin_periph periph src
ubfx \periph, \src, #4, #8
.endm

.macro dpin_misc misc src
ubfx \misc, \src, #12, #8
.endm

.macro dpin_port port src
ubfx \port, \src, #20, #4
.endm

.macro dpin_pin pin src
ubfx \pin, \src, #24, #4
.endm

.macro dpin_alt alt src
ubfx \alt, \src, #28, #4
.endm

