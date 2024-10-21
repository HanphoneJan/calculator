#include "calculator.h"
#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
#include <getopt.h>
#include<curses.h>
#include<readline/history.h>
#include<readline/readline.h>
#define CLOSE "\001\033[0m\002" // 关闭所有属性
#define BLOD "\001\033[1m\002" // 强调、加粗、高亮
#define BEGIN(x,y) "\001\033["#x";"#y"m\002" // x: 背景，y: 前景
#define MAX 1024
// #define debug


enum type{lf,word};
double* calculate(char* infix,FILE *history);
double result;
int i=0;

int main(int argc, char* argv[])
{ 
    FILE* history= fopen("./history.txt", "a+");
    char *infix=NULL;
    char in[MAX]={0};
    char expression[MAX]={0};
    startUI();
    read_history("./history.txt");
    if(NULL!=argv[1]){
    char *filename = argv[1];
    FILE *input = fopen(filename, "r+");
    char *fileoutput = argv[2];
    infix=&expression[0];
    int lines=0;
    if (input == NULL)
    {
        printf("Error:open file %s ", filename);
        exit(1);
    }
    while(!feof(input)){
            fgets(expression,MAX,input);
            while(expression[i]!='\n'&& i<MAX-1){
            i++;
            }
            expression[i]='\0';
            lines++; // 统计行数
            if(NULL!=argv[2]){
                FILE *output=fopen(fileoutput, "a+");
                double* re=calculate(expression,history);
                    if(NULL==re){
                    fprintf(output,"Result%d:Incorrect input\n",lines);
                    }
                    else{
                    fprintf(output,"Result%d:%lf\n",lines,*re);
                    }
		i=0;
                fclose(output);
            }
            else{
            calculate(expression,history);
            i=0;
            }
        }
    fclose(input);
    }

    while(1) {
    //开始输入

	infix = readline(BEGIN(49, 34)BLOD"Enter:"CLOSE);
	add_history(infix);
	if(infix==NULL){
		continue;
	}
	//quit正常退出程序的方式
    if (strcmp(infix, "quit") == 0) {
    //free(infix);
    #ifdef debug
	    fprintf(history,"%s\n",infix);
    #endif
    printf("Succeessful Exit.\n");
    break;
    }
//help打印提示
    if (strcmp(infix, "help") == 0) {
    fprintf(history,"%s\n",infix);
    help();
    continue;
    }
    #ifdef debug
   	 fprintf(history,"%s",infix);
    #endif
    if(strcmp(infix,"debug")==0){
	   continue;
    }
    calculate(infix,history);
    }
fclose(history);
free(infix);
return 0;
}


