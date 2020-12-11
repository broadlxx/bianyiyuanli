# include <stdio.h>
# include <ctype.h>
# include <string.h>
#include <math.h>
#define UNKNOWN -1
#define LB 0
#define RB 1
#define PL 2

#define DIGIT 1
#define POINT 2
#define OTHER 3
#define POWER 4
#define PLUS 5
#define MINUS 6
#define UCON 6
#define OVER 7
#define ClassOther 200
#define EndState -1
# define ID 20
# define INT 8
# define LT 8
# define LE 9
# define EQ 10
# define NE 11
# define GT 12
# define GE 13
# define IS 19
# define MI 3
# define MU 4
# define DI 5
//# define GE 6
# define REAL 20
#define Max 256
#define SMAX 256
#define PMAX 5
#define E 0
#define T 1
#define F 2

#define MAX_KEY_NUMBER 20
#define KEY_WORD_END "waiting for your expanding"
char* KeyWordTable[MAX_KEY_NUMBER] = { "begin","end","while", "if", "then", "else", KEY_WORD_END };
char* aa;
char TOKEN[20];
char ch;
int row = 1;
int index = 0;
int ac = 0;
int oc = 0;
int StateStack[Max];
int StackPoint;
int TopState;
int InputWordType;
int NXQ = 0;
int NXTemp = 1;
int SentenceCount = 1;
int line = 1;
int st = 0;
struct QUATERNION  /*四元式表的结构*/
{
    char op[PMAX]; /*操作符*/
    char arg1[PMAX];  /*第一个操作数*/
    char arg2[PMAX];  /*第二个操作数*/
    char result[PMAX];  /*运算结果*/
}pQuad[256];  /*存放四元式的数组*/
char EBracket_Place[PMAX];
char i_Place[PMAX];
char E_Place[PMAX];
char T_Place[PMAX];
char F_Place[PMAX];
char id_Place[10][10];
//1.E->E+T  2.E->E-T  3.E->T  4.T->T*F  5.T->T/F  6.T->F  7.F->(E) 8.F->i
char Action[16][8][4] = { {"s4"," "," "," "," "," ","s5"," "},
{" "," ","s6","s7", " "," "," ","A"},
{" ","r3","r3","r3","s8","s9"," ","r3"},
{" ","r6","r6","r6", "r6", "r6"," ","r6"},
{"s4"," "," "," "," "," ","s5"," "},
{" ", "r8", "r8","r8", "r8","r8"," ","r8"},
{"s4"," "," "," "," "," ","s5"," "},
{"s4"," "," "," "," "," ","s5"," "},
{"s4"," "," "," "," "," ","s5"," "},
{"s4"," "," "," "," "," ","s5"," "},
{" ","s15","s6","s7"," "," "," "," "},
{" ","r1","r1","r1","s8","s9"," ","r1"},
{" ","r2","r2","r2","s8","s9"," ","r2"},
{" ","r4","r4","r4","r4","r4"," ","r4"},
{" ","r5","r5","r5","r5","r5"," ","r5"},
{" ","r7","r7","r7","r7","r7"," ","r7"} };

char Action2[16][8][4] = { {"s4"," "," "," "," "," ","s5"," "},
{" "," ","s6","s7", " "," "," ","A"},
{" ","r3","r3","r3","s8","s9"," ","r3"},
{" ","r6","r6","r6", "r6", "r6"," ","r6"},
{"s4"," "," "," "," "," ","s5"," "},
{" ", "r8", "r8","r8", "r8","r8"," ","r8"},
{"s4"," "," "," "," "," ","s5"," "},
{"s4"," "," "," "," "," ","s5"," "},
{"s4"," "," "," "," "," ","s5"," "},
{"s4"," "," "," "," "," ","s5"," "},
{" ","s15","s6","s7"," "," "," "," "},
{" ","r1","r1","r1","s8","s9"," ","r1"},
{" ","r2","r2","r2","s8","s9"," ","r2"},
{" ","r4","r4","r4","r4","r4"," ","r4"},
{" ","r5","r5","r5","r5","r5"," ","r5"},
{" ","r7","r7","r7","r7","r7"," ","r7"} };

