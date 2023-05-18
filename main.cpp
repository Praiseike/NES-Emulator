/*
    Author:Praise Ike
    Email: Praiseike123@gmail.com
    Github: https://www.github.com/praiseike
*/

#include <iostream>
#include <chrono>
#include <thread>

uint16_t pc;
uint8_t sp;
uint8_t st;
uint8_t a;
uint8_t x;
uint8_t y;
uint8_t flipped = 0;

uint8_t ram[64000]; // 64K RAM

uint8_t cycles; // global variable to store cycles;

// tick() runs the clock
void tick(){
    while(cycles != 0){
        cycles --;
        // in order to emulate the 1.79MHz cpu speed 
        // we need to sleep for 558.65922 nanoseconds
        std::this_thread::sleep_for(std::chrono::duration<double, std::ratio<1,1000000000>>(558.65922));

    }
    flipped =0;
}

// reset() resets the cpu to start executing the the given reset vector
void reset(uint16_t vector){
    pc = vector;
    tick();
}


uint8_t get_flag(char flag){
    switch(flag){
        case 'c': return (st & (uint8_t)(0x01)); // mask 00000001b
        case 'z': return (st & (uint8_t)(0x02)); // mask 00000010b
        case 'i': return (st & (uint8_t)(0x04)); // mask 00000100b
        case 'd': return (st & (uint8_t)(0x08)); // mask 00001000b
        case 'b': return (st & (uint8_t)(0x10)); // mask 00010000b
        case 'u': return (st & (uint8_t)(0x20)); // mask 00100000b
        case 'v': return (st & (uint8_t)(0x40)); // mask 01000000b
        case 'n': return (st & (uint8_t)(0x80)); // mask 10000000b
        default:
            return 0;
    }
}



void set_flag(char flag, uint8_t bit){
    if(bit){
        switch(flag){
            case 'c': st |= (uint8_t)(0x01); break; // mask 00000001b
            case 'z': st |= (uint8_t)(0x02); break; // mask 00000010b
            case 'i': st |= (uint8_t)(0x04); break; // mask 00000100b
            case 'd': st |= (uint8_t)(0x08); break; // mask 00001000b
            case 'b': st |= (uint8_t)(0x10); break; // mask 00010000b
            case 'u': st |= (uint8_t)(0x20); break; // mask 00100000b
            case 'v': st |= (uint8_t)(0x40); break; // mask 01000000b
            case 'n': st |= (uint8_t)(0x80); break; // mask 10000000b
            default: break;
        }

    }else{
        switch(flag){
            case 'c': st &= ~(uint8_t)(0x01); break; // mask 00000001b
            case 'z': st &= ~(uint8_t)(0x02); break; // mask 00000010b
            case 'i': st &= ~(uint8_t)(0x04); break; // mask 00000100b
            case 'd': st &= ~(uint8_t)(0x08); break; // mask 00001000b
            case 'b': st &= ~(uint8_t)(0x10); break; // mask 00010000b
            case 'u': st &= ~(uint8_t)(0x20); break; // mask 00100000b
            case 'v': st &= ~(uint8_t)(0x40); break; // mask 01000000b
            case 'n': st &= ~(uint8_t)(0x80); break; // mask 10000000b
            default: break;
        }
    }
}

void interrupt(){
    if(get_flag('i')){
        return;
    }
    // store the progrma counter on the stack (most store high byte then low byte seperately, bucause the program couter is 2 bytes)

    uint8_t hi_byte = (uint8_t)(pc >> 8);
    uint8_t lo_byte = (uint8_t)pc;
    
    ram[0x0100 + (sp--)] = hi_byte;
    ram[0x0100 + (sp--)] = lo_byte;

    // store the status register on the stack
    ram[0x0100 + (sp--)] = st;

    // read in the new program counter from a hardcoded address
    lo_byte = ram[0xfffe];
    hi_byte = ram[0xffff];
    pc = ((hi_byte) << 8) | lo_byte;

    // set the flags the interrupt and break flags;
    set_flag('i',1);
    set_flag('b',1);


}

uint16_t zp(){
    uint16_t address;
    address = (uint8_t)ram[pc++];
    return address;
}

uint16_t zpx(){
    uint16_t address;
    uint8_t zp_addr = ram[pc++];
    zp_addr += x;
    // values greater than 0xff will wrap to an 8bit value
    // eg 0x0042 + 0x80 => 0x0172 ==>  0x0072
    address = (uint8_t)zp_addr;
    return address;
}

