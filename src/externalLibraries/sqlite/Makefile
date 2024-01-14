#
# Makefile for libServiceFramework.a
#

CPPFLAGS = -Wall -g -I.

OBJS = sqlite3.o

ARCHIVE = ${ITEMS_BIN_DIR}/libSqliteLibrary.a

$(ARCHIVE): $(OBJS)
	mkdir -p $(ITEMS_BIN_DIR)
	ar -r $(ARCHIVE) $(OBJS)

all: $(ARCHIVE)

clean:
	$(RM) $(ARCHIVE) $(OBJS)