int Goto[16][3] = { { 1, 2, 3},
{-1, -1, -1},
{-1, -1, -1},
{-1, -1, -1},
{10, 2, 3},
{-1, -1, -1},
{-1, 11, 3},
{-1, 12, 3},
{-1, -1, 13},
{-1, -1, 14},
{-1, -1, -1},
{-1, -1, -1},
{-1, -1, -1},
{-1, -1, -1},
{-1, -1, -1},
{-1, -1, -1}, };
int lookup(char*);
void out(int, char*);
void report_error();
int w, n, p, e, d;
int Class;
int ICON;
FILE* fp1;
float FCON;
static int CurrentState;
int GetChar(char ch);
int EXCUTE(int state, int symbol, FILE* fp, char JudgeStr[], int row, int index);
int LEX(void);
int Push(int State);
int Pop(int count);
int SLRControl(FILE* fp);
void GEN(char* Op, char* Arg1, char* Arg2, char* Result);
char* NewTemp(void);
void NextSentence(FILE* fp);

void scanner(FILE* fp)
{
    int i, c;
    fseek(fp, -1, 1);
    ch = fgetc(fp);
    while (ch == ' ' || ch == '\n' || ch == '\t') {
        if (ch == '\n') {
            row++;
        }
        ch = fgetc(fp);
    }
    if (ch == EOF) {
        return;
    }
    if (isalpha(ch)) {
        TOKEN[0] = ch;


        ch = fgetc(fp);
        i = 1;
        while (isalnum(ch)) {
            TOKEN[i] = ch;
            i++;
            ch = fgetc(fp);
        }
        TOKEN[i] = '\0';
        fseek(fp, -1, 1);

        c = lookup(TOKEN);
        if (c != 6) {
            out(c + 13, TOKEN);
        }
        else {
            if (isalpha(TOKEN[0])) {
                InputWordType = 6;
            }
            out(c + 14, TOKEN);
        }
    }
    else if (isdigit(ch) || ch == '.') {
        fseek(fp, -1, 1);
        int Type;
        CurrentState = 0;
        i = 0;
        do {
            ch = fgetc(fp);
            TOKEN[i] = ch;
            i++;
            TOKEN[i] = '\0';
            Type = GetChar(ch);
            EXCUTE(CurrentState, Type, fp, TOKEN, row, i);
        } while (CurrentState != EndState);
    }
    else {
        switch (ch) {
        case'<':ch = fgetc(fp);
            if (ch == '=') out(LE, "<=");
            else if (ch == '>') out(NE, "<>");
            else {
                out(LT, "<");
            }
            break;
        case'=':ch = fgetc(fp);
            if (ch == '=') out(EQ, "==");
            else {
                out(IS, "=");
            }
            break;
        case'>':ch = fgetc(fp);
            if (ch == '=') out(GE, ">=");
            else {
                out(GT, ">");
            }
            break;
        case'+':InputWordType = PL;
            out(PL, "+");
            break;
        case'-':InputWordType = MI;
            out(MI, "-");
            break;
        case'*':InputWordType = MU;
            out(MU, "*");
            break;
        case'/':InputWordType = DI;
            out(DI, "/");
            break;
        case'(':InputWordType = LB;
            out(LB, "(");
            break;
        case')':InputWordType = RB;
            out(RB, ")");
            break;
        case'#':InputWordType = OVER;
            out(OVER, "#");
            break;
        default:InputWordType = UNKNOWN; report_error(row); break; printf("14");
        }
        return;
    }

}

int HandleOtherWord(void)
{
    return ClassOther;
}

