#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdint.h>
#include <stddef.h>
#include <llist.h>
#include "hblk_crypto.h"

/*
 * Defines a transaction output.
 * It is an amount of "coins" locked by a public key.
 */
typedef struct tx_out
{
	uint32_t amount;
	uint8_t  pub[EC_PUB_LEN];
	uint8_t  hash[SHA256_DIGEST_LENGTH];
} tx_out_t;

/*
 * Defines an unspent transaction output.
 * This identifies an output that has not been spent.
 */
typedef struct unspent_tx_out
{
	uint8_t	block_hash[SHA256_DIGEST_LENGTH];
	uint8_t	tx_id[SHA256_DIGEST_LENGTH];
	tx_out_t out; /* The actual output, copied by value */
} unspent_tx_out_t;

/*
 * Defines a transaction input.
 * It is a reference to an unspent transaction output.
 */
typedef struct tx_in
{
	uint8_t	block_hash[SHA256_DIGEST_LENGTH];
	uint8_t	tx_id[SHA256_DIGEST_LENGTH];
	uint8_t	tx_out_hash[SHA256_DIGEST_LENGTH];
	sig_t   sig;
} tx_in_t;

/* Defines a transaction, contains input and outputs,things and stuff */
typedef struct transaction
{
	uint8_t id[SHA256_DIGEST_LENGTH];
	llist_t *inputs; /* list of tx_in_t */
	llist_t *outputs; /* list of tx_out_t */
} transaction_t;

/* --- Function Prototypes --- */

#endif /* TRANSACTION_H */