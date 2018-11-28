
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define    MAX_LEN      20
#define    COURSE_NUM   6
#define    CASE     break;case

typedef struct Student
{
	long num;						/* 每个学生的学号 */
	char name[MAX_LEN];         	/* 每个学生的姓名 */
	float score[COURSE_NUM];		/* 每个学生COURSE_NUM门功课的成绩 */
	float sum;                   	/* 每个学生的总成绩 */
	float aver;               	    /* 每个学生的平均成绩 */
	struct Student *next;
}student;

int menu(void)
{
	int a = 0;
	printf("1.Input record\n"\
		"2.Calculate total and average score of every course and each student\n"\
		"3.Sort in descending order by total score of every student\n"\
		"4.Sort in ascending order by total score of every student\n"\
		"5.Sort in ascending order by number\n"\
		"6.Sort in dictionary order by name\n"\
		"7.Search by number\n"\
		"8.Search by name\n"\
		"9.Statistic analysis for every course\n"\
		"10.List record\n"\
		"11.Write to a file\n"\
		"12.Read from a file\n"\
		"0.Exit\n"\
		"Please enter your choice :");
	scanf("%d", &a);
	return a;
}

student* input(int sno, int cno)
{
	student *head, *p, *current = NULL;

	head = (student*)malloc(sizeof(student));
	current = head;
	printf("Input data\n");
	for (register int i = 0; i < sno; i++)
	{
		p = (student*)malloc(sizeof(student));
		if (p == NULL) return NULL;

		scanf("%ld %s", &p->num, p->name);
		for (register int j = 0; j < cno; j++)
		{
			scanf("%f", &p->score[j]);
		}
		current->next = p;
		p->next = NULL;
		current = p;
	}
	return head;
}

void cul(student *p, int sno, int cno, float *cs, float *ca)
{
	for (register int i = 0; i < sno; i++)
	{
		p->sum = 0;
		for (register int j = 0; j < cno; j++)
		{
			p->sum += p->score[j];
			cs[j] += p->score[j];
		}
		p->aver = p->sum / 3;
		p = p->next;
	}
	for (register int i = 0; i < cno; i++)
	{
		ca[i] = cs[i] / cno;
	}
	printf("done");
}

student* sorts(student *head, int cno, int flag)
{
	int i = 0;
	student *new, *current = head;
	student *new_head, **ncurrent = NULL, *now = NULL;

	new_head = (student*)malloc(sizeof(student));
	if (new_head == NULL) return NULL;

	new_head->num = -1;
	new_head->sum = (flag == 0) ? 1000000 : -2;
	new_head->next = NULL;
	strcpy(new_head->name, "A");

	while ((current = current->next) != NULL)
	{
		new = (student*)malloc(sizeof(student));
		if (new == NULL) return NULL;

		new->num = current->num;
		strcpy(new->name, current->name);
		for (i = 0; i < cno; i++)
		{
			new->score[i] = current->score[i];
		}
		new->sum = current->sum;
		new->aver = current->aver;

		ncurrent = &new_head;
		if (flag == 0)//按学生总分从高到低
		{
			while ((now = *ncurrent) != NULL && now->sum > new->sum)
			{
				ncurrent = &now->next;
			}
		}
		else if (flag == 1)//按学生总分从低到高
		{
			while ((now = *ncurrent) != NULL && now->sum < new->sum)
			{
				ncurrent = &now->next;
			}
		}
		else if (flag == 2)//按学号
		{
			while ((now = *ncurrent) != NULL && now->num < new->num)
			{
				ncurrent = &now->next;
			}
		}
		else if (flag == 3)//按姓名
		{
			while ((now = *ncurrent) != NULL && strcmp(now->name, new->name) <= 0)
			{
				ncurrent = &now->next;
			}
		}
		new->next = now;
		*ncurrent = new;
	}
	return new_head;
}

