#ifndef _CPU_HELPERS_H_
#define _CPU_HELPERS_H_

// ********************************************* helper functions **********************************************
extern unsigned char cpu_ram_read(struct cpu *cpu, unsigned char pc);
extern void cpu_ram_write(struct cpu *cpu, int index, unsigned char binary);
extern void cpu_load_default(struct cpu *cpu);
extern void cpu_load_file(struct cpu *cpu, char *file);
extern void print_binary(const unsigned char dec);
extern void print_ir_bin_hex_dec(const unsigned char ir);

// ***************************************** cpu_run support functions *****************************************
extern void ldi(struct cpu *cpu, unsigned char IR, int num_operands, unsigned char *operands);
extern void mul(struct cpu *cpu, unsigned char IR, int num_operands, unsigned char *operands);
extern void prn(struct cpu *cpu, unsigned char IR, int num_operands, unsigned char *operands);

#endif