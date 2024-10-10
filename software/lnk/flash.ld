SECTIONS {
	. = ORIGIN(FLASH);

	.vector ALIGN(4) : {
		_IVT_start = .;
		KEEP(*(.vector))
		. = ALIGN(4);
		_ITV_end = .;
	} > FLASH

	.text ALIGN(4) : {
		_text_start = .;
		KEEP(*(.text))
		KEEP(*(.text*))
		. = ALIGN(4);
		_text_end = .;
	} > FLASH

	.rodata ALIGN(4) : {
		KEEP(*(.rodata))
		KEEP(*(.rodata*))
		. = ALIGN(4);
	} > FLASH

/*
	.pre_init_array ALIGN(4): {
		_pre_init_array_start = .;
		KEEP(*(.preinit_array))
		_pre_init_array_end = .;
	} > FLASH

	.init_array ALIGN(4): {
		_init_array_start = .;
		KEEP(*(.init_array))
		_init_array_end = .;
	} > FLASH

	.fini_array ALIGN(4): {
		_fini_array_start = .;
		KEEP(*(.fini_array))
		_fini_array_end = .;
	} > FLASH
*/
}