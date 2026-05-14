TEST ?= alu

WAVE_DIR := waves
LOG_DIR  := logs
GTKW_DIR := wave_cfg
BUILD_DIR := obj_dir

.PHONY: unit wave clean

unit:
	mkdir -p $(WAVE_DIR) $(LOG_DIR)
	$(MAKE) build_$(TEST)
	$(MAKE) run_$(TEST)

wave:
	@if [ -f "$(GTKW_DIR)/$(TEST).gtkw" ]; then \
		gtkwave "$(GTKW_DIR)/$(TEST).gtkw"; \
	else \
		gtkwave "$(WAVE_DIR)/$(TEST).vcd"; \
	fi

clean:
	rm -rf $(BUILD_DIR) $(WAVE_DIR) $(LOG_DIR)

# -------------------------
# ALU
# -------------------------

ALU_TOP  := alu
ALU_RTL  := ../rtl/pkg/riscv_pkg.sv ../rtl/core/alu.sv
ALU_TB   := ../tb/unit/alu/alu_tb.cpp
ALU_WAVE := $(WAVE_DIR)/alu.vcd

.PHONY: build_alu run_alu

build_alu:
	verilator -Wall --trace --cc $(ALU_RTL) --top-module $(ALU_TOP) --exe $(ALU_TB) --build

run_alu:
	./$(BUILD_DIR)/V$(ALU_TOP) $(ALU_WAVE)