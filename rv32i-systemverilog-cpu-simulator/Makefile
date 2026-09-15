SIM ?= iverilog
OUT := build/lumenv_tb

.PHONY: test clean lint

test:
	mkdir -p build
	$(SIM) -g2012 -Wall -s tb_lumenv -o $(OUT) rtl/lumenv_core.sv tb/tb_lumenv.sv
	vvp $(OUT)

lint:
	verilator --lint-only -Wall --Wno-fatal --timing rtl/lumenv_core.sv tb/tb_lumenv.sv

clean:
	rm -rf build