uint16_t zpy(){
    uint16_t address;
    uint8_t zp_addr = ram[pc++];
    zp_addr += y;
    // values greater than 0xff will wrap to an 8bit value
    // eg 0x0042 + 0x80 => 0x0172 ==>  0x0072
    address = (uint8_t)zp_addr;
    return address;
}

uint16_t abs(){
    uint16_t address;
    uint8_t lo_byte = ram[pc++];
    uint8_t hi_byte = ram[pc++];
    address = ((hi_byte << 8) | lo_byte);
    return address;
}

uint16_t absx(){
    uint16_t address;
    uint8_t lo_byte = ram[pc++];
    uint8_t hi_byte = ram[pc++];
    address = (( hi_byte << 8) | lo_byte);
    address += x;
    if(hi_byte < (uint8_t)((address & 0xff00) >> 8)){
        flipped = 1;
    }
    return address;
}

uint16_t absy(){
    uint16_t address;
    uint8_t lo_byte = ram[pc++];
    uint8_t hi_byte = ram[pc++];
    address = (( hi_byte << 8) | lo_byte);
    address += y;
    return address;
}

uint16_t ind(){
    uint16_t address;
    uint8_t lo_byte = ram[pc++];
    uint8_t hi_byte = ram[pc++];
    address = (( hi_byte << 8) | lo_byte);
    // return address for the jmp instruction 
    return address;
}

uint16_t idx(){
    uint16_t address;
    uint8_t supplied_zp_addr = ram[pc++];
    uint8_t offset = x + supplied_zp_addr;

    uint8_t lo_byte = ram[offset];
    uint8_t hi_byte = ram[offset+1];
    address = (( hi_byte << 8) | lo_byte);
    return address;
}

uint16_t idy(){
    uint16_t address;
    uint8_t lo_byte_loc = ram[pc++];
    uint8_t lo_byte = ram[lo_byte_loc];
    uint8_t hi_byte = ram[lo_byte_loc+1];
    address = (( hi_byte << 8) | lo_byte);
    address += y;
    return address;
}

/* 
    Opcode $AD
    Instruction: LDA
    Addr Mode: ABS
*/
void _ad(){
    uint16_t address = abs();
    a = ram[address];
    cycles = 4;
    if(flipped) {
        cycles += 1;
    }

    // check if high bit of accumulator is 1
    // if so possible negative value
    set_flag('n', a & 0x80);
    set_flag('z',!a); // check if accumulator is 0
    cycles = 4;
}

/*
    Opcode $F8
    Instruction: SED
    Addr Mode: IMP
*/
void _f8(){
    set_flag('d',1);
    cycles = 2;
}

/* 
    Opcode: $15
    Instruction: ORA
    Addr Mode: ZP,X
*/

void _15(){
    uint16_t address = zpx();
    a |= ram[address];
    cycles = 4;
    set_flag('n',a & 0x80); // check if a is negative
    set_flag('z',!a);
    cycles = 4;
}

/*
    Opcode: $11
    Instruction: ORA
    Addr Mode: IDY
*/
void _11(){
    uint16_t address = idy();
    a |= ram[address];
    cycles = 4;
    if(flipped){
        cycles+=1;
    }

    set_flag('n', a & 0x80 );
    set_flag('z',a | 0x00);
    cycles = 5 + flipped;
}

/*
    Opcode: $AA
    Instruction: TAX
    Addr Mode: IMP
*/

void __aa(){
    x = a;
    cycles = 2;
}


/*
    Opcode: $65
    Instruction: ADC
    Addr Mode: ZP
*/

void _65(){
    // get the value of add
    uint16_t address = zp();
    uint8_t v = ram[address];

    // record the hi bit of v and a
    uint8_t hi_bit_a = (( a & 0x80 ) >> 7);
    uint8_t hi_bit_v = (( v & 0x80 ) >> 7);

    // perform the addition
    uint16_t res = a + v;
    a = (uint8_t)(res & 0x00ff);

    // add 1 if the carry flag is set
    if(get_flag('c')){
        a += 1;
    }

    // set carry flag if there is a carry
    set_flag('c',res > 255);

    // check for overflow, and set flag if an overflow has occurred
    uint8_t hi_bit_a_res = ((a & 0x80 ) >> 7);
    if(hi_bit_a == hi_bit_v && hi_bit_a != hi_bit_a_res){
        set_flag('v',1);
    }

    // set the other flags

    set_flag('n',hi_bit_a_res);
    set_flag('z',a | 0x00);

    cycles = 3;
}


/*
    OPCODE TABLE
    A mapping between op
*/

void ()
