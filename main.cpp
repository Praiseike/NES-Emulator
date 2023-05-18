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
    A mapping between opcode and functions
*/

void (*opcodes[256])();

void _00(); void _01(); void _02(); void _03(); void _04(); void _05(); void _06(); void _07();
void _08(); void _09(); void _0a(); void _0b(); void _0c(); void _0d(); void _0e(); void _0f();
void _10(); void _11(); void _12(); void _13(); void _14(); void _15(); void _16(); void _17();
void _18(); void _19(); void _1a(); void _1b(); void _1c(); void _1d(); void _1e(); void _1f();
void _20(); void _21(); void _22(); void _23(); void _24(); void _25(); void _26(); void _27();
void _28(); void _29(); void _2a(); void _2b(); void _2c(); void _2d(); void _2e(); void _2f();
void _30(); void _31(); void _32(); void _33(); void _34(); void _35(); void _36(); void _37();
void _38(); void _39(); void _3a(); void _3b(); void _3c(); void _3d(); void _3e(); void _3f();
void _40(); void _41(); void _42(); void _43(); void _44(); void _45(); void _46(); void _47();
void _48(); void _49(); void _4a(); void _4b(); void _4c(); void _4d(); void _4e(); void _4f();
void _50(); void _51(); void _52(); void _53(); void _54(); void _55(); void _56(); void _57();
void _58(); void _59(); void _5a(); void _5b(); void _5c(); void _5d(); void _5e(); void _5f();
void _60(); void _61(); void _62(); void _63(); void _64(); void _65(); void _66(); void _67();
void _68(); void _69(); void _6a(); void _6b(); void _6c(); void _6d(); void _6e(); void _6f();
void _70(); void _71(); void _72(); void _73(); void _74(); void _75(); void _76(); void _77();
void _78(); void _79(); void _7a(); void _7b(); void _7c(); void _7d(); void _7e(); void _7f();
void _80(); void _81(); void _82(); void _83(); void _84(); void _85(); void _86(); void _87();
void _88(); void _89(); void _8a(); void _8b(); void _8c(); void _8d(); void _8e(); void _8f();
void _90(); void _91(); void _92(); void _93(); void _94(); void _95(); void _96(); void _97();
void _98(); void _99(); void _9a(); void _9b(); void _9c(); void _9d(); void _9e(); void _9f();
void _a0(); void _a1(); void _a2(); void _a3(); void _a4(); void _a5(); void _a6(); void _a7();
void _a8(); void _a9(); void _aa(); void _ab(); void _ac(); void _ad(); void _ae(); void _af();
void _b0(); void _b1(); void _b2(); void _b3(); void _b4(); void _b5(); void _b6(); void _b7();
void _b8(); void _b9(); void _ba(); void _bb(); void _bc(); void _bd(); void _be(); void _bf();
void _c0(); void _c1(); void _c2(); void _c3(); void _c4(); void _c5(); void _c6(); void _c7();
void _c8(); void _c9(); void _ca(); void _cb(); void _cc(); void _cd(); void _ce(); void _cf();
void _d0(); void _d1(); void _d2(); void _d3(); void _d4(); void _d5(); void _d6(); void _d7();
void _d8(); void _d9(); void _da(); void _db(); void _dc(); void _dd(); void _de(); void _df();
void _e0(); void _e1(); void _e2(); void _e3(); void _e4(); void _e5(); void _e6(); void _e7();
void _e8(); void _e9(); void _ea(); void _eb(); void _ec(); void _ed(); void _ee(); void _ef();
void _f0(); void _f1(); void _f2(); void _f3(); void _f4(); void _f5(); void _f6(); void _f7();
void _f8(); void _f9(); void _fa(); void _fb(); void _fc(); void _fd(); void _fe(); void _ff();

// then define a procedure for unused op codes
void _na(){
    return;
}

