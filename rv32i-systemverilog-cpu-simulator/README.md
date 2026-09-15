# RV32I SystemVerilog CPU Simulator

This is a simulation-first, single-cycle RV32I processor written in SystemVerilog. It is a complete virtual CPU project: no FPGA board or proprietary toolchain is required.

## What works

- Integer ALU operations, immediate operations, `LUI`, `AUIPC`
- `JAL`, `JALR`, and all six RV32I branch comparisons
- Byte, halfword, and word loads/stores
- 4 KiB byte-addressed data memory
- Memory-mapped UART at `0x10000000`; writing a byte prints a character in the simulator
- A self-checking smoke test that exercises arithmetic, branching, jump/link, memory, and UART output

## Run it

Install [Icarus Verilog](https://steveicarus.github.io/iverilog/) and run:

```sh
make test
```

Expected output contains `H` followed by:

```text
PASS: RV32I SystemVerilog CPU smoke test completed.
```

Optional static lint:

```sh
make lint
```

## Architecture

This first release is deliberately **single-cycle**. It is complete, readable, and testable before adding pipeline complexity. The next milestone is a five-stage version with forwarding and stalls, sharing the same instruction tests.

```text
instruction memory -> decode/register file -> ALU -> data memory -> register file
                                  |                         |
                                  +-------- PC control -----+
```

`rtl/lumenv_core.sv` is the processor; `tb/tb_lumenv.sv` is a self-checking simulation testbench; `programs/smoke_test.S` documents the test program in RISC-V assembly.

## Scope

The core supports the RV32I base integer instructions represented in the implementation. It does not claim support for privileged mode, floating point, atomics, compressed instructions, interrupts, caches, or an MMU.

## Learning reference

The design follows the processor-design progression in the free *Basic Computer Architecture* material by Smruti R. Sarangi: logic/registers/memory, processor design, pipelining, then RISC-V assembly.
