//
// Created by marijn on 4/18/24.
//

#include "app/rom.h"


/*!<
 * variables
 * */
uint32_t page_buffer[ROM_PAGE_SIZE >> 2];


/*!<
 * static functions
 * */
static inline void decrypt_page(const void* key, CRYP_KEY_t key_type, ROM_encrypted_page_t* buffer) {
	AES_CBC_decrypt_setup(buffer->IV, key, key_type);
	for (uint8_t i = 0; i < (ROM_PAGE_SIZE / AES_BLOCK_SIZE) - 1U; i++) {
		AES_CBC_process_block(&buffer->data[i * AES_BLOCK_SIZE], &buffer->data[i * AES_BLOCK_SIZE]);
	}
}
static inline void encrypt_page(const void* key, CRYP_KEY_t key_type, ROM_encrypted_page_t* buffer) {
	AES_CBC_encrypt_setup(buffer->IV, key, key_type);
	for (uint8_t i = 0; i < ((ROM_PAGE_SIZE) / AES_BLOCK_SIZE) - 1U; i++) {
		AES_CBC_process_block(&buffer->data[i * AES_BLOCK_SIZE], &buffer->data[i * AES_BLOCK_SIZE]);
	}
}


/*!<
 * functions
 * */
void read_page(I2C_TypeDef* i2c, uint8_t ROM_address, uint16_t page_address, void* buffer) {
	I2C_master_read_reg(i2c, ROM_address, page_address << 7U, I2C_REG_16, (void*)buffer, ROM_PAGE_SIZE, ROM_MAX_DELAY);
}
void write_page(I2C_TypeDef* i2c, uint8_t ROM_address, uint16_t page_address, const void* buffer) {
	I2C_master_write_reg(i2c, ROM_address, page_address << 7U, I2C_REG_16, (void*)buffer, ROM_PAGE_SIZE, ROM_MAX_DELAY);
}
void reset_ROM(I2C_TypeDef* i2c, uint8_t ROM_address, GPIO_TypeDef* status_port, uint8_t status_pin) {
	for (uint8_t i = 0; i < (ROM_PAGE_SIZE >> 2); i++)	{ page_buffer[i] = 0x00000000UL; }
	for (uint16_t i = 0; i < ROM_PAGE_COUNT; i++)		{
		if (status_port) { GPIO_toggle(status_port, status_pin); }
		write_page(i2c, ROM_address, i, page_buffer);
		delay_ms(ROM_MAX_DELAY / 4);
	}
}
void read_encrypted_page(I2C_TypeDef* i2c, uint8_t ROM_address, uint16_t page_address, const void* key, CRYP_KEY_t key_type, ROM_encrypted_page_t* buffer) {
	I2C_master_read_reg(i2c, ROM_address, page_address << 7U, I2C_REG_16, (void*)buffer, ROM_PAGE_SIZE, ROM_MAX_DELAY);
	decrypt_page(key, key_type, buffer);
}
void write_encrypted_page(I2C_TypeDef* i2c, uint8_t ROM_address, uint16_t page_address, const void* key, CRYP_KEY_t key_type, ROM_encrypted_page_t* buffer) {
	encrypt_page(key, key_type, buffer);
	I2C_master_write_reg(i2c, ROM_address, page_address << 7U, I2C_REG_16, (void*)buffer, ROM_PAGE_SIZE, ROM_MAX_DELAY);
}


/*!<
 * password ROM struct types
 * */
typedef struct __PACKED {
	uint8_t master_hash[0x20];
	uint8_t password_count;
	uint8_t data[0x5F];
} ROM_info_page_t;


/*!<
 * password ROM variables
 * */
