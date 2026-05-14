`timescale 1ns/1ps

module alu (
    input  logic [63:0]        a_i,
    input  logic [63:0]        b_i,
    input  riscv_pkg::alu_op_e op_i,

    output logic [63:0]        result_o,
    output logic               zero_o
);

    always_comb begin
        unique case (op_i)
            riscv_pkg::ALU_ADD:    result_o = a_i + b_i;
            riscv_pkg::ALU_SUB:    result_o = a_i - b_i;
            riscv_pkg::ALU_AND:    result_o = a_i & b_i;
            riscv_pkg::ALU_OR:     result_o = a_i | b_i;
            riscv_pkg::ALU_XOR:    result_o = a_i ^ b_i;
            riscv_pkg::ALU_SLL:    result_o = a_i << b_i[5:0];
            riscv_pkg::ALU_SRL:    result_o = a_i >> b_i[5:0];
            riscv_pkg::ALU_SRA:    result_o = $signed(a_i) >>> b_i[5:0];
            riscv_pkg::ALU_SLT:    result_o = ($signed(a_i) < $signed(b_i)) ? 64'd1 : 64'd0;
            riscv_pkg::ALU_SLTU:   result_o = (a_i < b_i) ? 64'd1 : 64'd0;
            riscv_pkg::ALU_COPY_B: result_o = b_i;
            default:               result_o = 64'h0;
        endcase
    end

    assign zero_o = (result_o == 64'd0);

endmodule
