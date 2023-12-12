#
# Makefile for libServiceFramework.a
#

CPPFLAGS = -std=c++17 -Wall -g -I.
CPPFLAGS += -I../externalLibraries/INIReader \
            -I../externalLibraries/spdlog \
            -I ${ITEMS_OATPP_INCLUDE_DIR}

LIB_DIR = ../lib/

OBJS = ConfigManager.o \
       ConfigSetup.o \
       ConfigSetupItem.o \
       Platform.o \
       ServiceContext.o

ARCHIVE = $(LIB_DIR)libServiceFramework.a

$(ARCHIVE): $(OBJS)
	mkdir -p $(LIB_DIR)
	ar -r $(ARCHIVE) $(OBJS)

all: $(ARCHIVE)

clean:
	$(RM) $(ARCHIVE) $(OBJS)
