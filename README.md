# rv64ima-linux-cpu

A pipelined 64-bit RISC-V CPU (RV64IMAF) with MMU, privilege architecture, and cache hierarchy — designed to boot Linux. Fully verified in simulation with OpenLane synthesis targeting SKY130.

## Architecture

- **ISA**: RV64IMAF (Integer, Multiply/Divide, Atomics, Single-Precision Float)
- **Pipeline**: 5-stage in-order (IF, ID, EX, MEM, WB)
- **Privilege**: M/S/U modes with full trap delegation
- **Virtual Memory**: Sv39 with hardware page table walker and TLB
- **Caches**: Separate I-cache and D-cache, 4-way set-associative, write-back
- **Peripherals**: CLINT, PLIC, UART (16550-compatible)
- **Target**: Linux boot with OpenSBI + BusyBox initramfs

## Directory Structure

```
rtl/            Synthesizable RTL (SystemVerilog)
tb/             Testbenches (Verilator C++)
sim/            Simulation scripts and configs
sw/             Software (tests, bootrom, OpenSBI, Linux, device tree)
synth/          Synthesis and physical design (OpenLane, SKY130)
docs/           Architecture documentation
scripts/        Utility scripts
```

## Toolchain Requirements

- Verilator (5.x+)
- RISC-V GNU Toolchain (riscv64-unknown-elf-gcc)
- GTKWave
- OpenLane 2 + SKY130 PDK (for synthesis phases)

## Building and Simulating

```bash
# Run a bare-metal test
make -C sim test TEST=sw/tests/asm/add_test.S

# Run RV64I compliance tests
./scripts/run_compliance.sh

# Boot Linux (takes hours in simulation)
./scripts/boot_linux.sh
```

## License

MIT
