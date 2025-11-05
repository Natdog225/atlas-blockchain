#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <llist.h>
#include <openssl/sha.h>
#include "hblk_crypto.h"

/*
 * Include the transaction header
 * This brings in all transaction-related structs and prototypes
 */
#include "transaction.h"

/* --- Constants (from v0.1/v0.2) --- */
#define HBLK_MAGIC "HBLK"
#define HBLK_VERSION "0.3"
#define COINBASE_AMOUNT 50

/* Maximum length for block data (from v0.2) */
#define BLOCK_DATA_MAX 1024

/* --- Block & Blockchain Structs (from v0.2, updated for v0.3) --- */

/**
 * struct block_info_t - Block info structure
 *
 * @index:       Block index
 * @difficulty:  Block difficulty
 * @timestamp:   Block timestamp
 * @nonce:       Block nonce
 * @prev_hash:   Hash of the previous block
 */
typedef struct block_info
{
	uint32_t index;
	uint32_t difficulty;
	uint64_t timestamp;
	uint64_t nonce;
	uint8_t prev_hash[SHA256_DIGEST_LENGTH];
} block_info_t;

/**
 * struct block_t - Block structure
 *
 * @info:          Block info
 * @data_len:      Length of the data
 * @data:          Block data (flexible)
 * @transactions:  List of transactions (NEW in v0.3)
 * @hash:          Hash of the block
 */
typedef struct block
{
	block_info_t info;
	uint32_t data_len;
	int8_t data[BLOCK_DATA_MAX];
	llist_t *transactions; /* Updated for Task 10 */
	uint8_t hash[SHA256_DIGEST_LENGTH];
} block_t;

/**
 * struct blockchain_t - Blockchain structure
 *
 * @chain:   List of blocks
 * @unspent: List of unspent transaction outputs (NEW in v0.3)
 */
typedef struct blockchain
{
	llist_t *chain;	  /* list of block_t */
	llist_t *unspent; /* Updated for Task 11 */
} blockchain_t;

/* --- Blockchain/Block Function Prototypes --- */
/* (These are from v0.2 and tasks 10, 11, 12, 13, 15) */

blockchain_t *blockchain_create(void);
block_t *block_create(block_t const *prev, int8_t const *data,
					  uint32_t data_len);
void block_destroy(block_t *block);
void blockchain_destroy(blockchain_t *blockchain);
uint8_t *block_hash(block_t const *block,
					uint8_t hash_buf[SHA256_DIGEST_LENGTH]);
int blockchain_serialize(blockchain_t const *blockchain, char const *path);
blockchain_t *blockchain_deserialize(char const *path);
int block_is_valid(block_t const *block, block_t const *prev_block,
				   llist_t *all_unspent);
int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH],
							uint32_t difficulty);
void block_mine(block_t *block);
uint32_t blockchain_difficulty(blockchain_t const *blockchain);

#endif /* BLOCKCHAIN_H */