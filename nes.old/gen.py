
file = open('file.c','w');

format = \
"""
void  _%s(){
    return;
}
"""
def fprint(data,end,file):
    file.write(data + end);

for i in range(256):
    a = hex(i);
    a = a.split('x')[1]
    if(len(a) == 1):
        a = '0' + a;
    end = ' ';
    if(i % 8 == 7):
        end = '\n'
    fprint(format % a,end,file);

#     fprint(f"void _{a}();",end,file);

# for i in range(256):
#     a = hex(i);
#     a = a.split('x')[1]
#     if(len(a) == 1):
#         a = '0' + a;
#     end = '\n';
#     fprint(f"opcodes[{hex(i)}] = _{a};",end,file);

file.close()