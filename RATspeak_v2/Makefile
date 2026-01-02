TARGET := RATspeak
OUT_DIR := temp
OUT := $(OUT_DIR)/$(TARGET)

FRONT_SRC := $(wildcard front/*.cpp)
RUNTIME_SRC := $(wildcard runtime/*.cpp)
$(OUT): $(TARGET).cpp $(FRONT_SRC) $(RUNTIME_SRC)
	g++ -g $(TARGET).cpp $(FRONT_SRC) $(RUNTIME_SRC) -o $(OUT)