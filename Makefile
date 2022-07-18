PROJECT_NAME=membrane-surface
APPLICATION_NAME="Membrane Surface"
APPLICATION_PACKAGE_NAME=MembraneSurface
PLATFORM=$(shell uname)
PLATFORM_CFLAGS=
ifeq ($(PLATFORM), Linux)
PLATFORM_ID=linux
PLATFORM_CFLAGS=-DPLATFORM_LINUX=1
CC=g++
endif
ifeq ($(PLATFORM), Darwin)
PLATFORM_ID=macos
PLATFORM_CFLAGS=-DPLATFORM_MACOS=1 -std=c++11
CC=g++
endif

ifndef BUILD_ID
BUILD_ID=0-ffffffff
endif
ifndef PLATFORM_ID
PLATFORM_ID=unknown
endif

CURL_PREFIX?= /usr
FREETYPE_PREFIX?= /usr
JPEG_PREFIX?= /usr
LIBPNG_PREFIX?= /usr
LUA_PREFIX?= /usr
OPENSSL_PREFIX?= /usr
SDL2_PREFIX?= /usr
SDL2IMAGE_PREFIX?= /usr
ZLIB_PREFIX?= /usr

SRC_PATH=src
O=App.o \
	Buffer.o \
	Button.o \
	Color.o \
	CountdownWindow.o \
	Font.o \
	HashMap.o \
	IconLabelWindow.o \
	Image.o \
	ImageWindow.o \
	Input.o \
	Ipv4Address.o \
	json-builder.o \
	Json.o \
	json-parser.o \
	Label.o \
	LabelWindow.o \
	Log.o \
	LuaScript.o \
	Main.o \
	MainUi.o \
	MathUtil.o \
	Network.o \
	OsUtil.o \
	Panel.o \
	Position.o \
	Prng.o \
	ProgressBar.o \
	Resource.o \
	SharedBuffer.o \
	SpriteGroup.o \
	SpriteHandle.o \
	Sprite.o \
	StdString.o \
	StringList.o \
	SystemInterface.o \
	TaskGroup.o \
	TextFlow.o \
	TooltipWindow.o \
	UiConfiguration.o \
	Ui.o \
	UiStack.o \
	UiText.o \
	WidgetHandle.o \
	Widget.o

VPATH=$(SRC_PATH)
CFLAGS=$(PLATFORM_CFLAGS) \
	-I$(CURL_PREFIX)/include \
	-I$(FREETYPE_PREFIX)/include \
	-I$(FREETYPE_PREFIX)/include/freetype2 \
	-I$(JPEG_PREFIX)/include \
	-I$(LIBPNG_PREFIX)/include \
	-I$(LUA_PREFIX)/include \
	-I$(OPENSSL_PREFIX)/include \
	-I$(SDL2_PREFIX)/include \
	-I$(SDL2_PREFIX)/include/SDL2 \
	-I$(SDL2IMAGE_PREFIX)/include \
	-I$(ZLIB_PREFIX)/include \
	-I$(SRC_PATH) $(EXTRA_CFLAGS)
LDFLAGS=-L$(CURL_PREFIX)/lib \
	-L$(FREETYPE_PREFIX)/lib \
	-L$(JPEG_PREFIX)/lib \
	-L$(LIBPNG_PREFIX)/lib \
	-L$(LUA_PREFIX)/lib \
	-L$(OPENSSL_PREFIX)/lib \
	-L$(SDL2_PREFIX)/lib \
	-L$(SDL2IMAGE_PREFIX)/lib \
	-L$(ZLIB_PREFIX)/lib \
	$(EXTRA_LDFLAGS)
LD_STATIC_LIBS=$(LUA_PREFIX)/lib/liblua.a
LD_DYNAMIC_LIBS=-lSDL2 \
	-lSDL2_image \
	-lfreetype \
	-ldl \
	-lm \
	-lpthread \
	-lcurl \
	-lssl \
	-lcrypto \
	-lpng \
	-ljpeg \
	-lz

all: $(PROJECT_NAME)

clean:
	rm -f $(O) $(PROJECT_NAME) $(SRC_PATH)/BuildConfig.h

$(SRC_PATH)/BuildConfig.h:
	@echo "#ifndef BUILD_CONFIG_H" > $@
	@echo "#define BUILD_CONFIG_H" >> $@
	@echo "#ifndef BUILD_ID" >> $@
	@echo "#define BUILD_ID \"$(BUILD_ID)\"" >> $@
	@echo "#endif" >> $@
	@echo "#ifndef PLATFORM_ID" >> $@
	@echo "#define PLATFORM_ID \"$(PLATFORM_ID)\"" >> $@
	@echo "#endif" >> $@
	@echo "#ifndef APPLICATION_NAME" >> $@
	@echo "#define APPLICATION_NAME \"$(APPLICATION_NAME)\"" >> $@
	@echo "#endif" >> $@
	@echo "#ifndef APPLICATION_PACKAGE_NAME" >> $@
	@echo "#define APPLICATION_PACKAGE_NAME \"$(APPLICATION_PACKAGE_NAME)\"" >> $@
	@echo "#endif" >> $@
	@echo "#endif" >> $@

$(PROJECT_NAME): $(SRC_PATH)/BuildConfig.h $(O)
	$(CC) -o $@ $(O) $(LD_STATIC_LIBS) $(LDFLAGS) $(LD_DYNAMIC_LIBS)

.SECONDARY: $(O)

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

json-parser.o: json-parser.c
	$(CC) -o $@ -g -c $<

json-builder.o: json-builder.c
	$(CC) -o $@ -g -c $<
