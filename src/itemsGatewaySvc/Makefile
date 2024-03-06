CPPFLAGS = -std=c++17 -Wall -g -I.
CPPFLAGS += -I../externalLibraries/INIReader \
            -I../externalLibraries/spdlog \
            -I../serviceFramework \
			-I../common \
            -I ${ITEMS_OATPP_INCLUDE_DIR}

LIB_DIRS = -L ../lib/ -L /usr/local/lib/oatpp-1.3.0

LIBRARIES = -lServiceFramework -loatpp

OBJS = SessionsManager.o \
       gatewaySvc.o \
	   serviceModules/StartupModule.o \
	   ../common/UUID.o

GATEWAYSVC = ${ITEMS_BIN_DIR}/gatewaySvc

$(GATEWAYSVC): $(OBJS)
	g++ -o $(GATEWAYSVC) $(OBJS) $(LIB_DIRS) $(LIBRARIES)

all: $(GATEWAYSVC)

clean:
	$(RM) $(GATEWAYSVC) $(OBJS)
