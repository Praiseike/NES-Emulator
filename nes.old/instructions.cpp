#include "cpu.h"


/*
    Opcode: $00
    Instruction: BRK
    Addr Mode: IMP
*/


void  _00(){
	set_flag('b',1);
	cycles = 7;
    return;
}
 
void  _01(){
    return;
}
 
void  _02(){
    return;
}
 
void  _03(){
    return;
}
 
void  _04(){
    return;
}
 
void  _05(){
    return;
}
 
void  _06(){
    return;
}
 
void  _07(){
    return;
}


void  _08(){
    return;
}
 
void  _09(){
    return;
}
 
void  _0a(){
    return;
}
 
void  _0b(){
    return;
}
 
void  _0c(){
    return;
}
 
void  _0d(){
    return;
}
 
void  _0e(){
    return;
}
 
void  _0f(){
    return;
}


void  _10(){
    return;
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

 
void  _12(){
    return;
}
 
void  _13(){
    return;
}
 
void  _14(){
    return;
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
 
void  _16(){
    return;
}
 
void  _17(){
    return;
}


/*
    Opcode: $18
    Instruction: CLC
    Addr Mode: IMP
*/

void  _18(){
	set_flag('c',0);
	cycles = 2;
    return;
}
 
void  _19(){
    return;
}
 
void  _1a(){
    return;
}
 
void  _1b(){
    return;
}
 
void  _1c(){
    return;
}
 
void  _1d(){
    return;
}
 
void  _1e(){
    return;
}
 
void  _1f(){
    return;
}


void  _20(){
    return;
}
 
void  _21(){
    return;
}
 
void  _22(){
    return;
}
 
void  _23(){
    return;
}
 
void  _24(){
    return;
}
 
void  _25(){
    return;
}
 
void  _26(){
    return;
}
 
void  _27(){
    return;
}


void  _28(){
    return;
}
 
void  _29(){
    return;
}
 
void  _2a(){
    return;
}
 
void  _2b(){
    return;
}
 
void  _2c(){
    return;
}
 
void  _2d(){
    return;
}
 
void  _2e(){
    return;
}
 
void  _2f(){
    return;
}


void  _30(){
    return;
}
 
void  _31(){
    return;
}
 
void  _32(){
    return;
}
 
void  _33(){
    return;
}
 
void  _34(){
    return;
}
 
void  _35(){
    return;
}
 
void  _36(){
    return;
}
 
void  _37(){
    return;
}


/*
    Opcode: $38
    Instruction: SEC
    Addr Mode: IMP
*/

void  _38(){
	set_flag('c',1);
	cycles = 2;
    return;
}
 
void  _39(){
    return;
}
 
void  _3a(){
    return;
}
 
void  _3b(){
    return;
}
 
void  _3c(){
    return;
}
 
void  _3d(){
    return;
}
 
void  _3e(){
    return;
}
 
void  _3f(){
    return;
}


void  _40(){
    return;
}
 
void  _41(){
    return;
}
 
void  _42(){
    return;
}
 
void  _43(){
    return;
}
 
void  _44(){
    return;
}
 
void  _45(){
    return;
}
 
void  _46(){
    return;
}
 
void  _47(){
    return;
}


void  _48(){
    return;
}
 
void  _49(){
    return;
}
 
void  _4a(){
    return;
}
 
void  _4b(){
    return;
}
 
void  _4c(){
    return;
}
 
void  _4d(){
    return;
}
 
void  _4e(){
    return;
}
 
void  _4f(){
    return;
}


void  _50(){
    return;
}
 
void  _51(){
    return;
}
 
void  _52(){
    return;
}
 
void  _53(){
    return;
}
 
void  _54(){
    return;
}
 
void  _55(){
    return;
}
 
void  _56(){
    return;
}
 
void  _57(){
    return;
}


/*
    Opcode: $58
    Instruction: CLI
    Addr Mode: IMP
*/

void  _58(){
	set_flag('i',0);
	cycles = 2;
    return;
}
 
void  _59(){
    return;
}
 
void  _5a(){
    return;
}
 
void  _5b(){
    return;
}
 
void  _5c(){
    return;
}
 
void  _5d(){
    return;
}
 
void  _5e(){
    return;
}
 
void  _5f(){
    return;
}


void  _60(){
    return;
}
 
void  _61(){
    return;
}
 
void  _62(){
    return;
}
 
void  _63(){
    return;
}
 
void  _64(){
    return;
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


 
void  _66(){
    return;
}
 
void  _67(){
    return;
}


void  _68(){
    return;
}
 
void  _69(){
    return;
}
 
void  _6a(){
    return;
}
 
void  _6b(){
    return;
}
 
void  _6c(){
    return;
}
 
void  _6d(){
    return;
}
 
void  _6e(){
    return;
}
 
void  _6f(){
    return;
}


void  _70(){
    return;
}
 
void  _71(){
    return;
}
 
void  _72(){
    return;
}
 
void  _73(){
    return;
}
 
void  _74(){
    return;
}
 
void  _75(){
    return;
}
 
void  _76(){
    return;
}
 
void  _77(){
    return;
}


/*
    Opcode: $78
    Instruction: SEI
    Addr Mode: IMP
*/

void  _78(){
	set_flag('i',1);
	cycles = 2;
    return;
}
 
void  _79(){
    return;
}
 
void  _7a(){
    return;
}
 
void  _7b(){
    return;
}
 
void  _7c(){
    return;
}
 
void  _7d(){
    return;
}
 
void  _7e(){
    return;
}
 
void  _7f(){
    return;
}


void  _80(){
    return;
}
 
void  _81(){
    return;
}
 
void  _82(){
    return;
}
 
void  _83(){
    return;
}
 
void  _84(){
    return;
}
 
void  _85(){
    return;
}
 
void  _86(){
    return;
}
 
void  _87(){
    return;
}


/*
    Opcode: $88
    Instruction: CLV
    Addr Mode: ZP
*/

void  _88(){
	set_flag('v',0);
	cycles = 2;
    return;
}
 
void  _89(){
    return;
}
 
void  _8a(){
    return;
}
 
void  _8b(){
    return;
}
 
void  _8c(){
    return;
}
 
void  _8d(){
    return;
}
 
void  _8e(){
    return;
}
 
void  _8f(){
    return;
}


void  _90(){
    return;
}
 
void  _91(){
    return;
}
 
void  _92(){
    return;
}
 
void  _93(){
    return;
}
 
void  _94(){
    return;
}
 
void  _95(){
    return;
}
 
void  _96(){
    return;
}
 
void  _97(){
    return;
}


void  _98(){
    return;
}
 
void  _99(){
    return;
}
 
void  _9a(){
    return;
}
 
void  _9b(){
    return;
}
 
void  _9c(){
    return;
}
 
void  _9d(){
    return;
}
 
void  _9e(){
    return;
}
 
void  _9f(){
    return;
}


void  _a0(){
    return;
}
 
void  _a1(){
    return;
}
 
void  _a2(){
    return;
}
 
void  _a3(){
    return;
}
 
void  _a4(){
    return;
}
 
void  _a5(){
    return;
}
 
void  _a6(){
    return;
}
 
void  _a7(){
    return;
}


void  _a8(){
    return;
}
 
void  _a9(){
    return;
}
 
/*
    Opcode: $AA
    Instruction: TAX
    Addr Mode: IMP
*/

void _aa(){
    x = a;
    cycles = 2;
}

 
void  _ab(){
    return;
}
 
void  _ac(){
    return;
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

 
void  _ae(){
    return;
}
 
void  _af(){
    return;
}


void  _b0(){
    return;
}
 
void  _b1(){
    return;
}
 
void  _b2(){
    return;
}
 
void  _b3(){
    return;
}
 
void  _b4(){
    return;
}
 
void  _b5(){
    return;
}
 
void  _b6(){
    return;
}
 
void  _b7(){
    return;
}


void  _b8(){
    return;
}
 
void  _b9(){
    return;
}
 
void  _ba(){
    return;
}
 
void  _bb(){
    return;
}
 
void  _bc(){
    return;
}
 
void  _bd(){
    return;
}
 
void  _be(){
    return;
}
 
void  _bf(){
    return;
}


void  _c0(){
    return;
}
 
void  _c1(){
    return;
}
 
void  _c2(){
    return;
}
 
void  _c3(){
    return;
}
 
void  _c4(){
    return;
}
 
void  _c5(){
    return;
}
 
void  _c6(){
    return;
}
 
void  _c7(){
    return;
}


void  _c8(){
    return;
}
 
void  _c9(){
    return;
}
 
void  _ca(){
    return;
}
 
void  _cb(){
    return;
}
 
void  _cc(){
    return;
}
 
void  _cd(){
    return;
}
 
void  _ce(){
    return;
}
 
void  _cf(){
    return;
}


void  _d0(){
    return;
}
 
void  _d1(){
    return;
}
 
void  _d2(){
    return;
}
 
void  _d3(){
    return;
}
 
void  _d4(){
    return;
}
 
void  _d5(){
    return;
}
 
void  _d6(){
    return;
}
 
void  _d7(){
    return;
}


/*
    Opcode: $d8
    Instruction: CLD
    Addr Mode: IMP
*/

void  _d8(){
	set_flag('d',0);
    return;
}
 
void  _d9(){
    return;
}
 
void  _da(){
    return;
}
 
void  _db(){
    return;
}
 
void  _dc(){
    return;
}
 
void  _dd(){
    return;
}
 
void  _de(){
    return;
}
 
void  _df(){
    return;
}


void  _e0(){
    return;
}
 
void  _e1(){
    return;
}
 
void  _e2(){
    return;
}
 
void  _e3(){
    return;
}
 
void  _e4(){
    return;
}
 
void  _e5(){
    return;
}
 
void  _e6(){
    return;
}
 
void  _e7(){
    return;
}


void  _e8(){
    return;
}
 
void  _e9(){
    return;
}
 
void  _ea(){
    return;
}
 
void  _eb(){
    return;
}
 
void  _ec(){
    return;
}
 
void  _ed(){
    return;
}
 
void  _ee(){
    return;
}
 
void  _ef(){
    return;
}


void  _f0(){
    return;
}
 
void  _f1(){
    return;
}
 
void  _f2(){
    return;
}
 
void  _f3(){
    return;
}
 
void  _f4(){
    return;
}
 
void  _f5(){
    return;
}
 
void  _f6(){
    return;
}
 
void  _f7(){
    return;
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

 
void  _f9(){
    return;
}
 
void  _fa(){
    return;
}
 
void  _fb(){
    return;
}
 
void  _fc(){
    return;
}
 
void  _fd(){
    return;
}
 
void  _fe(){
    return;
}
 
void  _ff(){
    return;
}

