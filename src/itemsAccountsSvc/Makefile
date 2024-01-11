CPPFLAGS = -std=c++17 -Wall -g -I.
CPPFLAGS += -I../externalLibraries/INIReader \
            -I../externalLibraries/spdlog \
            -I../serviceFramework \
            -I ${ITEMS_OATPP_INCLUDE_DIR}

LIB_DIR = ../lib/

OBJS = AccountsSvc.o \
       UUID.o \
       Sha256.o \
       routes/basicAuthenticate.o \
       serviceModules/StartupModule.o

PROG = ./Tutorial

$(PROG): $(OBJS)
	$(MAKE) -C $(OPENGL_DIR)
	$(MAKE) -C $(WAVEFRONT_DIR)
	$(MAKE) -C $(MATRIX_DIR)
	g++ -o $(PROG) $(INCLUDES) $(OBJS) $(LIBS) $(FRAMEWORKS)

all: $(PROG)

clean:
	$(RM) $(PROG) $(OBJS)
	$(MAKE) -C $(OPENGL_DIR) clean
	$(MAKE) -C $(WAVEFRONT_DIR) clean
	$(MAKE) -C $(MATRIX_DIR) clean