double* calculate(char* infix,FILE *history){
Stack* cac;cac = (Stack*)malloc(sizeof(Stack));StackInit(cac); //用于暂时储存运算符
Stack* num;num = (Stack*)malloc(sizeof(Stack));StackInit(num); //用于储存后缀表达式运算的结果
char postfix[MAX];
int safe=0,delete=0,bracket=0;

if (infix == NULL) {
    printf("please enter again.\n");
    return NULL;
}
else {

//判定是数字还是运算符并储存到栈当中
int j = 0;
for (i = 0; infix[i] != '\0'; i++) {

    //检查单个字符
    if(infix[i]==32){ //去空格
        continue;
    }
    if(checkinput(infix[i])){
        safe = 1;
        break;
    }
    if(i==0 && infix[i]=='-'){
        if(isdigit(infix[i+1])){
            postfix[j] = infix[i];
            j++;
            while (1) {
            if ((!isdigit(infix[i + 1]) && (infix[i + 1] != '.'))) {
            postfix[j] = '#';
            j++;
            break;
            } 
            else {
            i++;
            postfix[j] = infix[i];
            j++;
            }
            }
        }
        else if(infix[i+1]=='('){
            postfix[j] = '0';
            j++;
            postfix[j]='#';
            j++;
            StackPush(cac,&infix[0],word);
        }
        else{
            safe=1;
            break;
        }
    }

    else if (isdigit(infix[i])) {
        while (1) {
            if ((!isdigit(infix[i + 1]) && (infix[i + 1] != '.'))) {
                postfix[j] = infix[i];j++;
                postfix[j] = '#';j++;
                break;
            } 
            else {
                postfix[j] = infix[i];
                i++;
                j++;
            }
        }
    }
    else if (isoperator(infix[i])){ 
    //读取运算符并根据优先级判定执行操作
        while (1) {
        if ((StackEmpty(cac)) || cac->ch[(cac)->top - 1] =='(') { //运算符栈顶为左括号时或者空时
        StackPush(cac,&infix[i],word);
        break;
        }
        else if ((!StackEmpty(cac)) && (isoperator(cac->ch[(cac)->top - 1]) &&(Precedence(infix[i]) > Precedence(cac->ch[(cac)->top - 1])))) {
        //运算符栈顶为运算符且其优先级小于infix[i]时
        StackPush(cac, &infix[i],word);
        break;
        }
        else {
        StackPop(cac, &postfix[j],word); //将运算符栈顶输入前缀表达式
        j++;
        }
        }    
    } 

    else if (infix[i] == '(') {
        //左括号就直接入栈
        StackPush(cac, &infix[i],word);
        bracket++;
        if(infix[i+1]=='-'){
            postfix[j]='@';
            j++;
            i++;
            postfix[j]=infix[i];
            j++;
        }
    } 
    else if (infix[i] == ')') {
        bracket++;
        while (StackEmpty(cac) == 0 && isoperator(cac->ch[(cac)->top - 1])) {
        StackPop(cac, &postfix[j],word);
        j++;
        if (cac->ch[(cac)->top - 1] == '(') {
        (cac)->top--;
        break;
        }
        }
        if((cac->ch[(cac)->top - 1])== '('){
            (cac)->top--;
        }
    }

}

    if(bracket%2!=0){
        safe=1;
    }
    if(safe==1){
    printf("Error:incorrect input.\n");
    fprintf(history,"Incorrect input.\n");
    return NULL;
    }

    while (StackEmpty(cac) == 0) {
        postfix[j] = cac->ch[(cac)->top - 1];
        --cac->top;
        j++;
    }
    postfix[j] = '\0'; 
	#ifdef debug
    		printf("postfix:%s",postfix);

	#endif
//已经转出了类前缀表达式，没有括号
j = 0;int m=0;
double num1,num2;double digit[MAX]={0};
char c[MAX] = "";char d[MAX] = "";
char *e = NULL;
while (postfix[j] != '\0') {
    if(postfix[j]=='(' || postfix[j]==')' ){
        safe=1;
        break;
    }
    //如果是数字就不断追加到字符串数组中
    while(j==0 && postfix[0]=='-'){
        c[0] = postfix[j];
        c[1] = '\0';
        strcat(d, c);
        j++;
        while ((isdigit(postfix[j])) || (postfix[j] == '.')) {
        c[0] = postfix[j];
        c[1] = '\0';
        strcat(d, c);
        j++;
        if (postfix[j] == '#') { //遇到#标记就弹出#并退出循环
        j++;
        digit[m] = (double) strtod(d, &e); //运用strtod函数完成类型转换
        strcpy(d, "");
        StackPush(num, &digit[m], lf);m++;
        break;
        }
        }
    }
    while(postfix[j]=='@'){
        j++;
        c[0] = postfix[j];
        c[1] = '\0';
        strcat(d, c);
        j++;
        while ((isdigit(postfix[j])) || (postfix[j] == '.')) {
        c[0] = postfix[j];
        c[1] = '\0';
        strcat(d, c);
        j++;
        if (postfix[j] == '#') { //遇到#标记就弹出#并退出循环
        j++;
        digit[m] = (double) strtod(d, &e); //运用strtod函数完成类型转换
        strcpy(d, "");
        StackPush(num, &digit[m], lf);m++;
        break;
        }
        }
    }

    while ((isdigit(postfix[j])) || (postfix[j] == '.')) {
    c[0] = postfix[j];
    c[1] = '\0';
    strcat(d, c);
    j++;
    if (postfix[j] == '#') { //遇到#标记就弹出#并退出循环
    j++;
    digit[m]= (double) strtod(d, &e); //运用strtod函数完成类型转换
    strcpy(d, "");
    StackPush(num, &digit[m], lf);m++;
    break;
    }
    }

    if (isoperator(postfix[j])) { //遇到运算符就将两个数字弹出栈进行运算
        if(StackPop(num, &num1,lf)==-1){
            safe=1;
            break;
        }
        if(StackPop(num, &num2,lf)==-1){
            safe=1;
            break;
        }
        result = Calculate(postfix[j], num1, num2,&safe);
        StackPush(num, &result,lf);
        j++;
        
    }
    else{
        result=digit[m];
    }
}
    if (StackSize(num) != 1 ) { //最后栈中只有结果才是正确的
        safe=1;
    }

    if(safe==1){
    printf("Error:incorrect input.\n");
    fprintf(history,"Incorrect input.\n");
    return NULL;
    }
    printf("Result:%lf\n", result);
    fprintf(history,"Result:%lf\n",result);//将结果写入文件
    
    free(cac);
    free(num);
    return &result;
}
}


