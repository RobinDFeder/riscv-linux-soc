#include "Valu.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

#include <cstdint>
#include <iostream>
#include <string>

static vluint64_t sim_time = 0;

enum AluOp : uint8_t {
    ALU_ADD    = 0,
    ALU_SUB    = 1,
    ALU_AND    = 2,
    ALU_OR     = 3,
    ALU_XOR    = 4,
    ALU_SLL    = 5,
    ALU_SRL    = 6,
    ALU_SRA    = 7,
    ALU_SLT    = 8,
    ALU_SLTU   = 9,
    ALU_COPY_B = 10
};

static void eval_and_dump(Valu* dut, VerilatedVcdC* trace) {
    dut->eval();

    if (trace) {
        trace->dump(sim_time);
    }

    sim_time++;
}

static bool check(
    Valu* dut,
    VerilatedVcdC* trace,
    const std::string& name,
    uint64_t a,
    uint64_t b,
    uint8_t op,
    uint64_t expected
) {
    dut->a_i = a;
    dut->b_i = b;
    dut->op_i = op;

    eval_and_dump(dut, trace);

    if (dut->result_o != expected) {
        std::cerr << "FAIL " << name
                  << ": a=0x" << std::hex << a
                  << " b=0x" << b
                  << " expected=0x" << expected
                  << " got=0x" << dut->result_o
                  << std::dec << std::endl;
        return false;
    }

    bool expected_zero = (expected == 0);
    if (dut->zero_o != expected_zero) {
        std::cerr << "FAIL " << name
                  << ": zero_o expected " << expected_zero
                  << " got " << static_cast<int>(dut->zero_o)
                  << std::endl;
        return false;
    }

    return true;
}

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);

    const char* wave_path = "waves/alu.vcd";
    if (argc >= 2) {
        wave_path = argv[1];
    }

    Valu* dut = new Valu;

    Verilated::traceEverOn(true);
    VerilatedVcdC* trace = new VerilatedVcdC;
    dut->trace(trace, 99);
    trace->open(wave_path);

    bool pass = true;

    pass &= check(dut, trace, "ADD basic", 10, 20, ALU_ADD, 30);
    pass &= check(dut, trace, "ADD wrap", UINT64_MAX, 1, ALU_ADD, 0);

    pass &= check(dut, trace, "SUB basic", 20, 10, ALU_SUB, 10);
    pass &= check(dut, trace, "SUB zero", 20, 20, ALU_SUB, 0);

    pass &= check(dut, trace, "AND", 0b1100, 0b1010, ALU_AND, 0b1000);
    pass &= check(dut, trace, "OR",  0b1100, 0b1010, ALU_OR,  0b1110);
    pass &= check(dut, trace, "XOR", 0b1100, 0b1010, ALU_XOR, 0b0110);

    pass &= check(dut, trace, "SLL", 1, 4, ALU_SLL, 16);
    pass &= check(dut, trace, "SRL", 16, 2, ALU_SRL, 4);

    pass &= check(
        dut,
        trace,
        "SRA negative",
        0x8000000000000000ULL,
        4,
        ALU_SRA,
        0xF800000000000000ULL
    );

    pass &= check(dut, trace, "SLT signed true",  static_cast<uint64_t>(-1), 1, ALU_SLT, 1);
    pass &= check(dut, trace, "SLT signed false", 1, static_cast<uint64_t>(-1), ALU_SLT, 0);

    pass &= check(dut, trace, "SLTU unsigned true", 1, 2, ALU_SLTU, 1);
    pass &= check(dut, trace, "SLTU unsigned false", UINT64_MAX, 1, ALU_SLTU, 0);

    pass &= check(dut, trace, "COPY_B", 123, 456, ALU_COPY_B, 456);

    dut->final();

    trace->close();
    delete trace;
    delete dut;

    if (!pass) {
        std::cerr << "ALU TEST FAILED" << std::endl;
        return 1;
    }

    std::cout << "ALU TEST PASSED" << std::endl;
    std::cout << "Waveform written to " << wave_path << std::endl;

    return 0;
}