MEMORY {
    FLASH(rx) : ORIGIN = 0x08000000, LENGTH = 32K
    SRAM(rwx) : ORIGIN = 0x20000000, LENGTH = 4K
}

INCLUDE "lnk/flash.ld"
INCLUDE "lnk/sram.ld"