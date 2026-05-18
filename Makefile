# 1. Cấu hình cơ bản
PROJECT_NAME       ?= game
SRC_DIR             = src
OBJ_DIR             = obj

# 2. Phát hiện hệ điều hành (OS Detection)
ifeq ($(OS),Windows_NT)
    # --- CẤU HÌNH CHO WINDOWS ---
    PLATFORM_OS = WINDOWS
    # Giả sử bạn cài Raylib ở C:/raylib trên Windows, hãy sửa lại nếu khác
    RAYLIB_PATH ?= C:/raylib/raylib
    CC           = g++
    EXT          = .exe
    RM           = del /Q /S
    FIX_PATH     = $(subst /,\,$1)
    # Thư viện cho Windows
    LDLIBS       = -lraylib -lopengl32 -lgdi32 -lwinmm -lpthread
    # CFLAGS cho Windows (thêm đường dẫn include)
    INCLUDE_PATHS = -I. -I$(SRC_DIR) -I$(RAYLIB_PATH)/src
    LDFLAGS      = -L$(RAYLIB_PATH)/src
else
    # --- CẤU HÌNH CHO MACOS/LINUX ---
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
        PLATFORM_OS = OSX
        # Tự động lấy đường dẫn Raylib từ Homebrew
        HBREW_PATH   = $(shell brew --prefix raylib)
        RAYLIB_PATH ?= $(HBREW_PATH)
        CC           = clang++
        EXT          =
        RM           = rm -rf
        FIX_PATH     = $1
        # Thư viện Frameworks cho Mac
        LDLIBS       = -lraylib -framework CoreVideo -framework IOKit -framework Cocoa -framework OpenGL
        INCLUDE_PATHS = -I. -I$(SRC_DIR) -I$(RAYLIB_PATH)/include
        LDFLAGS      = -L$(RAYLIB_PATH)/lib
    endif
endif

# 3. Gom tất cả file nguồn .cpp
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# 4. Cờ biên dịch chung
CFLAGS = -Wall -std=c++17 -O2 $(INCLUDE_PATHS)

# --- QUY TẮC BIÊN DỊCH ---

all: $(PROJECT_NAME)

# Liên kết (Link)
$(PROJECT_NAME): $(OBJECTS)
	$(CC) -o $(PROJECT_NAME)$(EXT) $(OBJECTS) $(LDFLAGS) $(LDLIBS)
	@echo "Khoi chay thanh cong~~~~ $(PLATFORM_OS)!"

# Biên dịch (Compile)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

# Tạo thư mục chứa object files
$(OBJ_DIR):
ifeq ($(PLATFORM_OS),WINDOWS)
	if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
else
	mkdir -p $(OBJ_DIR)
endif

# Dọn dẹp
clean:
ifeq ($(PLATFORM_OS),WINDOWS)
	$(RM) $(call FIX_PATH,$(OBJ_DIR)/*.o) $(PROJECT_NAME)$(EXT)
else
	$(RM) $(OBJ_DIR) $(PROJECT_NAME)
endif
	@echo "Da don dep xong!"