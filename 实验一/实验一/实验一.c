#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

//===================���ʷ��ż��������==========================

#define IF 1
#define THEN 2
#define ELSE 3
#define FOR 4
#define BEGIN 5
#define END 6
#define ID 7    // ��ʶ��
#define INT 8   // ����
#define REAL 9  // ʵ�� 
#define LT 10   // <
#define LE 11   // <=
#define EQ 12   // =
#define NE 13   // <>
#define GT 14   // >
#define GE 15   // >=
#define IS 16   // :=
#define PL 17   // +
#define MI 18   // -
#define MU 19   // *
#define DI 20   // /

//==================��������������==============================
char TOKEN[20];
char ch;
extern int lookup(char*);
extern void out(int, char*);
extern void report_error(void);

#define DIGIT 1
#define POINT 2
#define OTHER 3
#define POWER 4
#define PLUS 5
#define MINUS 6
#define UCON 7 // Suppose the class number of unsigned constant is 7
#define ClassOther 200
#define EndState -1
int w, n, p, e, d;
int Class;  //Used to indicate class of the word
int ICON;
float FCON;
static int CurrentState;  //Used to present current state, the initial value:0
int GetChar(void);
int EXCUTE(int, int);
int LEX(void);

//==================ɨ����==============================

void scanner(FILE* fp) {
	int i, c;
	ch = fgetc(fp);
	if (isalpha(ch)) {   /*it must be an identifer!*/
		TOKEN[0] = ch;
		ch = fgetc(fp);
		i = 1;
		while (isalnum(ch)) {
			TOKEN[i] = ch;
			++i;
			ch = fgetc(fp);
		}
		TOKEN[i] = '\0';
		fseek(fp, -1, 1);  /*retract*/
		c = lookup(TOKEN);
		if (c == 0)
			out(ID, TOKEN);  // out�������� ע�⣺out���յĵ�һ������Ϊ
		else                 // �����룬��Ҫת�������Ƿ�������ļ�
			out(c, " ");
	}
	else if (isdigit(ch)) {
		TOKEN[0] = ch;
		ch = fgetc(fp);
		i = 1;
		while (isdigit(ch) || ch == '.' || ch == 'E' || ch == '-') { // �˴�����isalunm()������ʶ���ѧ������
			TOKEN[i] = ch;   // ����ʶ�𸡵���
			++i;
			ch = fgetc(fp);
		}
		TOKEN[i] = '\0';
		fseek(fp, -1, 1);
		if (isdigit(ch))
			out(INT, TOKEN);
		//else
			//out(REAL, TOKEN);
		LEX();

	}
	else {
		if (ch == ' ' || ch == '\n');
		else switch (ch) {
		case '<': ch = fgetc(fp);
			if (ch == '=')
				out(LE, " ");
			else if (ch == '>')
				out(NE, " ");
			else {
				fseek(fp, -1, 1);
				out(LT, " ");
			}
			break;
		case '=': out(EQ, " ");
			break;
		case '>': ch = fgetc(fp);
			if (ch == '=')
				out(GE, " ");
			else {
				fseek(fp, -1, 1);
				out(GT, " ");
			}
			break;
		case ':': ch = fgetc(fp);
			if (ch == '=')
				out(IS, " ");
			break;
		case '+': out(PL, " ");
			break;
		case '-': out(MI, " ");
			break;
		case '*': out(MU, " ");
			break;
		case '/': out(DI, " ");
			break;

		default: report_error(); // ����report_error����
			break;
		}
	}
	return;
}


//===================���������ֱ�===============================

#define MAX_KEY_NUMBER 20   /*�ؼ��ֵ�����*/
#define KEY_WORD_END "Waiting for new"   /*�ؼ��ֽ������*/
char* KeyWordTable[MAX_KEY_NUMBER] = { "if", "then", "else", "for", "begin",
									  "end", KEY_WORD_END };


//===================�鱣���ֱ��ж��Ƿ�Ϊ�ؼ���================

int lookup(char* token) {
	int n = 0;
	while (strcmp(KeyWordTable[n], KEY_WORD_END)) {
		/*strcmp�Ƚ������Ƿ���ͬ������ͬ����0*/
		if (!strcmp(KeyWordTable[n], token)) {
			/*�Ƚ�token��ָ��Ĺؼ��ֺͱ����ֱ����ĸ��ؼ������*/
			return n + 1;
			break;
		}
		++n;
	}
	return 0;
}


//==================�����output.txt�ļ�=========================

