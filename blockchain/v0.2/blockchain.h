#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H
#define BLOCK_DATA_MAX_LEN 1024
#define BLOCK_GENERATION_INTERVAL 1
#define DIFFICULTY_ADJUSTMENT_INTERVAL 5

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <llist.h>
#include "../../crypto/hblk_crypto.h"

/* MACRO for Genesis Block data length */
#define BLOCKCHAIN_DATA_MAX 1024

/**
 * struct block_info_t - Block info structure
 *
 * @index:      Index of the Block in the Blockchain
 * @difficulty: Difficulty of a Block to be mined
 * @timestamp:  Time the Block was created at (UNIX timestamp)
 * @nonce:      Salt value used to alter the Block hash
 * @prev_hash:  Hash of the previous Block in the chain
 */
typedef struct block_info_t
{
	uint32_t index;
	uint32_t difficulty;
	uint64_t timestamp;
	uint64_t nonce;
	uint8_t prev_hash[SHA256_DIGEST_LENGTH];
} block_info_t;

/**
 * struct block_data_t - Block data
 *
 * @buffer: Data buffer
 * @len:    Data size (in bytes)
 */
typedef struct block_data_t
{
	int8_t buffer[BLOCKCHAIN_DATA_MAX];
	uint32_t len;
} block_data_t;

/**
 * struct block_t - Block structure
 *
 * @info: Block info
 * @data: Block data
 * @hash: 256-bit digest of the Block, to ensure authenticity
 */
typedef struct block_t
{
	block_info_t info;
	block_data_t data;
	uint8_t hash[SHA256_DIGEST_LENGTH];
} block_t;

/**
 * struct blockchain_t - Blockchain structure
 *
 * @chain: Linked list of blocks
 */
typedef struct blockchain_t
{
	llist_t *chain;
} blockchain_t;

/* Function Prototypes */
blockchain_t *blockchain_create(void);
block_t *block_create(block_t const *prev, int8_t const *data,
					  uint32_t data_len);
void block_destroy(block_t *block);
void blockchain_destroy(blockchain_t *blockchain);
uint8_t *block_hash(block_t const *block,
					uint8_t hash_buf[SHA256_DIGEST_LENGTH]);
int blockchain_serialize(blockchain_t const *blockchain, char const *path);
blockchain_t *blockchain_deserialize(char const *path);
int block_is_valid(block_t const *block, block_t const *prev_block);
int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH],
							uint32_t difficulty);
void block_mine(block_t *block);
uint32_t blockchain_difficulty(blockchain_t const *blockchain);

#endif /* BLOCKCHAIN_H */