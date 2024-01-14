#
# Makefile for libServiceFramework.a
#

CPPFLAGS = -Wall -g -I.

LIB_DIR = ../../lib/

OBJS = sqlite3.o

ARCHIVE = $(LIB_DIR)libSqliteLibrary.a

$(ARCHIVE): $(OBJS)
	mkdir -p $(LIB_DIR)
	ar -r $(ARCHIVE) $(OBJS)

all: $(ARCHIVE)

clean:
	$(RM) $(ARCHIVE) $(OBJS)