int SLRControl(FILE* fp) {
    while (Action2[TopState][InputWordType][0] != 'A') {
        if (TopState == -1) {
            printf("分析语句  %i  时状态栈栈顶指针错误！分析结束\n", SentenceCount);
            return 0;
        }
        if (Action[TopState][InputWordType][0] == ' ') {
            printf("分析语句  %i  时语法分析出错！分析结束\n", SentenceCount);
            return 0;
        }
        else if (Action[TopState][InputWordType][0] == 's') {
            Push(atoi(&Action[TopState][InputWordType][1]));
            printf("push\n");
            if (fgetc(fp) != EOF) {
                scanner(fp);
            }
            else {
                printf("语句  %i  不完整！分析结束\n", SentenceCount);
                return 0;
            }
        }
        else if (Action[TopState][InputWordType][0] == 'r') {
            int ProductionNum = atoi(&Action[TopState][InputWordType][1]);
            int ProductionLeft = 0;
            if (ProductionNum == 1) {
                ProductionLeft = E;
                Pop(3);
                printf("1归约\n");
                if (st != 0) {
                    int len1 = strlen(E_Place);
                    int len2 = strlen(T_Place);
                    if (len1 == 0) {
                        strcpy(E_Place, id_Place[oc]);
                        oc++;
                    }
                    if (len2 == 0) {
                        strcpy(T_Place, id_Place[oc]);
                        oc++;
                    }

                }
                else {
                    strcpy(E_Place, id_Place[ac - 1]);

                    strcpy(T_Place, id_Place[ac - 2]);
                    st++;
                }
                char* Temp = NewTemp();
                GEN("+", E_Place, T_Place, Temp);
                printf("生成四元式：(+，%s，%s，%s)\n", E_Place, T_Place, Temp);
                strcpy(E_Place, Temp);
            }
            else if (ProductionNum == 2) {
                ProductionLeft = E;
                Pop(3);
                printf("2归约\n");
                if (st != 0) {

                    int len1 = strlen(E_Place);
                    int len2 = strlen(T_Place);
                    if (len1 == 0) {
                        strcpy(E_Place, id_Place[oc]);
                        oc++;
                    }
                    if (len2 == 0) {
                        strcpy(T_Place, id_Place[oc]);
                        oc++;
                    }
                }
                else {
                    strcpy(E_Place, id_Place[ac - 1]);
                    strcpy(T_Place, id_Place[ac - 2]);
                    st++;
                }
                char* Temp = NewTemp();
                GEN("-", E_Place, T_Place, Temp);
                printf("生成四元式：(-，%s，%s，%s)\n", E_Place, T_Place, Temp);
                strcpy(E_Place, Temp);
            }
            else if (ProductionNum == 3) {
                ProductionLeft = E;
                Pop(1);
                printf("3归约\n");
                //char* Temp=NewTemp();
                //GEN(" ",T_Place,"",Temp);
                //strcpy(E_Place,Temp);

            }
            else if (ProductionNum == 4) {
                ProductionLeft = T;
                Pop(3);
                printf("4归约\n");
                char* Temp = NewTemp();

                if (st != 0) {
                    int len1 = strlen(T_Place);
                    int len2 = strlen(F_Place);
                    if (len1 == 0) {
                        strcpy(T_Place, id_Place[oc]);
                        oc++;
                    }
                    if (len2 == 0) {
                        strcpy(F_Place, id_Place[oc]);
                        oc++;
                    }
                }
                else {
                    strcpy(T_Place, id_Place[ac - 1]);
                    strcpy(F_Place, id_Place[ac - 2]);
                    printf("id_Place[oc]=%s\n", id_Place[ac - 1]);
                    printf("id_Place[oc]=%s\n", id_Place[ac - 2]);
                    st++;
                }
                GEN("*", T_Place, F_Place, Temp);
                printf("生成四元式：(*，%s，%s，%s)\n", T_Place, F_Place, Temp);
                strcpy(T_Place, Temp);
            }
            else if (ProductionNum == 5) {
                ProductionLeft = T;
                Pop(3);
                printf("5归约\n");
                if (st != 0) {

                    int len1 = strlen(T_Place);
                    int len2 = strlen(F_Place);
                    if (len1 == 0) {
                        strcpy(T_Place, id_Place[oc]);
                        printf("id_Place[oc]=%s\n", id_Place[oc]);
                        oc++;
                    }
                    if (len2 == 0) {
                        strcpy(F_Place, id_Place[oc]);
                        printf("id_Place[oc]=%s\n", id_Place[oc]);
                        oc++;
                    }
                }
                else {
                    strcpy(T_Place, id_Place[ac - 1]);
                    strcpy(F_Place, id_Place[ac - 2]);
                    printf("id_Place[oc]=%s\n", id_Place[ac - 1]);
                    printf("id_Place[oc]=%s\n", id_Place[ac - 2]);
                    st++;
                }

                char* Temp = NewTemp();
                GEN("/", T_Place, F_Place, Temp);

                printf("生成四元式：(/，%s，%s，%s)\n", T_Place, F_Place, Temp);
                strcpy(T_Place, Temp);
            }
            else if (ProductionNum == 6) {
                ProductionLeft = T;
                Pop(1);
                printf("6归约\n");

            }
            else if (ProductionNum == 7) {
                ProductionLeft = F;
                Pop(3);
                printf("7归约\n");
            }
            else if (ProductionNum == 8) {
                ProductionLeft = F;
                Pop(1);
                printf("8归约\n");

            }
            else {
                printf("分析语句  %i  时产生式编号超出范围！分析结束\n", SentenceCount);
                return 0;
            }
            Push(Goto[TopState][ProductionLeft]);
        }
    }

    printf("correct！\n");
    return 1;

}

