#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node
{
    char data;
    struct node *next;
} *head;

void insert(char data)
{
    struct node *temp, *q;
    temp = malloc(sizeof(struct node));
    temp->data = data;
    temp->next = NULL;
    if (head == NULL)
    {
        head = temp;
    }
    else
    {
        q = head;
        while (q->next != NULL)
        {
            q = q->next;
        }
        q->next = temp;
    }
}

void display()
{
    struct node *q;
    if (head == NULL)
    {
        printf("\nThe Data is Not Entered!");
    }
    else
    {
        q = head;
        while (q != NULL)
        {
            printf(" %c", q->data);
            q = q->next;
        }
    }
}

void InsertData(char *ptr, int len)
{
    for (int i = 0; i < len; i++)
    {
        insert(*(ptr + i));
    }
}

int checkdata(char *ptr, int len)
{
    for (int i = 0; i < len; i++)
    {
        if (*(ptr + i) != '1' && *(ptr + i) != '0')
        {
            return 0;
        }
    }
    return 1;
}

struct node* createnode()
{
    struct node *temp;
    temp = malloc(sizeof(struct node));
            temp->data = '0';
            temp->next = NULL;
    return temp;

}

// start change
void AddBetween()
{
    int count = 0;
    struct node *q, *temp;
    if (head == NULL)
    {
        printf("\nData Not Present !");
    }
    else
    {
        q=head;
        while(q!=NULL)
        {
            if(q->data=='1')
            {
                count++;
                if(count==5)
                {
                    temp=createnode();
                    temp->next=q->next;
                    q->next=temp;
                    count=0;
                }
            }
            else
            {
                count=0;
            }
            q=q->next;
        }
    }
    

}

void finaldata(char *ptr)
{
    struct node *q;
    int i =0;
    if(head==NULL)
    {
        printf("\nNo data!");
    }
    else
    {
        q=head;
        while (q!=NULL)   
        {
            *(ptr+i) = q->data;
            i++;
            q=q->next;
        }
        *(ptr+i)='\0';
        

    }
}

void clientReader(char *ptr,int len)
{
    int count=0;
    for(int i=0;i<len;i++)
    {
        
        if(*(ptr+i)=='1')
        {
            count++;
            printf(" %c",*(ptr+i));
        }
        else
        {
        if(count==5)
        {
            count=0;
            continue;
        }
        count=0;
        printf(" %c",*(ptr+i));
        }

    }
}

int main()
{
    char GivenData[1024];
    char FinalData[1024];
    head = NULL;
c:
    printf("\nEnter Your data : ");
    scanf("%s", GivenData);
    if (checkdata(&GivenData[0], strlen(GivenData)))
    {
        InsertData(&GivenData[0], strlen(GivenData));
        display();
        printf("\n");
        AddBetween();
        display();
        finaldata(&FinalData[0]);
        printf("\nFinal Data : %s ",FinalData);
        printf("\n");
        clientReader(&FinalData[0],strlen(FinalData));
    }
    else
    {
        printf("\nPlease Only enter '1' & '0' in the data ");
        goto c;
    }
    
}