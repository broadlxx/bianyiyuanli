#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#define DIGIT 1
#define POINT 2
#define OTHER 3
#define POWER 4
#define PLUS 5
#define MINUS 6
#define UCON 7
#define LT 8
#define LE 9
#define EQ 10
#define NE 11
#define GT 12
#define GE 13
#define IS 14
#define MI 15
#define MU 16
#define DI 17
#define ID 18
#define BEGIN 19
#define END 20
#define IF 21
#define THEN 22
#define ELSE 23
#define WHILE 24
#define L 25
#define R 26
#define MAX_KEY_NUMBER 20
#define KEY_WORD_END "waiting for your expanding"
#define ClassOther 200
#define EndState -1
#define RIGHT 1
#define ERROR 0
#define MAXINPUT 100
#define MAXSTACK 100
#define STARTSYMBOL 'E'
char TOKEN[20];
int w, n, p, e, d;
int Class;  //Used to indicate class of the word
int ICON;
double FCON;
static int CurrentState;
char* KeyWordTable[MAX_KEY_NUMBER] = { "begin", "end", "if", "then", "else", "while", KEY_WORD_END };
//�ļ��л�ȡ�ַ�������֮�󷵻�����
int GetChar(FILE* fp)//�ļ��л�ȡ�ַ�������֮�󷵻�����
{
	int c;
	c = fgetc(fp);
	if (isdigit(c))
	{
		d = c - '0';
		return DIGIT;
	}
	if (c == '.')
		return POINT;
	if (c == 'E' || c == 'e')
		return POWER;
	if (c == '+')
		return PLUS;
	if (c == '-')
		return MINUS;
	return OTHER;
}
//ʶ���޷�����
int EXCUTE(int state, int symbol)
{
	switch (state)
	{
	case 0:switch (symbol)
	{
	case DIGIT:n = 0; p = 0; e = 1; w = d; CurrentState = 1; Class = UCON; break;
	case POINT:w = 0; n = 0; p = 0; e = 1; CurrentState = 3; Class = UCON; break;
	default:Class = ClassOther;
		CurrentState = EndState;
	}
		  break;
	case 1:switch (symbol)
	{
	case DIGIT:w = w * 10 + d; break;  //CurrentState=1
	case POINT:CurrentState = 2; break;
	case POWER:CurrentState = 4; break;
	default: ICON = w; CurrentState = EndState;
	}
		  break;
	case 2:switch (symbol)
	{
	case DIGIT:n++; w = w * 10 + d; break;
	case POWER:CurrentState = 4; break;
	default:FCON = w * pow(10, e * p - n); CurrentState = EndState;
	}
		  break;
	case 3:switch (symbol)
	{
	case DIGIT:n++; w = w * 10 + d; CurrentState = 2; break;
	default:printf("������Ŵ�����\n"); CurrentState = EndState;
	}
		  break;
	case 4:switch (symbol)
	{
	case DIGIT:p = p * 10 + d; CurrentState = 6; break;
	case MINUS:e = -1; CurrentState = 5; break;
	case PLUS:CurrentState = 5; break;
	default:printf("������Ŵ�����\n"); CurrentState = EndState;
	}
		  break;
	case 5:switch (symbol)
	{
	case DIGIT:p = p * 10 + d; CurrentState = 6; break;
	default:printf("������Ŵ�����\n"); CurrentState = EndState;
	}
		  break;
	case 6:switch (symbol)
	{
	case DIGIT:p = p * 10 + d; break;
	default:FCON = w * pow(10, e * p - n); CurrentState = EndState;
	}
		  break;
	}
	return CurrentState;
}
//�޷�����ʶ�����
void LEX(FILE* fp)
{
	int ch;
	CurrentState = 0;
	while (CurrentState != EndState)
	{
		ch = GetChar(fp);
		EXCUTE(CurrentState, ch);
	}
}
//ʶ��ؼ���
int lookup(char* token)
{
	int n = 0;
	while (strcmp(KeyWordTable[n], KEY_WORD_END)) /*strcmp�Ƚ������Ƿ���ͬ������ͬ����0*/
	{
		if (!strcmp(KeyWordTable[n], token)) /*�Ƚ�token��ָ��Ĺؼ��ֺͱ����ֱ����ĸ��ؼ������*/
		{
			return n + 19; /*���ݵ��ʷ������I��������ȷ�Ĺؼ�������룬�����ش�������ֵ*/
			break;
		}
		n++;
	}
	return 0;
}
//�ʷ��������Ϊ��ʶ�����ؼ���������ļ��� 
void out(int m, char* a, FILE* fp2, FILE* fp3)
{
	fprintf(fp2, "(", fp2);
	switch (m)
	{
	case BEGIN:fputs("BEGIN", fp2);
		fputs("begin", fp3); break;
	case END:fputs("END", fp2);
		fputs("end", fp3); break;
	case IF:fputs("IF", fp2);
		fputs("if", fp3); break;
	case THEN:fputs("THEN", fp2);
		fputs("then", fp3); break;
	case ELSE:fputs("ELSE", fp2);
		fputs("else", fp3); break;
	case WHILE:fputs("WHILE", fp2);
		fputs("while", fp3); break;
	case ID:fputs("ID", fp2);
		fputs("i", fp3); break;
	case LT:fputs("LT", fp2);
		fputs("<", fp3); break;
	case LE:fputs("LE", fp2);
		fputs("<=", fp3); break;
	case EQ:fputs("EQ", fp2);
		fputs("=", fp3); break;
	case NE:fputs("NE", fp2);
		fputs("<>", fp3); break;
	case GT:fputs("GT", fp2);
		fputs(">", fp3); break;
	case GE:fputs("GE", fp2);
		fputs(">=", fp3); break;
	case IS:fputs("IS", fp2);
		fputs(":=", fp3); break;
	case PLUS:fputs("PL", fp2);
		fputs("+", fp3); break;
	case MI:fputs("MI", fp2);
		fputs("-", fp3); break;
	case MU:fputs("MU", fp2);
		fputs("*", fp3); break;
	case DI:fputs("DI", fp2);
		fputs("/", fp3); break;
	case L:fputs("L", fp2);
		fputs("(", fp3); break;
	case R:fputs("R", fp2);
		fputs(")", fp3); break;
	}
	if (a != " ")
		fprintf(fp2, ",'%s')\n", a);
	else
		fprintf(fp2, ",%s)\n", a);
}