void out(int a, char* p) {
	FILE* fp;
	fp = fopen("output.txt", "a");
	switch (a) {
	case IF: fprintf(fp, "(IF, %s)\n", p); break;
	case THEN: fprintf(fp, "(THEN, %s)\n", p); break;
	case ELSE: fprintf(fp, "(ELSE, %s)\n", p); break;
	case FOR: fprintf(fp, "(FOR, %s)\n", p); break;
	case BEGIN: fprintf(fp, "(BEGIN, %s)\n", p); break;
	case END: fprintf(fp, "(END, %s)\n", p); break;
	case ID: fprintf(fp, "(ID, %s)\n", p); break;
	case INT: fprintf(fp, "(INT, %s)\n", p); break;
	case REAL: fprintf(fp, "(REAL, %s)\n", p); break;
	case LT: fprintf(fp, "(LT, %s)\n", p); break;
	case LE: fprintf(fp, "(LE, %s)\n", p); break;
	case EQ: fprintf(fp, "(EQ, %s)\n", p); break;
	case NE: fprintf(fp, "(NE, %s)\n", p); break;
	case GT: fprintf(fp, "(GT, %s)\n", p); break;
	case GE: fprintf(fp, "(GE, %s)\n", p); break;
	case IS: fprintf(fp, "(IS, %s)\n", p); break;
	case PL: fprintf(fp, "(PL, %s)\n", p); break;
	case MI: fprintf(fp, "(MI, %s)\n", p); break;
	case MU: fprintf(fp, "(MU, %s)\n", p); break;
	case DI: fprintf(fp, "(DI, %s)\n", p); break;
	default: break;
	}
}


//======================������================================


int HandleOtherWord(void) {
	return ClassOther;
}

int HandleError(void) {
	printf("Error!\n");
	return 0;
}

int GetChar(int i) {
	int c;
	c = (int)TOKEN[i];
	if (isdigit(c)) {
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

int EXCUTE(int state, int symbol) {
	switch (state) {
	case 0: switch (symbol) {
	case DIGIT: n = 0; p = 0; e = 1; w = d;
		CurrentState = 1; Class = UCON;
		break;
	case POINT: w = 0; n = 0; p = 0; e = 1;
		CurrentState = 3; Class = UCON;
		break;
	default: HandleOtherWord();
		Class = ClassOther; CurrentState = EndState;
	}
		  break;
	case 1: switch (symbol) {
	case DIGIT: w = w * 10 + d; break; // CurrentState = 1
	case POINT: CurrentState = 2; break;
	case POWER: CurrentState = 4; break;
	default: ICON = w; CurrentState = EndState;
	}
		  break;
	case 2: switch (symbol) {
	case DIGIT: ++n; w = w * 10 + d; break;
	case POWER: CurrentState = 4; break;
	default: FCON = w * pow(10, e * p - n);
		CurrentState = EndState;
	}
		  break;
	case 3: switch (symbol) {
	case DIGIT: ++n; w = w * 10 + d;
		CurrentState = 2; break;
	default: HandleError(); CurrentState = EndState;
	}
		  break;
	case 4: switch (symbol) {
	case DIGIT: p = p * 10 + d;
		CurrentState = 6; break;
	case MINUS: e = -1; CurrentState = 5; break;
	case PLUS: CurrentState = 5; break;
	default: HandleError(); CurrentState = EndState;
	}
		  break;
	case 5: switch (symbol) {
	case DIGIT: p = p * 10 + d;
		CurrentState = 6; break;
	default: HandleError(); CurrentState = EndState;
	}
		  break;
	case 6: switch (symbol) {
	case DIGIT: p = p * 10 + d; break;
	default: FCON = w * pow(10, e * p - n);
		CurrentState = EndState;
		FILE* fp;
		fp = fopen("output.txt", "a");
		fprintf(fp, "(REAL, %g)\n", FCON);

	}
		  break;
	}
	return CurrentState;
}

int LEX(void) {
	int ch, a = 0;
	CurrentState = 0;
	while (CurrentState != EndState) {
		ch = GetChar(a);
		EXCUTE(CurrentState, ch);
		++a;
	}
	return Class;
}


//======================�����������==========================

void report_error() {
	FILE* fp;
	fp = fopen("output.txt", "a");
	fprintf(fp, "\n There is an error. \n");
	//exit(0);
}

void main() {
	extern char ch;
	FILE* fp;
	if ((fp = fopen("file.txt", "r")) == NULL) {
		printf("File open error.");
		exit(0);
	}

	do {
		scanner(fp);
	} while (ch != EOF);
}