uint8_t master_key[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//uint32_t master_hash[8];


/*!<
 * password ROM static functions
 * */
static inline uint32_t calculate_page_CRC(void* buffer) {
	reset_CRC();
	for (uint8_t i = 0; i < (ROM_PAGE_SIZE >> 2); i++) {
		CRC->DR = ((uint32_t*)buffer)[i];
	} return CRC->DR;
}
static inline uint32_t get_page_CRC(I2C_TypeDef* i2c, uint8_t ROM_address, uint16_t page_address) {
	uint32_t crc; I2C_master_read_reg(
		i2c, ROM_address, (ROM_CRC << 7U) + (page_address << 2U),
		I2C_REG_16, (void*)&crc, 4U, ROM_MAX_DELAY
	); return crc;
}
static inline void set_page_CRC(I2C_TypeDef* i2c, uint8_t ROM_address, uint16_t page_address, uint32_t crc) {
	I2C_master_write_reg(
		i2c, ROM_address, (ROM_CRC << 7U) + (page_address << 2U),
		I2C_REG_16, (void*)&crc, 4U, ROM_MAX_DELAY
	);
}
// returns original password_count
static inline uint8_t increment_password_count(I2C_TypeDef* i2c, uint8_t ROM_address) {
	read_page(i2c, ROM_address, ROM_INFO, page_buffer);
	uint8_t password_count = ((ROM_info_page_t*)page_buffer)->password_count++;
	write_page(i2c, ROM_address, ROM_INFO, page_buffer);
	set_page_CRC(i2c, ROM_address, ROM_INFO, calculate_page_CRC(page_buffer));
	return password_count;
}


/*!<
 * password ROM functions
 * */
uint8_t init_password_ROM(I2C_TypeDef* i2c, uint8_t ROM_address, const char* master_password) {
	uint8_t eq; read_page(i2c, ROM_address, ROM_INFO, (void*)page_buffer);
	eq = 0; for (uint8_t i = 0; i < 8U; i++) { eq |= (page_buffer[i] != 0x00000000UL); }
	process_HMAC((void*)master_password, strlen(master_password), UID, sizeof(ID), HASH_ALGO_SHA2_256);
	if (!eq) {  // set master password
		for (uint8_t i = 0U; i < 8U; i++) { page_buffer[i] = ((__IO uint32_t*)HASH_digest)[i]; }
		for (uint8_t i = 8U; i < (ROM_PAGE_SIZE >> 2); i++) { page_buffer[i] = 0x00000000UL; }
		write_page(i2c, ROM_address, ROM_INFO, page_buffer);	// set page 0
		*page_buffer = calculate_page_CRC(page_buffer);
		for (uint8_t i = 1U; i < 8U; i++) { page_buffer[i] = 0x00000000UL; }
		write_page(i2c, ROM_address, ROM_CRC, page_buffer);		// set CRC of page 0 and reset the rest of the CRC page
	} else {	// check master password
		if (get_page_CRC(i2c, ROM_address, ROM_INFO) != calculate_page_CRC(page_buffer)) { return 0xFFU; }	// error (data corrupted) TODO: status enum
		eq = 0; for (uint8_t i = 0; i < 8U; i++) { eq |= (page_buffer[i] != ((__IO uint32_t*)HASH_digest)[i]); }
		if (eq) { return 0xFEU; }	// error (unauthorized) TODO: status enum
	}
	process_HASH((void*)master_password, strlen(master_password), HASH_ALGO_SHA2_256);
	for (uint8_t i = 0; i < 4U; i++) { ((uint32_t*)master_key)[i] = ((__IO uint32_t*)HASH_digest)[i] ^ ((__IO uint32_t*)HASH_digest)[4U + i]; }
	return 0x00U;  // success TODO: status enum
}


uint8_t add_password(
	I2C_TypeDef* i2c, uint8_t ROM_address,
	const char* label, const char* email, const char* phone,
	const char* password, const char* note
) {  // TODO: delete characteristics!!! (broken memory)
	ROM_password_descriptor_t*	descriptor =		(void*)page_buffer;
	ROM_encrypted_page_t*		password_page =		(void*)page_buffer;
	uint8_t						password_index =	increment_password_count(i2c, ROM_address);
	uint8_t*					ptr;
	uint8_t						password_length =	strlen(password);
	uint8_t						note_legth =		0;
	if (note)					{ note_legth =		strlen(note); }
	if (password_length + note_legth + 2 > 0x70U)	{ return 0xFDU; }  // data does not fit!! TODO: status enum
	/*!< write descriptor */
	for (uint8_t i = 0; i < (ROM_PAGE_SIZE >> 2); i++)	{ page_buffer[i] = 0x00000000UL; }
	if (label) { ptr = descriptor->label; while (*label) { *ptr++ = *label++; } }
	if (email) { ptr = descriptor->email; while (*email) { *ptr++ = *email++; } }
	if (phone) { ptr = descriptor->phone; while (*phone) { *ptr++ = *phone++; } }
	descriptor->page_number = password_index;
	write_page(i2c, ROM_address, ROM_PASSWORD_DESCRIPTORS + password_index, page_buffer);
	set_page_CRC(i2c, ROM_address, ROM_PASSWORD_DESCRIPTORS + password_index, calculate_page_CRC(page_buffer));
	/*!< write password data */
	for (uint8_t i = 0; i < (ROM_PAGE_SIZE >> 2); i++)	{ page_buffer[i] = 0x00000000UL; }
	for (uint8_t i = 0; i < (AES_BLOCK_SIZE >> 2); i++)	{ password_page->IV[i] = RNG_generate(); }
	ptr = password_page->data;
	for (uint8_t i = 0; i < password_length; i++)			{ ptr[i] = password[i]; }
	ptr += password_length; *ptr++ =	0x00U;	// regular NULL terminator to end password TODO: redo this!!!
	if (note) { for (uint8_t i = 0; i < note_legth; i++)	{ ptr[i] = note[i]; } }
	*(ptr + password_length) =			0x00U;	// regular NULL terminator to end password TODO: redo this!!!
	write_encrypted_page(i2c, ROM_address, ROM_PASSWORD_DATA + password_index, master_key, CRYP_KEY_128, password_page);
	// password_page/page_buffer is now in the encrypted state!
	set_page_CRC(i2c, ROM_address, ROM_PASSWORD_DATA + password_index, calculate_page_CRC(page_buffer));
	return 0x00U;  // success TODO: status enum
}

uint8_t read_descriptor(
	I2C_TypeDef* i2c, uint8_t ROM_address, uint8_t index,
	ROM_password_descriptor_t* descriptor
) {  // TODO: delete characteristics!!! (broken memory)
	read_page(i2c, ROM_address, ROM_PASSWORD_DESCRIPTORS + index, descriptor);
	if (
		get_page_CRC(i2c, ROM_address, ROM_PASSWORD_DESCRIPTORS + index) !=
		calculate_page_CRC(descriptor)
	) { return 0xFFU; }	// error (data corrupted) TODO: status enum
	return 0x00U;  // success TODO: status enum
}
uint8_t read_password(
	I2C_TypeDef* i2c, uint8_t ROM_address, uint16_t page_num,
	ROM_encrypted_page_t* password
) {  // TODO: delete characteristics!!! (broken memory)
	read_page(i2c, ROM_address, page_num, password);
	if (get_page_CRC(i2c, ROM_address, page_num) != calculate_page_CRC(password)) { return 0xFFU; }	// error (data corrupted) TODO: status enum
	decrypt_page(master_key, CRYP_KEY_128, password);
	return 0x00U;  // success TODO: status enum
}