void scanner_example(FILE* fp1, FILE* fp2, FILE* fp3)
{
	char ch;
	int i, c;
	ch = fgetc(fp1);
	if (isalpha(ch))//��һ���ַ�Ϊ��ĸ�����ǹؼ��ֻ��߱�ʶ��
	{
		TOKEN[0] = ch;
		ch = fgetc(fp1);
		i = 1;
		while (isalnum(ch))//Ϊ��ĸ������ʱ����������
		{
			TOKEN[i] = ch;
			i++;
			ch = fgetc(fp1);
		}
		TOKEN[i] = '\0';
		if (!feof(fp1))
		{
			fseek(fp1, -1, 1);  //retract
		}
		c = lookup(TOKEN);//c=0�Ǳ�ʶ��
		if (c == 0)
			out(ID, TOKEN, fp2, fp3);
		else
			out(c, " ", fp2, fp3);
	}
	else if (isdigit(ch) || ch == '.')//��һ���ַ�Ϊ���֣������޷�����
	{
		fseek(fp1, -1, 1);
		LEX(fp1);//�����޷�����ʶ�����
		if (ICON != 0) {
			fprintf(fp2, "(INT,%d)\n", ICON);
			fputs("i", fp3);
		}
		if (FCON != 0.0) {
			fprintf(fp2, "(REAL,%g)\n", FCON);
			fputs("i", fp3);
		}
		if (!feof(fp1))
		{
			fseek(fp1, -1, 1);
		}
		ICON = 0; FCON = 0.0;
	}

	else
	{
		switch (ch)
		{
		case '<':ch = fgetc(fp1);//<=��<>��<�ķ���
			if (ch == '=')
				out(LE, " ", fp2, fp3);
			else if (ch == '>')
				out(NE, " ", fp2, fp3);
			else
			{
				fseek(fp1, -1, 1);
				out(LT, " ", fp2, fp3);
			}
			break;
		case '=': out(EQ, " ", fp2, fp3); break;
		case '>': ch = fgetc(fp1);//>=��>�ķ���
			if (ch == '=')out(GE, " ", fp2, fp3);
			else
			{
				fseek(fp1, -1, 1);
				out(GT, " ", fp2, fp3);
			}
			break;
		case '+':out(PLUS, " ", fp2, fp3); break;//�������
		case '-':out(MI, " ", fp2, fp3); break;
		case '*':out(MU, " ", fp2, fp3); break;
		case '/':out(DI, " ", fp2, fp3); break;
		case '(':out(L, " ", fp2, fp3); break;
		case ')':out(R, " ", fp2, fp3); break;
		case ':':ch = fgetc(fp1);//:=����
			if (ch == '=')
				out(IS, " ", fp2, fp3);
			else
			{
				fseek(fp1, -1, -1);
			}

		case '�': break;
		case '\0':case ' ':case '\t':case '\n':scanner_example(fp1, fp2, fp3); break;//���ո��Ʊ�����س�����������ɨ��
		default:

			fprintf(fp2, "����ķ��Ŵ� %c ����!\n", ch);
			//		fputs("ERROR!\n",fp2);
			printf("������Ŵ�����\n"); break;
		}
	}
}