void searches(student *p, int cno, int flag)
{
	student *current = p;
	long int number;
	char name1[20];
	int i = 0;

	if (flag == 2)
	{
		printf("Input the student's number:");
		scanf("%ld", &number);
		while (current->next != NULL)
		{
			if (current->num == number) break;
			current = current->next;
		}
	}
	else if (flag == 3)
	{
		printf("Input the student's name:");
		scanf("%s", name1);
		while (current->next != NULL)
		{
			if (strcmp(current->name, name1) == 0) break;
			current = current->next;
		}
	}
	printf("\n%ld   %s   ", current->num, current->name);
	for (i = 0; i < cno; i++)
	{
		printf("%.1f   ", current->score[i]);
	}
	printf("sum:%.1f   aver:%.2f  ", current->sum, current->aver);

}

void print(student *p, int cno, float *cs, float *ca)
{
	int i;
	printf("ID number  Name   ");
	for (i = 0; i < cno; i++)
	{
		printf("Course %d   ", i + 1);
	}
	printf("sum       aver\n");

	while (p != NULL)
	{
		printf("%7ld    %s ", p->num, p->name);
		for (i = 0; i < cno; i++)
		{
			printf("%7.1f   ", p->score[i]);
		}
		printf("     %.1f    %.2f\n", p->sum, p->aver);
		p = p->next;
	}
	printf("         Coursesum       aver\n");
	for (i = 0; i < cno; i++)
	{
		printf("Course%d  %4.3f   %.3f\n", i + 1, cs[i], ca[i]);
	}
}

int fileout(student *p, int cno, float *cs, float *ca)
{
	int i = 0;
	char filename[30];
	printf("Name this file: ");
	scanf("%s", filename);

	FILE *fp = NULL;
	if ((fp = fopen(filename, "w")) == NULL) return 0;
	
	fputs("ID number  Name   ", fp);
	for (i = 0; i < cno; i++)
	{
		fprintf(fp, "Course%d   ", i + 1);
	}
	fprintf(fp,"sum       aver\n");

	while(p != NULL)
	{
		fprintf(fp, "%7ld    %s ", p->num, p->name);
		for (i = 0; i < cno; i++)
		{
			fprintf(fp, "%7.1f   ", p->score[i]);
		}
		fprintf(fp, "     %.1f    %.2f\n", p->sum, p->aver);
		p = p->next;
	}
	fprintf(fp, "         Coursesum       aver\n");
	for (i = 0; i < cno; i++)
	{
		fprintf(fp, "Course%d  %4.3f   %.3f\n", i + 1, cs[i], ca[i]);
	}
	fclose(fp);
	return 1;
}

int filein(void)
{
	char *text;
	char filename[30];
	int textlength = 0;
	FILE *fp = NULL;
	printf("Open a file:");
	scanf("%s", filename);
	if ((fp = fopen(filename, "rb")) == NULL) return 0;
	
	fseek(fp, 0, SEEK_END);
	textlength = ftell(fp);
	text = (char*)malloc(textlength * sizeof(char));
	rewind(fp);

	textlength = fread(text, 1, textlength, fp);
	text[textlength] = '\0';
	fclose(fp);

	printf("%s", text);
	return 1;
}
int main() {

	student *head = NULL, *p1 = NULL;
	int sno, cno, manner;
	printf("Input students number and course number");
	scanf("%d %d", &sno, &cno);	

	float course_score[6] = { 0 };
	float course_aver[6] = { 0 };

	printf("\nWhat to do now?");
	manner = menu();
	
	while (1)
	{
		switch (manner)
		{
			CASE 1: head = input(sno, cno);
					if (head == NULL) exit(0);
			CASE 2: cul(head->next, sno, cno, course_score, course_aver);
			CASE 3: head = sorts(head, cno, 0);
			CASE 4: head = sorts(head, cno, 1);
			CASE 5: head = sorts(head, cno, 2);
			CASE 6: head = sorts(head, cno, 3);
			CASE 7: searches(head, cno, 2);
			CASE 8: searches(head, cno, 3);
			CASE 9: print(head->next, cno, course_score, course_aver);
			CASE 10: if(fileout(head->next, cno, course_score, course_aver))
						printf("done");
			CASE 11: filein();
			CASE 0 : return 0;
		}
		printf("\nWhat to do then?");
		scanf("%d", &manner);
	}
}