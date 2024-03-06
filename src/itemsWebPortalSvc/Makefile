CPPFLAGS = -std=c++17 -Wall -g -I.
CPPFLAGS += -I../externalLibraries/INIReader \
            -I../externalLibraries/spdlog \
            -I../serviceFramework \
			-I../common \
            -I ${ITEMS_OATPP_INCLUDE_DIR}

LIB_DIRS = -L ../lib/ -L /usr/local/lib/oatpp-1.3.0

LIBRARIES = -lServiceFramework -loatpp

OBJS = WebPortalSvc.o \
	serviceModules/StartupModule.o

WEBPORTALSVC = ${ITEMS_BIN_DIR}/webPortalSvc

$(WEBPORTALSVC): $(OBJS)
	g++ -o $(WEBPORTALSVC) $(OBJS) $(LIB_DIRS) $(LIBRARIES)

all: $(CMSSVC)

clean:
	$(RM) $(CMSSVC) $(OBJS)