// start assigning opcode, function pairs;
opcodes[0x00] = _00; opcodes[0x01] = _01; opcodes[0x02] = _02; opcodes[0x03] = _03; opcodes[0x04] = _04; opcodes[0x05] = _05; opcodes[0x06] = _06; opcodes[0x07] = _07;
opcodes[0x08] = _08; opcodes[0x09] = _09; opcodes[0x0a] = _0a; opcodes[0x0b] = _0b; opcodes[0x0c] = _0c; opcodes[0x0d] = _0d; opcodes[0x0e] = _0e; opcodes[0x0f] = _0f;
opcodes[0x10] = _10; opcodes[0x11] = _11; opcodes[0x12] = _12; opcodes[0x13] = _13; opcodes[0x14] = _14; opcodes[0x15] = _15; opcodes[0x16] = _16; opcodes[0x17] = _17;
opcodes[0x18] = _18; opcodes[0x19] = _19; opcodes[0x1a] = _1a; opcodes[0x1b] = _1b; opcodes[0x1c] = _1c; opcodes[0x1d] = _1d; opcodes[0x1e] = _1e; opcodes[0x1f] = _1f;
opcodes[0x20] = _20; opcodes[0x21] = _21; opcodes[0x22] = _22; opcodes[0x23] = _23; opcodes[0x24] = _24; opcodes[0x25] = _25; opcodes[0x26] = _26; opcodes[0x27] = _27;
opcodes[0x28] = _28; opcodes[0x29] = _29; opcodes[0x2a] = _2a; opcodes[0x2b] = _2b; opcodes[0x2c] = _2c; opcodes[0x2d] = _2d; opcodes[0x2e] = _2e; opcodes[0x2f] = _2f;
opcodes[0x30] = _30; opcodes[0x31] = _31; opcodes[0x32] = _32; opcodes[0x33] = _33; opcodes[0x34] = _34; opcodes[0x35] = _35; opcodes[0x36] = _36; opcodes[0x37] = _37;
opcodes[0x38] = _38; opcodes[0x39] = _39; opcodes[0x3a] = _3a; opcodes[0x3b] = _3b; opcodes[0x3c] = _3c; opcodes[0x3d] = _3d; opcodes[0x3e] = _3e; opcodes[0x3f] = _3f;
opcodes[0x40] = _40; opcodes[0x41] = _41; opcodes[0x42] = _42; opcodes[0x43] = _43; opcodes[0x44] = _44; opcodes[0x45] = _45; opcodes[0x46] = _46; opcodes[0x47] = _47;
opcodes[0x48] = _48; opcodes[0x49] = _49; opcodes[0x4a] = _4a; opcodes[0x4b] = _4b; opcodes[0x4c] = _4c; opcodes[0x4d] = _4d; opcodes[0x4e] = _4e; opcodes[0x4f] = _4f;
opcodes[0x50] = _50; opcodes[0x51] = _51; opcodes[0x52] = _52; opcodes[0x53] = _53; opcodes[0x54] = _54; opcodes[0x55] = _55; opcodes[0x56] = _56; opcodes[0x57] = _57;
opcodes[0x58] = _58; opcodes[0x59] = _59; opcodes[0x5a] = _5a; opcodes[0x5b] = _5b; opcodes[0x5c] = _5c; opcodes[0x5d] = _5d; opcodes[0x5e] = _5e; opcodes[0x5f] = _5f;
opcodes[0x60] = _60; opcodes[0x61] = _61; opcodes[0x62] = _62; opcodes[0x63] = _63; opcodes[0x64] = _64; opcodes[0x65] = _65; opcodes[0x66] = _66; opcodes[0x67] = _67;
opcodes[0x68] = _68; opcodes[0x69] = _69; opcodes[0x6a] = _6a; opcodes[0x6b] = _6b; opcodes[0x6c] = _6c; opcodes[0x6d] = _6d; opcodes[0x6e] = _6e; opcodes[0x6f] = _6f;
opcodes[0x70] = _70; opcodes[0x71] = _71; opcodes[0x72] = _72; opcodes[0x73] = _73; opcodes[0x74] = _74; opcodes[0x75] = _75; opcodes[0x76] = _76; opcodes[0x77] = _77;
opcodes[0x78] = _78; opcodes[0x79] = _79; opcodes[0x7a] = _7a; opcodes[0x7b] = _7b; opcodes[0x7c] = _7c; opcodes[0x7d] = _7d; opcodes[0x7e] = _7e; opcodes[0x7f] = _7f;
opcodes[0x80] = _80; opcodes[0x81] = _81; opcodes[0x82] = _82; opcodes[0x83] = _83; opcodes[0x84] = _84; opcodes[0x85] = _85; opcodes[0x86] = _86; opcodes[0x87] = _87;
opcodes[0x88] = _88; opcodes[0x89] = _89; opcodes[0x8a] = _8a; opcodes[0x8b] = _8b; opcodes[0x8c] = _8c; opcodes[0x8d] = _8d; opcodes[0x8e] = _8e; opcodes[0x8f] = _8f;
opcodes[0x90] = _90; opcodes[0x91] = _91; opcodes[0x92] = _92; opcodes[0x93] = _93; opcodes[0x94] = _94; opcodes[0x95] = _95; opcodes[0x96] = _96; opcodes[0x97] = _97;
opcodes[0x98] = _98; opcodes[0x99] = _99; opcodes[0x9a] = _9a; opcodes[0x9b] = _9b; opcodes[0x9c] = _9c; opcodes[0x9d] = _9d; opcodes[0x9e] = _9e; opcodes[0x9f] = _9f;
opcodes[0xa0] = _a0; opcodes[0xa1] = _a1; opcodes[0xa2] = _a2; opcodes[0xa3] = _a3; opcodes[0xa4] = _a4; opcodes[0xa5] = _a5; opcodes[0xa6] = _a6; opcodes[0xa7] = _a7;
opcodes[0xa8] = _a8; opcodes[0xa9] = _a9; opcodes[0xaa] = _aa; opcodes[0xab] = _ab; opcodes[0xac] = _ac; opcodes[0xad] = _ad; opcodes[0xae] = _ae; opcodes[0xaf] = _af;
opcodes[0xb0] = _b0; opcodes[0xb1] = _b1; opcodes[0xb2] = _b2; opcodes[0xb3] = _b3; opcodes[0xb4] = _b4; opcodes[0xb5] = _b5; opcodes[0xb6] = _b6; opcodes[0xb7] = _b7;
opcodes[0xb8] = _b8; opcodes[0xb9] = _b9; opcodes[0xba] = _ba; opcodes[0xbb] = _bb; opcodes[0xbc] = _bc; opcodes[0xbd] = _bd; opcodes[0xbe] = _be; opcodes[0xbf] = _bf;
opcodes[0xc0] = _c0; opcodes[0xc1] = _c1; opcodes[0xc2] = _c2; opcodes[0xc3] = _c3; opcodes[0xc4] = _c4; opcodes[0xc5] = _c5; opcodes[0xc6] = _c6; opcodes[0xc7] = _c7;
opcodes[0xc8] = _c8; opcodes[0xc9] = _c9; opcodes[0xca] = _ca; opcodes[0xcb] = _cb; opcodes[0xcc] = _cc; opcodes[0xcd] = _cd; opcodes[0xce] = _ce; opcodes[0xcf] = _cf;
opcodes[0xd0] = _d0; opcodes[0xd1] = _d1; opcodes[0xd2] = _d2; opcodes[0xd3] = _d3; opcodes[0xd4] = _d4; opcodes[0xd5] = _d5; opcodes[0xd6] = _d6; opcodes[0xd7] = _d7;
opcodes[0xd8] = _d8; opcodes[0xd9] = _d9; opcodes[0xda] = _da; opcodes[0xdb] = _db; opcodes[0xdc] = _dc; opcodes[0xdd] = _dd; opcodes[0xde] = _de; opcodes[0xdf] = _df;
opcodes[0xe0] = _e0; opcodes[0xe1] = _e1; opcodes[0xe2] = _e2; opcodes[0xe3] = _e3; opcodes[0xe4] = _e4; opcodes[0xe5] = _e5; opcodes[0xe6] = _e6; opcodes[0xe7] = _e7;
opcodes[0xe8] = _e8; opcodes[0xe9] = _e9; opcodes[0xea] = _ea; opcodes[0xeb] = _eb; opcodes[0xec] = _ec; opcodes[0xed] = _ed; opcodes[0xee] = _ee; opcodes[0xef] = _ef;
opcodes[0xf0] = _f0; opcodes[0xf1] = _f1; opcodes[0xf2] = _f2; opcodes[0xf3] = _f3; opcodes[0xf4] = _f4; opcodes[0xf5] = _f5; opcodes[0xf6] = _f6; opcodes[0xf7] = _f7;
opcodes[0xf8] = _f8; opcodes[0xf9] = _f9; opcodes[0xfa] = _fa; opcodes[0xfb] = _fb; opcodes[0xfc] = _fc; opcodes[0xfd] = _fd; opcodes[0xfe] = _fe; opcodes[0xff] = _ff;



int main(){
    return 0;
}
