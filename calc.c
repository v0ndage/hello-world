/* COP 3502C Midterm Assignment One This program is written by: Johnathan von der Heyde */ 

//This program converts string infix to string postfix,
//and then computes the postfix, all according to PEMDAS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define max 50
char steps;										//turn on steps for debugging


//STACKING

int mathstack[max];								//used to calculate from charstacks
char rightstack[max];							//R stack organizes infix
char leftstack[max];							//L stack organizes postfix
int rtop = -1, ltop = -1, mtop = -1;

//Int Stacking

void Mpush(int i, int* stack){
	if(mtop==max-1){if(steps!='\n'){printf("[M]stack is FULL\n");}}
	else{mtop=mtop+1; stack[mtop]=i; if(steps!='\n'){printf("[M]pushed: %d\n", stack[mtop]);}}
}

int Mpop(int* stack){							//returning max int represents empty mathstack
	int t = stack[mtop];
	if(mtop==-1){if(steps!='\n'){printf("[M]stack is EMPTY\n");} return 2147483647;}
	else{mtop=mtop-1; if(steps!='\n'){printf("[M]popped: %d\n", t);} return t;}
}

//Char Stacking

void push(char c, char d, char* stack){			//c is character to push
												//d is direction indicator (right or left stack)
	if(d=='R'){
		if(rtop==max-1){if(steps!='\n'){printf("[%c]stack is FULL\n", d);}}
		else{rtop=rtop+1; stack[rtop]=c; if(steps!='\n'){printf("[%c]pushed: %c\n", d, stack[rtop]);}}
	}
	else if(d=='L'){
		if(ltop==max-1){if(steps!='\n'){printf("[%c]stack is FULL\n", d);}}
		else{ltop=ltop+1; stack[ltop]=c; if(steps!='\n'){printf("[%c]pushed: %c\n", d, stack[ltop]);}}
	}
}
 
char pop(char d, char* stack){					//redundancy is for global r & l stacks

	if(d=='R'){
		char t = stack[rtop];
		if(rtop==-1){if(steps!='\n'){printf("[%c]stack is EMPTY\n", d);} return '!';}
		else{rtop=rtop-1; if(steps!='\n'){printf("[%c]popped: %c\n", d, t);} return t;}
	}
	else{										//! also signals open parentheses below
		char t = stack[ltop];
		if(ltop==-1){if(steps!='\n'){printf("[%c]stack is EMPTY\n", d);} return '!';}
		else{ltop=ltop-1; if(steps!='\n'){printf("[%c]popped: %c\n", d, t);} return t;}
	}
}

//CHECK BALANCED INPUT

int isparentheses(char a){

	if((a=='{')||(a=='}')||(a=='[')||(a==']')||(a=='(')||(a==')')){return 1;}
	else{return 0;}
}

int compare(char a, char b){

	if(((b=='{')&&(a=='}'))||((b=='[')&&(a==']'))||((b=='(')&&(a==')'))){return 1;}
	else{return 0;}
}

int checkbalance(char* in){							//checks for correct parentheses

	char* string = malloc((char)sizeof(in+1));

	strcpy(string, in);

	char c;

	for (int i=0; i<max; i++){

		if((string[i]=='{')||(string[i]=='[')||(string[i]=='(')){

			push(string[i], 'R', rightstack);
		}

		if((string[i]=='}')||(string[i]==']')||(string[i]==')')){

			c = pop('R', rightstack);

			if(c == '!'){return 0;}
			
			if(compare(string[i], c)==1){continue;}	//compare current with last parentheses
			else{return 0;}							//continue if match, return 0 otherwise
		}
	}

	free(string);

	if(rtop==-1){return 1;}							//if stack is empty, parentheses are correct
	else{return 0;}
}

//P E M D A S 										//parentheses are prioritized implicitly below

int priority(char o){								//calculate operator's priority

	if(o=='^'){return 3;}
	else if((o=='*')||(o=='/')||(o=='%')){return 2;}
	else if((o=='+')||(o=='-')){return 1;}
	else{return 0;}
}

int math(int a, int b, char o){						//operates with characters

	int j=1;

	if(o=='^'){for(int i=0; i<b; i++){j=j*a;} return (j);}
	else if(o=='*'){return (a*b);}
	else if(o=='/'){
		if(b==0){printf("Divide by Zero Error\n"); exit(1);} 
		else{return (a/b);}
	}
	else if(o=='%'){
		if(b==0){printf("Mod by Zero Error\n"); exit(1);} 
		else{return (a%b);}
	}
	else if(o=='+'){return (a+b);}
	else if(o=='-'){return (a-b);}
	else{return 0;}
}

//INFIX TO POSTFIX

