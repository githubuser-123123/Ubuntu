#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct node 
{
    char data[128];
    struct node *next;
}*head;
struct finalnode 
{
    char data[128];
    struct finalnode *next;
}*head1;

void insertData(char data[128])
{
    struct node *temp,*q;
    temp = (node*) malloc(sizeof(struct node));
    strcpy(temp->data,data);
    temp->next=NULL;
    if(head==NULL)
    {
        head=temp;
    }
    else
    {
        q=head;
        while(q->next!=NULL)
        {
            q=q->next;
        }
        q->next=temp;
    }
}

void insertFinalData(char data[128])
{
    struct finalnode *temp,*q;
    temp = (finalnode*) malloc(sizeof(struct finalnode));
    strcpy(temp->data,data);
    temp->next=NULL;
    if(head1==NULL)
    {
        head1=temp;
    }
    else
    {
        q=head1;
        while(q->next!=NULL)
        {
            q=q->next;
        }
        q->next=temp;
    }
}

int display()
{
    int count=0;
    struct node *q;
    if(head==NULL)
    {
        printf("\nNo Data!");
    }
    else
    {
        q=head;
        while(q!=NULL)
        {
            printf(" %s",q->data);
            count++;
            q=q->next;
        }
        
    }

    return count;
}

struct node* createflag()
{
    char flag[128];
    strcpy(flag,"11100111\0");
    struct node *temp;
    temp= (node*) malloc(sizeof(struct node));
    strcpy(temp->data,flag);
    temp->next=NULL;
    return temp;
}
struct node* createESC()
{
    char esc[128];
    strcpy(esc,"00011010\0");
    struct node *temp;
    temp= (node*)malloc(sizeof(struct node));
    strcpy(temp->data,esc);
    temp->next=NULL;
    return temp;
}

void addFlag()
{
    struct node *temp,*q;
    if(head==NULL)
    {
        printf("\nNo data!");
    }
    else
    {
        temp=createflag();
        temp->next = head;
        head=temp;
        q=head;
        while (q->next!=NULL)
        {
            q=q->next;
        }
        temp=createflag();
        q->next=temp;
        
    }
}

void checkdata()
{
    struct node *temp,*q;
    char flag[128];
    strcpy(flag,"11100111\0");
    char esc[128];
    strcpy(esc,"00011010\0");
    if(head==NULL)
    {
        printf("\nNo data!");
    }
    else
    {
        if(strcmp(head->data,flag)==0 || strcmp(head->data,esc)==0)
        {
            temp=createESC();
            temp->next=head;
            head=temp;
        }
        // printf("\n%s",head->data);
        q=head;
        // printf("\n%s",q->data);
        q=q->next;
        while(q!=NULL)
        {
            if((q->next)==NULL)
            {
                return;
            }

            else if(strcmp((q->next)->data,flag)==0  || strcmp((q->next)->data,esc)==0 )
            {
                 temp=createESC();
                 temp->next=q->next;
                 q->next=temp;
                 q=q->next;
            }
            q=q->next;
        }
    }

}

void reciver(int len)
{
    int i=0;
    char flag[128];
    strcpy(flag,"11100111\0");
    char esc[128];
    strcpy(esc,"00011010\0");
    char data[1024];
    while(i<len)
    {
        if(i==0 || i==len-1)
        {
            printf("\nEnter the data : ");
            scanf("%s",data);
            i++;
            continue;
        }
        else
        {
            printf("\nEnter the data : ");
            scanf("%s",data);
            if(strcmp(data,esc)==0)
            {
                i++;
                printf("\nEnter the data : ");
                scanf("%s",data);
                if(strcmp(data,esc)==0 || strcmp(data,flag)==0)
                {
                    insertFinalData(data);
                }
                else
                {
                    printf("\nError Occured!");
                    break;
                }
            }
            else
            {
                insertFinalData(data);
                
            }
        }
        i++;
    }
}

void displayFinalData()
{
    struct finalnode *q;
    if(head1==NULL)
    {
        printf("\nNO Data Entered!");
    }
    else
    {
        q=head1;
        while(q!=NULL)
        {
            printf(" %s",q->data);
            q=q->next;
        }
    }

}

int main()
{
    head=NULL;
    head1=NULL;
    char GivenData[128];
    char FinalData[1024];
    char temp[1024];
    int count=0;
    int i=1;
    int CountFrame=0;

    char flag[128]="11100111";
    char esc[128]="00011010";
    
    
    printf("\nEnter the how many bytes of data you want to enter : ");
    scanf("%d",&count);
    while(count)
    {

        printf("\nEnter Your %d Byte : ",i);
        memset(&GivenData[0],'\0',sizeof(GivenData));
        scanf("%s",GivenData);
        insertData(GivenData);
        i++;
        count--;
    }
    checkdata();
    addFlag();
    
    CountFrame = display();

    printf("%d",CountFrame);

    reciver(CountFrame);

    displayFinalData();


    return 0;


}