CPPFLAGS = -std=c++17 -Wall -g -I.
CPPFLAGS += -I../externalLibraries/INIReader \
            -I../externalLibraries/spdlog \
            -I../externalLibraries/sqlite \
            -I../serviceFramework \
			-I../common \
            -I ${ITEMS_OATPP_INCLUDE_DIR}

LIB_DIRS = -L ../lib/ -L /usr/local/lib/oatpp-1.3.0

LIBRARIES = -lServiceFramework -loatpp -lSqliteLibrary

OBJS = AccountsSvc.o \
       ../common/UUID.o \
       Sha256.o \
	   SqliteInterface.o \
       serviceModules/StartupModule.o

ACCOUNTSSVC = ${ITEMS_BIN_DIR}/accountsSvc

$(ACCOUNTSSVC): $(OBJS)
	$(MAKE) -C ../externalLibraries/sqlite
	g++ -o $(ACCOUNTSSVC) $(OBJS) $(LIB_DIRS) $(LIBRARIES)

all: $(ACCOUNTSSVC)

clean:
	$(MAKE) -C ../externalLibraries/sqlite clean
	$(RM) $(ACCOUNTSSVC) $(OBJS)
