//
// Created by marijn on 4/18/24.
//

#ifndef SOFT_ROM_H
#define SOFT_ROM_H
#include "i2c.h"
#include "hash.h"
#include "cryp.h"
#include "rng.h"
#include "crc.h"


/*!<
 * defines
 * */
#define ROM_BASE_ADDRESS	0x50U
#define ROM_MAX_DELAY		1000U
#define ROM_MIN_DELAY		10U

#define ROM_PAGE_SIZE		0x80U
#define ROM_PAGE_COUNT		0x1FFU


/*!<
 * struct types
 * */
typedef struct __PACKED {
	uint32_t IV[0x04U];  // 0x10U
	uint8_t data[0x70U];
} ROM_encrypted_page_t;


/*!<
 * variables
 * */
extern uint32_t page_buffer[ROM_PAGE_SIZE >> 2];


/*!<
 * functions
 * */  // TODO: names!
void read_page(I2C_TypeDef* i2c, uint8_t ROM_address, uint16_t page_address, void* buffer);
void write_page(I2C_TypeDef* i2c, uint8_t ROM_address, uint16_t page_address, const void* buffer);
void reset_ROM(I2C_TypeDef* i2c, uint8_t ROM_address, GPIO_TypeDef* status_port, uint8_t status_pin);
void read_encrypted_page(I2C_TypeDef* i2c, uint8_t ROM_address, uint16_t page_address, const void* key, CRYP_KEY_t key_type, ROM_encrypted_page_t* buffer);
void write_encrypted_page(I2C_TypeDef* i2c, uint8_t ROM_address, uint16_t page_address, const void* key, CRYP_KEY_t key_type, ROM_encrypted_page_t* buffer);


/*!<
 * password ROM enum types
 * */
typedef enum {  //            [page_num]
	ROM_INFO =					0x000U,
	ROM_UNUSED =				0x001U,	// TODO
	ROM_PASSWORD_DESCRIPTORS =	0x010U,	// ROM_password_descriptor_t[240]
	ROM_PASSWORD_DATA =			0x100U,	// ROM_password_data_t[240]
	ROM_CRC =					0x1EFU,	// uint32_t[512]					// TODO
} ROM_page_map_t;
typedef enum {  //            [offset]
	ROM_MASTER_HASH =			0x00,
	ROM_PASSWORD_COUNT =		0x20,
} ROM_info_map_t;


/*!<
 * password ROM struct types
 * */
typedef struct __PACKED {
	uint8_t label[0x40];
	uint8_t email[0x30];
	uint8_t phone[0x0F];
	uint8_t page_number;
} ROM_password_descriptor_t;


/*!<
 * password ROM variables
 * */
extern uint8_t master_key[16];


/*!<
 * password ROM functions
 * */  // TODO: names!, return_type enum!!
uint8_t init_password_ROM(I2C_TypeDef* i2c, uint8_t ROM_address, const char* master_password);
uint8_t add_password(  // TODO: delete characteristics!!! (broken memory) (INFO page??)
	I2C_TypeDef* i2c, uint8_t ROM_address,
	const char* label, const char* email, const char* phone,
	const char* password, const char* note
);
uint8_t read_descriptor(  // TODO: delete characteristics!!! (broken memory) (INFO page??)
	I2C_TypeDef* i2c, uint8_t ROM_address, uint8_t index,
	ROM_password_descriptor_t* descriptor
);


#endif // SOFT_ROM_H
