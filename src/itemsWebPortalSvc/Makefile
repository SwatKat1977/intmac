CPPFLAGS = -std=c++17 -Wall -g -I.
CPPFLAGS += -I../externalLibraries/INIReader \
			-I../externalLibraries/spdlog \
            -I../serviceFramework \
			-I../common \
			-I ${ITEMS_OATPP_INCLUDE_DIR} \
			-I ${ITEMS_INJA_INCLUDE_DIR}

LIB_DIRS = -L ../lib/ -L /usr/local/lib/oatpp-1.3.0

LIBRARIES = -lServiceFramework -loatpp

OBJS = WebPortalSvc.o \
       ../common/PathHelpers.o \
       WebRoute.o \
       serviceModules/StartupModule.o \
	   controllers/RootController.o

WEBPORTALSVC = ${ITEMS_BIN_DIR}/webPortalSvc

$(WEBPORTALSVC): $(OBJS)
	g++ -o $(WEBPORTALSVC) $(OBJS) $(LIB_DIRS) $(LIBRARIES)

all: $(WEBPORTALSVC)

clean:
	$(RM) $(WEBPORTALSVC) $(OBJS)
