#include "blockchain.h"

/**
 * block_destroy - Deletes a block.
 * @block: The block to be deleted.
 */
void block_destroy(block_t *block)
{
	if (block)
		free(block);
}
