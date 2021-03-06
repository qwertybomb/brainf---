#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*interprets brainf*** */
void bf(char *str) {
    //allocate 30000 cells
    unsigned char data [30000] = {0};
    unsigned char *ptr = data;
    size_t l;
    char s = 0;
    for(;*str;str++) {
        switch(*str) {
            case '>': ++ptr; break;
            case '<': --ptr; break;
            case '+': ++(*ptr); break;
            case '-': --(*ptr); break;
            case '.': putchar(*ptr); break;
            case ',': *ptr=getchar(); break;
            case '[': if(!*ptr) for(l=1;l>0;s=*(++str), l+=(s=='[')-(s==']')); break;
            case ']': if(*ptr) for(l=1;l>0;s=*(--str), l+=(s==']')-(s=='[')); break;
            default: break;
        }
    }
}

/*Converts brainf*** to c.
 *Remember to use free after
 *being done with this.*/
char* bf2c(char *str) {
    /*allocate a large amount of memory*/
    size_t default_memory_size = 100000;
    char *dest = (char*)calloc(default_memory_size,1);
    char code[] = "#include <stdio.h>\n"
                 "#include <stdlib.h>\n"
                 "int main() \n"
                 "{\n"
                 "    unsigned char tape[30000] = {0}, *ptr = tape;\n";
    strncat(dest,code,sizeof(code));
    size_t len = sizeof(code);
    for (;*str;str++) {
        /*check if more memory needs to be allocated*/
        if(len > default_memory_size) dest = (char*)realloc(dest,len+default_memory_size);
        switch (*str) {
            case '>':
                strncat(dest, "    ++ptr;\n", 11), len += 11;
                break;
            case '<':
                strncat(dest, "    --ptr;\n", 11), len += 11;
                break;
            case '+':
                strncat(dest, "    ++*ptr;\n", 12), len += 12;
                break;
            case '-':
                strncat(dest, "    --*ptr;\n", 12), len += 12;
                break;
            case '.':
                strncat(dest, "    putchar(*ptr);\n", 19), len += 19;
                break;
            case ',':
                strncat(dest, "    *ptr = getchar();\n", 21), len += 21;
                break;
            case '[':
                strncat(dest, "    while(*ptr) \n    {\n", 23), len += 23;
                break;
            case ']':
                strncat(dest, "    }\n", 7), len += 7;
                break;
            default:
                break;
        }
    }
    strncat(dest,"\n}",2), len+=2;
    /*resize to the actual length to save space*/
    dest = (char*)realloc(dest,len+1);
    return dest;
}


/*here is a simple test case*/
int main() {

    char *code = "  +>>>>>+>+++<<+[-[-<++++++++++++[>-------\n"
                 "  <-]>[<+<<+>>>-]<++<[>-<-]<[>+<-]+>>[-[-<\n"
                 "  <->>[->+<[[-]<<+>>>-<]]]]<<[-<[-]>>>>>>[\n"
                 "  >>]<<[[-]+<<]<<<]>>>[->[>>]<<[-<<]>>+<]>\n"
                 "  [>>]+>+<[<<]>],----------]>>[>>]<<[>+<[-\n"
                 "  [-[->-<]]]>[-<<<[<<]<<<<[-]>>>>>>[>>]>]<\n"
                 "  <[-]<]<<[-]<+<[+++++++++++++++[>++++++>+\n"
                 "  ++++++<<-]>.++..++.>.++++.<.-.[-]<]>[+++\n"
                 "  ++++++++++[>+++++++>++++++++<<-]>>++.<++\n"
                 "  +.+++++.-----.--.>++.<++.-.<]++++++++++.";
    char* c_code = bf2c(code);
    /*open file for writing*/
   FILE* file = fopen("bf.c","w");
   fwrite(c_code,1,strlen(c_code),file);
   fclose(file);
    /*free memory*/
   free(c_code);
}
