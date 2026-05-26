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
	verilator --trace -I../rtl/pkg -I../rtl/core --cc $(ALU_RTL) --top-module $(ALU_TOP) --exe $(ALU_TB) --build

run_alu:
	./$(BUILD_DIR)/V$(ALU_TOP) $(ALU_WAVE)

# -------------------------
# Regfile
# -------------------------

REGFILE_TOP  := regfile
REGFILE_RTL  := ../rtl/core/regfile.sv
REGFILE_TB   := ../tb/unit/regfile/regfile_tb.cpp
REGFILE_WAVE := $(WAVE_DIR)/regfile.vcd

.PHONY: build_regfile run_regfile

build_regfile:
	verilator -Wall --trace --cc $(REGFILE_RTL) --top-module $(REGFILE_TOP) --exe $(REGFILE_TB) --build

run_regfile:
	./$(BUILD_DIR)/V$(REGFILE_TOP) $(REGFILE_WAVE)

# -------------------------
# Decoder
# -------------------------

DECODER_TOP  := decoder
DECODER_RTL  := ../rtl/pkg/riscv_pkg.sv ../rtl/core/decoder.sv
DECODER_TB   := ../tb/unit/decoder/decoder_tb.cpp
DECODER_WAVE := $(WAVE_DIR)/decoder.vcd

.PHONY: build_decoder run_decoder

build_decoder:
	verilator --trace -I../rtl/pkg -I../rtl/core --cc $(DECODER_RTL) --top-module $(DECODER_TOP) --exe $(DECODER_TB) --build

run_decoder:
	./$(BUILD_DIR)/V$(DECODER_TOP) $(DECODER_WAVE)

# -------------------------
# SoC Top
# -------------------------

SOC_TOP  := soc_top
SOC_RTL  := ../rtl/pkg/riscv_pkg.sv \
            ../rtl/core/alu.sv \
            ../rtl/core/regfile.sv \
            ../rtl/core/pc.sv \
            ../rtl/core/imm_gen.sv \
            ../rtl/core/decoder.sv \
            ../rtl/core/branch_cmp.sv \
            ../rtl/core/lsu.sv \
            ../rtl/core/hazard_detect.sv \
            ../rtl/core/forwarding_unit.sv \
            ../rtl/core/csr_file.sv \
            ../rtl/core/trap_ctrl.sv \
            ../rtl/core/rv64_core.sv \
            ../rtl/mem/boot_rom.sv \
            ../rtl/mem/dram.sv \
            ../rtl/peripheral/uart.sv \
            ../rtl/peripheral/clint.sv \
            ../rtl/soc_top.sv
ASM_RUNNER := ../tb/integration/asm_test_runner.cpp
ASM_RUNNER_EXE := $(BUILD_DIR)/Vasm_test_runner

.PHONY: build_soc build_asm_runner run_add_test run_branch_test

build_soc:
	verilator --trace -I../rtl/pkg -I../rtl/core -I../rtl/mem -I../rtl/peripheral --cc $(SOC_RTL) --top-module $(SOC_TOP) --exe --build

build_asm_runner: build_soc
	@echo "Building assembly test runner..."
	verilator --trace -I../rtl/pkg -I../rtl/core -I../rtl/mem -I../rtl/peripheral --cc $(SOC_RTL) --top-module $(SOC_TOP) --exe $(ASM_RUNNER) --build -o Vasm_test_runner

# Assembly tests
ASM_BIN_DIR := ../sw/tests/asm
ADD_TEST_BIN := $(ASM_BIN_DIR)/add_test.bin
BRANCH_TEST_BIN := $(ASM_BIN_DIR)/branch_test.bin
LOAD_STORE_TEST_BIN := $(ASM_BIN_DIR)/load_store_test.bin

$(ADD_TEST_BIN):
	$(MAKE) -C ../sw/tests asm/add_test.bin

$(BRANCH_TEST_BIN):
	$(MAKE) -C ../sw/tests asm/branch_test.bin

$(LOAD_STORE_TEST_BIN):
	$(MAKE) -C ../sw/tests asm/load_store_test.bin

run_add_test: build_asm_runner $(ADD_TEST_BIN)
	mkdir -p $(WAVE_DIR)
	$(ASM_RUNNER_EXE) $(ADD_TEST_BIN) 1000 $(WAVE_DIR)/add_test.vcd

run_branch_test: build_asm_runner $(BRANCH_TEST_BIN)
	mkdir -p $(WAVE_DIR)
	$(ASM_RUNNER_EXE) $(BRANCH_TEST_BIN) 1000 $(WAVE_DIR)/branch_test.vcd

run_load_store_test: build_asm_runner $(LOAD_STORE_TEST_BIN)
	mkdir -p $(WAVE_DIR)
	$(ASM_RUNNER_EXE) $(LOAD_STORE_TEST_BIN) 1000 $(WAVE_DIR)/load_store_test.vcd

# -------------------------
# Comprehensive Test Suite
# -------------------------

TEST_SUITE_RUNNER := ../tb/integration/test_suite_runner.cpp
TEST_SUITE_EXE := $(BUILD_DIR)/Vtest_suite_runner

.PHONY: build_test_suite run_tests all_tests

build_test_suite: $(wildcard ../sw/tests/asm/*.bin)
	@echo "Building comprehensive test suite runner..."
	verilator --trace -I../rtl/pkg -I../rtl/core -I../rtl/mem -I../rtl/peripheral \
		-Wno-IMPLICITSTATIC -Wno-WIDTHTRUNC -Wno-WIDTHEXPAND -Wno-UNOPTFLAT -Wno-LATCH \
		--cc $(SOC_RTL) --top-module $(SOC_TOP) --exe $(TEST_SUITE_RUNNER) --build -o Vtest_suite_runner

run_tests: build_test_suite
	@echo "Using prebuilt assembly test binaries..."
	@echo "Running comprehensive test suite..."
	mkdir -p $(WAVE_DIR)
	$(TEST_SUITE_EXE)

all_tests: clean run_tests
	@echo "All tests completed!"