/*
double* calculate(char* infix,FILE *history){
Stack* cac;cac = (Stack*)malloc(sizeof(Stack));StackInit(cac); //用于暂时储存运算符
Stack* num;num = (Stack*)malloc(sizeof(Stack));StackInit(num); //用于储存后缀表达式运算的结果

char postfix[MAX];
int safe=0,delete=0,bracket=0;

if (infix == NULL) {
    printf("please enter again.\n");
    return NULL;
}
else {


//判定是数字还是运算符并储存到栈当中
int j = 0;
for (i = 0; infix[i] != '\0'; i++) {
    //检查单个字符
    if(checkinput(infix[i])){
        safe = 1;
        break;
    }
    if (isdigit(infix[i])) {
    while (1) {
    if ((!isdigit(infix[i + 1]) && (infix[i + 1] != '.'))) {
    postfix[j] = infix[i];
    j++;
    postfix[j] = '#';
    j++;
    break;
    } 
    else {
    postfix[j] = infix[i];
    i++;
    j++;
    }
    }
    }
    else if (isoperator(infix[i])){ 
    //读取运算符并根据优先级判定执行操作
    while (1) {
    char *oper=&infix[i];
    if ((StackEmpty(cac)) || cac->ch[(cac)->top - 1] =='(') {
    StackPush(cac,oper,word);
    break;
    }
    else if ((!StackEmpty(cac)) && (isoperator(cac->ch[(cac)->top - 1]) &&(Precedence(infix[i]) > Precedence(cac->ch[(cac)->top - 1])))) {
    StackPush(cac, oper,word);
    break;
    }
    else {
    StackPop(cac, &postfix[j],word);
    j++;
    }
    }    
    } 
    else if (infix[i] == '(') {
    StackPush(cac, &infix[i],word);
    bracket++;
    } 
    else if (infix[i] == ')') {
        bracket++;
        while (StackEmpty(cac) == 0 && isoperator(cac->ch[(cac)->top - 1])) {
        StackPop(cac, &postfix[j],word);
        j++;
        if (cac->ch[(cac)->top - 1] == '(') {
        (cac)->top--;
        break;
        }
        }
    }

}


while (StackEmpty(cac) == 0) {
postfix[j] = cac->ch[(cac)->top - 1];
--cac->top;
j++;
}
postfix[j] = '\0';
printf("postfix%s",postfix);
if(bracket%2!=0){
        safe=1;
    }

if(safe==1){
    printf("Error:incorrect input.Please enter again.\n");
    fprintf(history,"Incorrect input.\n");
    return NULL;
}


j = 0;
double num1, num2;
char c[MAX] = "";
char d[MAX] = "";
char *e = NULL;
while (postfix[j] != '\0') {
//如果是数字就不断追加到字符串数组中
while ((isdigit(postfix[j])) || (postfix[j] == '.')) {
c[0] = postfix[j];
c[1] = '\0';
strcat(d, c);
j++;
if (postfix[j] == '#') { //遇到#标记就弹出#并退出循环
j++;
num1 = (double) strtod(d, &e); //运用strtod函数完成类型转换
strcpy(d, "");
StackPush(num, &num1, lf);
break;
}
}
    if (isoperator(postfix[j])) { //遇到运算符就将两个数字弹出栈进行运算
        if (StackEmpty(num) == 1) {
        safe=1;
        break;
        }
        StackPop(num, &num1,lf);
        StackPop(num, &num2,lf);
        result = Calculate(postfix[j], num1, num2);
        StackPush(num, &result,lf);
        j++;
    }
    else{
        result=num1;
    }
}
    if (StackSize(num) != 1 ) { //最后栈中只有结果才是正确的
        safe=1;
    }

    if(safe==1){
    printf("Error:incorrect input.Please enter again.\n");
    fprintf(history,"Incorrect input.\n");
    return NULL;
    }
    printf("Result:%lf\n", result);
    //可以将结果写入文件
    fprintf(history,"Result:%lf\n",result);
    free(cac);
    free(num);
    return &result;
}
}*/