int count = 0;
char VT[8] = { '(','i','*','/','+','-',')','#' };
char stack[MAXSTACK],
a[MAXINPUT];
//���������ȽϷ������ȼ�
char matrix[8][8] =
{ '<','<','<','<','<','<','=','e',
'e','e','>','>','>','>','>','>',
'<','<','>','>','>','>','>','>',
'<','<','>','>','>','>','>','>',
'<','<','<','<','>','>','>','>',
'<','<','<','<','>','>','>','>',
'e','e','>','>','>','>','>','>',
'<','<','<','<','<','<','e','=' };
int CharToInt(char c)
{
	switch (c)
	{
	case '(':return 0;
	case 'i':return 1;
	case '*':return 2;
	case '/':return 3;
	case '+':return 4;
	case '-':return 5;
	case ')':return 6;
	case '#':return 7;
	default:return -1;
	}
}
int IsHigherThan(char a, char b)
{
	int i = CharToInt(a), j = CharToInt(b);
	if (matrix[i][j] == '>')
		return 1;
	else
		return 0;
}
int IsLowerThan(char a, char b)
{
	int i = CharToInt(a), j = CharToInt(b);
	if (matrix[i][j] == '<')
		return 1;
	else
		return 0;
}
int IsEqualTo(char a, char b)
{
	int i = CharToInt(a), j = CharToInt(b);
	if (matrix[i][j] == '=')
		return 1;
	else
		return 0;
}
int IsVt(char c)
{
	for (int i = 0; i < 8; i++)
	{
		if (c == VT[i])
			return 1;
	}
	return 0;
}
char Reduce(int begin, int end, int len)
{
	if (len == 1)
	{
		switch (stack[begin])
		{
		case 'i':return 'F';
		default:return '!';
		}
	}
	else if (len == 3)
	{
		switch (stack[begin + 1])
		{
		case '*':case'/':return 'T';
		case '+':case'-':return 'E';
		case 'E':case 'T':case 'F':return 'F';
		default:return '!';
		}
	}
	else
	{
		printf("ERROR!\n");
		exit(0);
	}
}

int parser(FILE* fp)
{
	fscanf(fp, "%s", a);
	int i, k;
	char r, NewVn;
	i = 0; k = 0;
	stack[0] = '#';
	do
	{
		int j;
		r = a[i++];
		if (IsVt(stack[k]))
			j = k;
		else
			j = k - 1;
		while (IsHigherThan(stack[j], r))
		{
			char q;
			do
			{
				q = stack[j];
				if (IsVt(stack[j - 1]))
					j--;
				else
					j -= 2;
			} while (!IsLowerThan(stack[j], q));
			NewVn = Reduce(j + 1, k, k - j); count++;
			printf("%d.", count);
			for (int m = j + 1; m <= k; m++)
				printf("%c", stack[m]);
			printf("��ԼΪ��%c\n", NewVn);
			k = j + 1;

			stack[k] = NewVn;
		}
		if ((IsLowerThan(stack[j], r)) || (IsEqualTo(stack[j], r)))
		{
			stack[++k] = r; count++;
			printf("%d.�ƽ�:%c\n", count, r);
		}
		else
			return ERROR;
	} while (r != '#');
	return RIGHT;
}

int main()
{
	FILE* fp1, * fp2, * fp3;
	if ((fp1 = fopen("expr.txt", "r")) == NULL)
	{
		printf("file open error!\n");
		return 0;
	}
	if ((fp2 = fopen("file1_2.txt", "w")) == NULL)
	{
		printf("file open error!\n");
		return 0;
	}
	if ((fp3 = fopen("output.txt", "w+")) == NULL)
	{
		printf("file open error!\n");
		return 0;
	}
	while (!feof(fp1))
	{
		scanner_example(fp1, fp2, fp3);
	}
	fputs("#", fp3);
	fseek(fp3, 0, SEEK_SET);
	/*fclose(fp3);
	if ((fp3 = fopen("file2.txt", "r+")) == NULL)
	{
		printf("file open error!\n");
	}
	*/
	switch (parser(fp3))
	{
	case ERROR:printf("ERROR!\n"); break;
	case RIGHT:printf("RIGHT!\n"); break;
	default:break;
	}
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	return 0;
}

