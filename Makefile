PROJECT_NAME=membrane-surface
PLATFORM=$(shell uname)
PLATFORM_CFLAGS=
ifeq ($(PLATFORM), Linux)
PLATFORM_ID=linux
PLATFORM_CFLAGS=-DPLATFORM_LINUX=1
CC=g++
endif

ifndef BUILD_ID
BUILD_ID=0-ffffffff
endif
ifndef PLATFORM_ID
PLATFORM_ID=unknown
endif

SDL2_PREFIX?= /usr
JPEG_PREFIX?= /usr
LIBPNG_PREFIX?= /usr
SDL2_IMAGE_PREFIX?= /usr
ZLIB_PREFIX?= /usr
FREETYPE_PREFIX?= /usr
OPENSSL_PREFIX?= /usr
CURL_PREFIX?= /usr

SRCPATH=$(PWD)/src
O=App.o \
	Buffer.o \
	Button.o \
	CardView.o \
	Chip.o \
	Color.o \
	ComboBox.o \
	Font.o \
	HashMap.o \
	Image.o \
	ImageWindow.o \
	Input.o \
	Ipv4Address.o \
	json-builder.o \
	Json.o \
	json-parser.o \
	Label.o \
	LabelWindow.o \
	ListView.o \
	Log.o \
	Main.o \
	MainUi.o \
	MathUtil.o \
	MediaUtil.o \
	Menu.o \
	Network.o \
	OsUtil.o \
	Panel.o \
	Position.o \
	Prng.o \
	ProgressBar.o \
	Resource.o \
	ScrollBar.o \
	ScrollView.o \
	SharedBuffer.o \
	Slider.o \
	SpriteGroup.o \
	SpriteHandle.o \
	Sprite.o \
	StdString.o \
	StringList.o \
	SystemInterface.o \
	TextArea.o \
	TextField.o \
	Toggle.o \
	Toolbar.o \
	TooltipWindow.o \
	UiConfiguration.o \
	Ui.o \
	UiStack.o \
	UiText.o \
	WidgetHandle.o \
	Widget.o

VPATH=$(SRCPATH)
CFLAGS=$(PLATFORM_CFLAGS) \
	-I$(SDL2_PREFIX)/include \
	-I$(SDL2_PREFIX)/include/SDL2 \
	-I$(JPEG_PREFIX)/include \
	-I$(LIBPNG_PREFIX)/include \
	-I$(SDL2_IMAGE_PREFIX)/include \
	-I$(ZLIB_PREFIX)/include \
	-I$(FREETYPE_PREFIX)/include \
	-I$(FREETYPE_PREFIX)/include/freetype2 \
	-I$(OPENSSL_PREFIX)/include \
	-I$(CURL_PREFIX)/include \
	-I$(SRCPATH) $(EXTRA_CFLAGS)
LDFLAGS=-L$(SDL2_PREFIX)/lib \
	-L$(JPEG_PREFIX)/lib \
	-L$(LIBPNG_PREFIX)/lib \
	-L$(SDL2_IMAGE_PREFIX)/lib \
	-L$(ZLIB_PREFIX)/lib \
	-L$(FREETYPE_PREFIX)/lib \
	-L$(OPENSSL_PREFIX)/lib \
	-L$(CURL_PREFIX)/lib \
	$(EXTRA_LDFLAGS)
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
	rm -f $(O) $(PROJECT_NAME) $(SRCPATH)/BuildConfig.h

$(SRCPATH)/BuildConfig.h:
	@echo "#ifndef BUILD_CONFIG_H" > $@
	@echo "#define BUILD_CONFIG_H" >> $@
	@echo "#ifndef BUILD_ID" >> $@
	@echo "#define BUILD_ID \"$(BUILD_ID)\"" >> $@
	@echo "#endif" >> $@
	@echo "#ifndef PLATFORM_ID" >> $@
	@echo "#define PLATFORM_ID \"$(PLATFORM_ID)\"" >> $@
	@echo "#endif" >> $@
	@echo "#endif" >> $@

$(PROJECT_NAME): $(SRCPATH)/BuildConfig.h $(O)
	$(CC) -o $@ $(O) $(LDFLAGS) $(LD_DYNAMIC_LIBS)

.SECONDARY: $(O)

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

json-parser.o: json-parser.c
	$(CC) -o $@ -g -c $<

json-builder.o: json-builder.c
	$(CC) -o $@ -g -c $<
