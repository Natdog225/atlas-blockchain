#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdint.h>
#include <stddef.h>
#include <llist.h>
#include "hblk_crypto.h" /* For sig_t, EC_KEY, EC_PUB_LEN, etc. */

/*
 * Defines a transaction output.
 * It is an amount of "coins" locked by a public key.
 */
typedef struct tx_out
{
	uint32_t amount;
	uint8_t pub[EC_PUB_LEN];
	uint8_t hash[SHA256_DIGEST_LENGTH];
} tx_out_t;

/*
 * Defines an unspent transaction output.
 * This identifies an output that has not been spent.
 */
typedef struct unspent_tx_out
{
	uint8_t block_hash[SHA256_DIGEST_LENGTH];
	uint8_t tx_id[SHA256_DIGEST_LENGTH];
	tx_out_t out; /* The actual output, copied by value */
} unspent_tx_out_t;

/*
 * Defines a transaction input.
 * It is a reference to an unspent transaction output.
 */
typedef struct tx_in
{
	uint8_t block_hash[SHA256_DIGEST_LENGTH];
	uint8_t tx_id[SHA256_DIGEST_LENGTH];
	uint8_t tx_out_hash[SHA256_DIGEST_LENGTH];
	sig_t sig;
} tx_in_t;

/* Defines a transaction, containing inputs and outputs */
typedef struct transaction
{
	uint8_t id[SHA256_DIGEST_LENGTH];
	llist_t *inputs;  /* list of tx_in_t */
	llist_t *outputs; /* list of tx_out_t */
} transaction_t;

/* --- Transaction Function Prototypes --- */

/* task 0 */
tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN]);
/* task 1 */
unspent_tx_out_t *unspent_tx_out_create(
	uint8_t block_hash[SHA256_DIGEST_LENGTH],
	uint8_t tx_id[SHA256_DIGEST_LENGTH],
	tx_out_t const *out);
/* task 2 */
tx_in_t *tx_in_create(unspent_tx_out_t const *unspent);
/* task 3 */
uint8_t *transaction_hash(
	transaction_t const *transaction,
	uint8_t hash_buf[SHA256_DIGEST_LENGTH]);
/* task 4 */
sig_t *tx_in_sign(
	tx_in_t *in,
	uint8_t const tx_id[SHA256_DIGEST_LENGTH],
	EC_KEY const *sender,
	llist_t *all_unspent);
/* task 5 */
transaction_t *transaction_create(
	EC_KEY const *sender,
	EC_KEY const *receiver,
	uint32_t amount,
	llist_t *all_unspent);
/* task 6 */
int transaction_is_valid(
	transaction_t const *transaction,
	llist_t *all_unspent);
/* task 7 */
transaction_t *coinbase_create(EC_KEY const *receiver, uint32_t block_index);
/* task 8 */
int coinbase_is_valid(transaction_t const *coinbase, uint32_t block_index);
/* task 9 */
void transaction_destroy(transaction_t *transaction);
static int is_spent(unspent_tx_out_t const *utxo, llist_t *transactions);
static unspent_tx_out_t *clone_utxo(unspent_tx_out_t const *utxo);
llist_t *update_unspent(llist_t *transactions, uint8_t block_hash[SHA256_DIGEST_LENGTH], llist_t *all_unspent);

#endif /* TRANSACTION_H */