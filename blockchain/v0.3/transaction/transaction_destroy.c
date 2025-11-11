#include "blockchain.h"

/**
 * transaction_destroy - deallocates a transaction structure
 * @transaction: points to the transaction to delete
 */
void transaction_destroy(transaction_t *transaction)
{
	if (!transaction)
		return;

	/*
	 * Free the list of inputs.
	 * The '1' signifies to also free the data in each node (the tx_in_t).
	 */
	llist_destroy(transaction->inputs, 1, free);

	/*
	 * Free the list of outputs.
	 * The '1' signifies to also free the data in each node (the tx_out_t).
	 */
	llist_destroy(transaction->outputs, 1, free);

	/* Free the transaction struct itself */
	free(transaction);
}
