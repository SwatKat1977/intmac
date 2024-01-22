CPPFLAGS = -std=c++17 -Wall -g -I.
CPPFLAGS += -I../externalLibraries/INIReader \
            -I../externalLibraries/spdlog \
            -I../serviceFramework \
			-I../common \
            -I ${ITEMS_OATPP_INCLUDE_DIR}

LIB_DIRS = -L ../lib/ -L /usr/local/lib/oatpp-1.3.0

LIBRARIES = -lServiceFramework -loatpp

OBJS = ItemsCMSSvc.o

CMSSVC = ${ITEMS_BIN_DIR}/itemsCMSSvc

$(CMSSVC): $(OBJS)
	g++ -o $(CMSSVC) $(OBJS) $(LIB_DIRS) $(LIBRARIES)

all: $(CMSSVC)

clean:
	$(RM) $(CMSSVC) $(OBJS)
