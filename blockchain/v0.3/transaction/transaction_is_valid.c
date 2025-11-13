#include "blockchain.h"
#include <string.h> /* For memcmp */
#include <stdio.h>  /* For fprintf, stderr */

/**
 * calculate_output_amount - calculates the total amount of all outputs
 * @transaction: transaction to verify
 *
 * Return: sum of output amounts
 */
uint32_t calculate_output_amount(transaction_t const *transaction)
{
	int i, out_size;
	uint32_t output_amount = 0;
	tx_out_t *out = NULL;

	/*
	 * FIX 1: This must be a loop to sum ALL outputs,
	 * not just the first two.
	 */
	out_size = llist_size(transaction->outputs);
	for (i = 0; i < out_size; i++)
	{
		out = (tx_out_t *)llist_get_node_at(transaction->outputs, i);
		if (out)
			output_amount += out->amount;
	}

	return (output_amount);
}

/**
 * check_unspent_vs_input - checks whether an unspent tx is part of the input
 * @unspent: individual unspent transaction
 * @input: individual input transaction
 *
 * Return: 1 if matching, 0 otherwise
 */
int check_unspent_vs_input(unspent_tx_out_t *unspent, tx_in_t *input)
{
	if ((memcmp(input->block_hash, unspent->block_hash, SHA256_DIGEST_LENGTH)
		== 0) &&
		(memcmp(input->tx_id, unspent->tx_id, SHA256_DIGEST_LENGTH) == 0) &&
		(memcmp(input->tx_out_hash, unspent->out.hash, SHA256_DIGEST_LENGTH)
		== 0))
		return (1);
	return (0);
}

/**
 * verify_inputs - verifies inputs used in transaction vs unspent
 * @transaction: transaction to verify
 * @all_unspent: is the list of all unspent transaction outputs to date
 * @input_amount: sum of matching amounts in inputs
 *
 * Return: 1 on sucess, 0 otherwise
 */
int verify_inputs(transaction_t const *transaction, llist_t *all_unspent,
	uint32_t *input_amount)
{
	int i, j, inputs_size, all_unspent_size, found;
	tx_in_t *input = NULL;
	unspent_tx_out_t *unspent = NULL;
	EC_KEY *unspent_key = NULL;

	inputs_size = llist_size(transaction->inputs);
	all_unspent_size = llist_size(all_unspent);

	for (i = 0; i < inputs_size; i++)
	{
		input = (tx_in_t *)llist_get_node_at(transaction->inputs, i);
		if (!input)
			return (0); /* Should not happen, but good to check */
		
		found = 0;
		for (j = 0; j < all_unspent_size; j++)
		{
			unspent = (unspent_tx_out_t *)llist_get_node_at(all_unspent, j);
			if (!unspent)
				continue;

			if (check_unspent_vs_input(unspent, input))
			{
				found = 1;
				/*
				 * FIX: Create and free the key *only* when a match is found
				 * to prevent memory leaks or double-frees.
				 */
				unspent_key = ec_from_pub(unspent->out.pub);
				if (!unspent_key)
					return (0); /* Key creation failed */
				
				if (!ec_verify(unspent_key, transaction->id,
					SHA256_DIGEST_LENGTH, &input->sig))
				{
					fprintf(stderr, "!ec_verify\n");
					EC_KEY_free(unspent_key);
					return (0);
				}
				*input_amount += unspent->out.amount;
				EC_KEY_free(unspent_key); /* Free the key after use */
				break; /* Stop inner loop, move to next input */
			}
		}
		if (!found)
		{
			fprintf(stderr, "!found\n");
			return (0);
		}
	}
	return (1);
}

/**
 * transaction_is_valid - checks whether a transaction is valid
 * @transaction: transaction to verify
 * @all_unspent: is the list of all unspent transaction outputs to date
 *
 * Return: 1 if the transaction is valid, 0 otherwise
 */
int transaction_is_valid(transaction_t const *transaction,
	llist_t *all_unspent)
{
	uint8_t test_hash[SHA256_DIGEST_LENGTH] = {0};
	uint32_t output_amount = 0, input_amount = 0;

	/*
	 * FIX: Add NULL checks for inputs/outputs to prevent segfaults
	 * on malformed transactions.
	 */
	if (!transaction || !all_unspent || !transaction->inputs ||
		!transaction->outputs)
	{
		fprintf(stderr, "!params\n");
		return (0);
	}

	transaction_hash(transaction, test_hash);
	if (memcmp(test_hash, transaction->id, SHA256_DIGEST_LENGTH) != 0)
	{
		fprintf(stderr, "!transaction hash\n");
		return (0);
	}

	if (llist_size(transaction->inputs) == 0)
		return (0); /* Regular transactions must have inputs */

	if (!verify_inputs(transaction, all_unspent, &input_amount))
		return (0);

	output_amount = calculate_output_amount(transaction);

	if (output_amount != input_amount)
	{
		fprintf(stderr, "outputs != inputs outputs: %d, inputs: %d\n",
			output_amount, input_amount);
		fprintf(stderr, "output_size: %d, input_size: %d\n",
			llist_size(transaction->outputs), llist_size(transaction->inputs));
		return (0);
	}

	return (1);
}
