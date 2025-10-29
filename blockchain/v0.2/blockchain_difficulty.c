#include "blockchain.h"

/**
 * blockchain_difficulty - computes the difficulty to assign to a potential
 * next Block in the Blockchain.
 * @blockchain: points to the Blockchain to analyze
 *
 * Return: the difficulty to be assigned to a potential next Block
 */
uint32_t blockchain_difficulty(blockchain_t const *blockchain)
{
	block_t *latest_block, *adjustment_block;
	size_t size;
	uint64_t expected_time, actual_time;

	if (!blockchain || !blockchain->chain)
		return (0);

	size = llist_size(blockchain->chain);
	if (size == 0)
		return (0);

	latest_block = llist_get_node_at(blockchain->chain, size - 1)->data;

	/* Adjust difficulty only at specified intervals, not for Genesis */
	if (latest_block->info.index == 0 ||
		latest_block->info.index % DIFFICULTY_ADJUSTMENT_INTERVAL != 0)
	{
		return (latest_block->info.difficulty);
	}

	/* Get the block at the start of this adjustment period */
	/* 'size' is latest_block->info.index + 1 */
	adjustment_block = llist_get_node_at(blockchain->chain,
										 size - DIFFICULTY_ADJUSTMENT_INTERVAL)
						   ->data;

	expected_time = (uint64_t)DIFFICULTY_ADJUSTMENT_INTERVAL *
					(uint64_t)BLOCK_GENERATION_INTERVAL;

	actual_time = latest_block->info.timestamp -
				  adjustment_block->info.timestamp;

	if (actual_time < (expected_time / 2))
	{
		/* Generated blocks too fast, increase difficulty */
		return (latest_block->info.difficulty + 1);
	}
	else if (actual_time > (expected_time * 2))
	{
		/* Generated blocks too slow, decrease difficulty */
		/* Prevent underflow */
		if (latest_block->info.difficulty > 0)
			return (latest_block->info.difficulty - 1);
		return (0); /* Already at minimum */
	}

	/* Time is within range, no change */
	return (latest_block->info.difficulty);
}
