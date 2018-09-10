# Settings for iOS.
ifeq ($(TARGET), MACOS)
	CXXFLAGS += \
		-Wno-c++11-narrowing \
		-fno-exceptions \
		-arch $(TARGET_ARCH) \
		-O3
	CCFLAGS += \
		-arch $(TARGET_ARCH) \
		-O3
	LDFLAGS := \
		-framework Accelerate \
		-arch $(TARGET_ARCH)
	OBJDIR := $(OBJDIR)macos/
	LIBDIR := $(LIBDIR)macos/
	BINDIR := $(BINDIR)macos/
	DEPDIR := $(DEPDIR)macos/
endif
