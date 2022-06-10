int getint(void)
{

int input;
char ch;

scanf("%d",&input)!=1)//입력이 정수가 아닌동안 
{

    while((ch=getchar()) != '\n')//입력이 문자이면 입력을 비운다 

printf("error! you inputted character!\n");

printf("input a number 0 between 100\n\n");

}

return input; 

}