char* in2postfix(char** input){

	int j=0, k=0, l=0, size=strlen(*input);

	char c, p;

 //Main Loop

	for (int i=0; i < size; i++){

		fflush(stdout);

		if(steps!='\n'){printf("*viewing* %c\n", input[0][i]);}

	 //Spaces

		if(input[0][i]==' '){}							//spaces are skipped and dealt with in result

	 //Operands

		else if(isdigit(input[0][i])){					//operands get pushed to Lstack

			push(input[0][i], 'L', leftstack);

			l=i+1;										//l substitutes i

			while(isdigit(input[0][l])){				//this loop is for consecutive digits (e.g. 123)

				push(input[0][l], 'L', leftstack);

				l++;
			}

			push(' ', 'L', leftstack); i=l-1;
		}

	 //Operators

		else if(priority(input[0][i])!=0){				//operaters are compared by priority

			c=pop('R', rightstack);

			if(c=='!'){push(input[0][i], 'R', rightstack);}

			else if(c==' '){push(c, 'L', leftstack);}

			else if(priority(input[0][i])>priority(c)){

				push(c, 'R', rightstack); push(input[0][i], 'R', rightstack);
			}

			else{

				while((c!='!')&&(isparentheses(c)!=1)&&(priority(input[0][i])<=priority(c))){

					if(steps!='\n'){printf("comparing %c to %c ...\n", input[0][i], c);}

					push(c, 'L', leftstack); push(' ', 'L', leftstack); c=pop('R', rightstack);
				}

				push(input[0][i], 'R', rightstack);
			}
		}

	 //Parentheses

		else if(isparentheses(input[0][i])==1){

			if((input[0][i]=='{')||(input[0][i]=='[')||(input[0][i]=='(')){

				push(input[0][i], 'R', rightstack);
			}

			else if((input[0][i]=='}')||(input[0][i]==']')||(input[0][i]==')')){

				c=pop('R', rightstack);

				while((c!='!')&&(compare(input[0][i], c)!=1)&&(c!=' ')){

					if((c!='{')&&(c!='[')&&(c!='(')){

						push(c, 'L', leftstack);
						push(' ', 'L', leftstack);
					}

					c=pop('R', rightstack);
				}
			}
		}
		
		else{printf("Syntax Error 1: Invalid Input\n"); exit(1);}

		fflush(stdout);
	}

 //Debugging

	//printf("[L]stack: %s\n", leftstack);
	//printf("[R]stack: %s\n", rightstack);

	if(steps!='\n'){printf("CONVERSION LOOP DONE\n");}

 //Making postfix from stacks

	int i = 0;

	c=pop('R', rightstack);
	
	while((c!='!')&&(isparentheses(c)!=1)){i++; push(c, 'L', leftstack); push(' ', 'L', leftstack); c=pop('R', rightstack);}

	input[0] = strcpy(*input, leftstack);

	return *input;
}

//EVALUATING POSTFIX

int* post2fix(char** string){
	
	int a, b;
	char c;

	int i=0, j=0, k=0, l=0, p=1, lastnum, size = strlen(*string);

	if(steps!='\n'){printf("size: %d\n", size);}

	if(size == 0){printf("Syntax Error 1: Invalid Input\n"); exit(1);}				//postfix must end in operator, otherwise error

	else if((priority(string[0][size-1])==0)&&(string[0][size-1]!=' ')){printf("Syntax Error 3: Extra Operand\n"); exit(1);}

	for(i=0; i < size; i++){
		
		if(isdigit(string[0][i])){			//here we take something like 123
											//and turn it into 100+20+3
			l=i;							//l is dummy index so as to conserve i

			while((string[0][l]!=' ')&&(string[0][l]!='\0')&&(isdigit(string[0][l]))&&(l<size)){

				k++; l++;					//k counts digits before a space
			}

			l=i;

			while((k>0)&&(l<size)){			//this loop mimics the pow() function

				k--;

				for(int i=0; i<k; i++){p=p*10;}

				j+=((string[0][l] - '0') * (p));

				p=1;

				l++;
			}

			Mpush(j, mathstack);			//j contains the result of the operation

			i=l-1;							//resetting i to where it was
		
			k=0; j=0;
		}

		else if(string[0][i]==' '){}		//skipping spaces

		else{								//these are necessarily operators

			b=Mpop(mathstack);				//2147483647 is chosen to represent empty int stack
			a=Mpop(mathstack);

			if((a==2147483647)||(b==2147483647)){printf("Syntax Error 2: Extra Operator\n"); exit(1);}

			else{if(steps!='\n'){printf("Mathing: %d %c %d \n", a, string[0][i], b);}

				lastnum=math(a, b, string[0][i]);

				Mpush(lastnum, mathstack);	//lastnum should be bottom of mathstack
			}								//if operators and operands balance out
		}
	}

	if(lastnum!=mathstack[0]){printf("Syntax Error 3: Extra Operand\n"); exit(1);}

	if(steps!='\n'){printf("EVALUATION LOOP DONE\n");}
	
	return mathstack;
}

//DRIVER

int main(){

	char* in = malloc(sizeof(char)*max);

	char* post = malloc(sizeof(char)*max);

	int* out = malloc(sizeof(int)*max);

	int size;

	printf("\n//This program converts string equations to int results//\n//[e.g., (1+2)^3 -> 27]\t\t[current char max = %d]//\n\n", max);

	printf("Please Input String: ");

	scanf("%[^\n]%*c", in);

	size = strlen(in);

	printf("\n\t\t*just press enter to skip the steps*\n\t\t*otherwise press anything else first*\n");

	steps = getchar();

 //check

	fflush(stdout);

	if(steps!='\n'){printf("Checking for balance...\n");}

	if(checkbalance(in)==1){if(steps!='\n'){printf("String is balanced\n");}}

	else{printf("String is Unbalanced\n"); exit(1);}

 //in

	fflush(stdout);

	post=in2postfix(&in);

	printf("Postfix is: %s\n", post);

 //out

	fflush(stdout);

	out=post2fix(&post);

	printf("Result is: %d\n", out[0]);

	//strings should be freed by system

	return 0;
}
//Program can handle brackets of any sort, yet only ints for now.