int Push(int State) {
    if (SMAX == StackPoint) {
        printf("状态栈已满！");
        return 0;
    }
    StateStack[StackPoint] = State;
    StackPoint++;
    TopState = State;
    return 1;
}

int Pop(int count) {
    StackPoint = StackPoint - count;
    if (StackPoint < 0) {
        printf("状态栈指针不能为负值！");
        return 0;
    }
    TopState = StateStack[StackPoint - 1];
    return 1;
}

void GEN(char* Op, char* Arg1, char* Arg2, char* Result)
{
    strcpy(pQuad[NXQ].op, Op);
    strcpy(pQuad[NXQ].arg1, Arg1);
    strcpy(pQuad[NXQ].arg2, Arg2);
    strcpy(pQuad[NXQ].result, Result);
    fp1 = fopen("D:\\2.txt", "a");
    fprintf(fp1, "(%s，%s，%s，%s)\n", Op, Arg1, Arg2, Result);
    fclose(fp1);
    NXQ++;  /*全局变量NXQ用于指示所要产生的下一个四元式的编号*/
}

char* NewTemp(void)  /*产生一个临时变量*/
{
    char* TempID = (char*)malloc(PMAX);
    sprintf(TempID, "T%d", NXTemp++);
    return TempID;
}

void NextSentence(FILE* fp)
{
    
    while ('#' != ch)
    {
        
        ch = fgetc(fp);
    }
    SentenceCount++;
    return;
}

int HandleError(char StrJudge[], int row)
{
    printf("Row:%d*****%s 不合法的无符号数!\n", row, StrJudge); return 0;
}

int GetChar(char ch) {
    if (isdigit(ch)) { d = ch - '0'; return DIGIT; }
    if (ch == '.') return POINT;
    if (ch == 'E' || ch == 'e') return POWER;
    if (ch == '+') return PLUS;
    if (ch == '-') return MINUS;

    return OTHER;
}

int EXCUTE(int state, int symbol, FILE* fp, char JudgeStr[], int row, int index)
{
    switch (state)
    {
    case 0:switch (symbol)
    {
    case DIGIT: n = 0; p = 0; e = 1; w = d; CurrentState = 1; Class = UCON; break;
    case POINT: w = 0; n = 0; p = 0; e = 1; CurrentState = 3; Class = UCON; break;
    default: Class = ClassOther;
        CurrentState = EndState;
        InputWordType = UNKNOWN;
        printf("无符号数的第一个字符是非法的！\n");
    }
          break;
    case 1:switch (symbol)
    {
    case DIGIT: w = w * 10 + d; break;
    case POINT: CurrentState = 2; break;
    case POWER: CurrentState = 4; break;
    default: {
        if (ch != EOF) {
            fseek(fp, -1, 1);
        }

        ICON = w; CurrentState = EndState;
        JudgeStr[index - 1] = '\0';
        InputWordType = UCON;
        printf("(INT,%i)\n", ICON);
        char str[100];
        itoa(ICON, str, 10);
        strcpy(id_Place[ac], str);
        ac++;
    }break;

    }
          break;
    case 2:switch (symbol)
    {
    case DIGIT: n++; w = w * 10 + d; break;
    case POWER: CurrentState = 4; break;
    default: {
        if (ch != EOF) {
            fseek(fp, -1, 1);
        }

        FCON = w * pow(10, e * p - n); CurrentState = EndState;
        JudgeStr[index - 1] = '\0';
        InputWordType = UCON;
        printf("(REAL,%f)\n", FCON);
        char str[100];
        sprintf(str, "%f", FCON);
        strcpy(id_Place[ac], str);
        ac++;
    }break;
    }
          break;
    case 3:switch (symbol)
    {
    case DIGIT: n++; w = w * 10 + d; CurrentState = 2; break;
    default:InputWordType = UNKNOWN; HandleError(JudgeStr, row); CurrentState = EndState; printf("11");
    }
          break;
    case 4:switch (symbol)
    {
    case DIGIT: p = p * 10 + d; CurrentState = 6; break;
    case MINUS: e = -1; CurrentState = 5; break;
    case PLUS: CurrentState = 5; break;
    default: InputWordType = UNKNOWN; HandleError(JudgeStr, row); CurrentState = EndState; printf("12");
    }
          break;
    case 5:switch (symbol)
    {
    case DIGIT: p = p * 10 + d; CurrentState = 6; break;
    default: InputWordType = UNKNOWN; HandleError(JudgeStr, row); CurrentState = EndState; printf("13");
    }
          break;
    case 6:switch (symbol)
    {
    case DIGIT:p = p * 10 + d; break;
    default: {
        if (ch != EOF) {
            fseek(fp, -1, 1);
        }

        FCON = w * pow(10, e * p - n); CurrentState = EndState;
        JudgeStr[index - 1] = '\0';
        InputWordType = UCON;
        printf("(REAL,%f)\n", FCON);
        char str[100];
        sprintf(str, "%f", FCON);
        strcpy(id_Place[ac], str);
        ac++;
    }break;
    }
          break;
    }
    return CurrentState;
}

