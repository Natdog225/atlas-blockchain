CFLAGS = -Wall -Wextra -Werror -pedantic -I. -I./transaction -I../../crypto
LDFLAGS = -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread
CC = gcc

# Source files
V03_SRC = blockchain_create.c block_create.c block_destroy.c \
          blockchain_destroy.c block_hash.c blockchain_serialize.c \
          blockchain_deserialize.c block_is_valid.c \
          hash_matches_difficulty.c blockchain_difficulty.c block_mine.c

TX_SRC = transaction/tx_out_create.c transaction/unspent_tx_out_create.c \
         transaction/tx_in_create.c transaction/transaction_hash.c \
         transaction/tx_in_sign.c transaction/transaction_create.c \
         transaction/transaction_is_valid.c transaction/coinbase_create.c \
         transaction/coinbase_is_valid.c transaction/transaction_destroy.c \
         transaction/update_unspent.c

# All source files
SRC = $(V03_SRC) $(TX_SRC)

# Object files
OBJ = $(SRC:.c=.o)

# Library name
NAME = libhblk_blockchain.a

# Default rule
all: $(NAME)

# Library rule
$(NAME): $(OBJ)
	ar -rcs $(NAME) $(OBJ)

# Compile rule
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rules
clean:
	rm -f $(OBJ)

oclean: clean
	rm -f $(NAME)

re: oclean all
.PHONY: all clean oclean re