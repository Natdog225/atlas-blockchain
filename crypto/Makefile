CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -I. -Wno-deprecated-declarations
LDFLAGS = -lssl -lcrypto
RM = rm -f

# Source files
SRCS = sha256.c ec_create.c ec_to_pub.c ec_from_pub.c ec_save.c ec_load.c ec_sign.c ec_verify.c

# Object files
OBJS = $(SRCS:.c=.o)

# Library name
TARGET = libhblk_crypto.a

# Default rule
all: $(TARGET)

# Rule to create the static library
$(TARGET): $(OBJS)
	ar -rcs $(TARGET) $(OBJS)

# Rule to compile .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	$(RM) $(OBJS) $(TARGET)

# Re-compile rule
re: clean all

.PHONY: all clean re