int lookup(char* token)
{
    int n = 0;
    while (strcmp(KeyWordTable[n], KEY_WORD_END))
    {
        if (!strcmp(KeyWordTable[n], token))
        {
            return n + 1;
            break;
        }
        n++;
    }
    return 6;
}



void report_error() {
    printf("%s Error! In the %d row\n", TOKEN, row);
}

void out(int i, char* pStr) {
    char Mnemonic[20];
    if (i == 0) {
        strcpy(Mnemonic, "LB");
    }
    else if (i == 1) {
        strcpy(Mnemonic, "RB");
    }
    else if (i == 2) {
        strcpy(Mnemonic, "PL");
    }
    else if (i == 3) {
        strcpy(Mnemonic, "MI");
    }
    else if (i == 4) {
        strcpy(Mnemonic, "MU");
    }
    else if (i == 5) {
        strcpy(Mnemonic, "DI");
    }
    else if (i == 6) {
        strcpy(Mnemonic, "UCON");
    }
    else if (i == 7) {
        strcpy(Mnemonic, "OVER");
    }
    else if (i == 8) {
        strcpy(Mnemonic, "LT");
    }
    else if (i == 9) {
        strcpy(Mnemonic, "LE");
    }
    else if (i == 10) {
        strcpy(Mnemonic, "EQ");
    }
    else if (i == 11) {
        strcpy(Mnemonic, "NE");
    }
    else if (i == 12) {
        strcpy(Mnemonic, "GT");
    }
    else if (i == 13) {
        strcpy(Mnemonic, "GE");
    }
    else if (i == 14) {
        strcpy(Mnemonic, "BEGIN");
    }
    else if (i == 15) {
        strcpy(Mnemonic, "END");
    }
    else if (i == 16) {
        strcpy(Mnemonic, "IF");
    }
    else if (i == 17) {
        strcpy(Mnemonic, "THEN");
    }
    else if (i == 18) {
        strcpy(Mnemonic, "ELSE");
    }
    else if (i == 19) {
        strcpy(Mnemonic, "IS");
    }
    else if (i == 20) {
        strcpy(Mnemonic, "ID");
        strcpy(id_Place[ac], pStr);
        ac++;
    }
    else {
        strcpy(Mnemonic, "Unknown Type");
    }
    printf("(%s,%s)\n", Mnemonic, pStr);

}

void main() {
    FILE* p = fopen("D:\\1.txt", "r");
    if (ch = fgetc(p) == EOF) {
        printf("The file is null.\n");
        return 0;
    }
    do
    {
        TopState = 0;
        StackPoint = 0;
        memset(StateStack, -1, sizeof(StateStack));
        printf("语句分析开始\n");
        scanner(p);
        SLRControl(p);
        NextSentence(p);
    } while (EOF != fgetc(p));

    return 0;
}
