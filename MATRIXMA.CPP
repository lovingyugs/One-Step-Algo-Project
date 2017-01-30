 #include<iostream.h>
 #include<stdio.h>
 #include<string.h>
 #include<graphics.h>
 #include<math.h>
 #include<stdlib.h>
 #include<dos.h>
 #include<conio.h>
 #include<assert.h>
 #include<time.h>

 //typedef struct node;
 typedef struct path		//heuristic values for AI
 {
	int h_value;
	int g_value;
	int f_value;
	int added;
 }path;

 typedef struct node  		//2d array
 {
	int v;
	int mrow,mcol;
	float cx,cy;
	node*parent;
	path *p;
 }node;

 typedef struct bot3                      //bot3 array
 {
	int region;
	int l;
	node *dest[5];
 }bot3;

node *a[30][30];        //our main 2d matrix array
node *bots[4];		 //array which stores current position of vans
node *next[900];
node *closed[900];
node *bot3_path[200];

node*bot3_goal[5];                 //stores bot3 goals to traverse
node*bot2_array[11];
bot3*bot3_array[10];

node *same;
int bot3_goalno=0,bot3_current=0;
int bot3_diff=0,bot3_run=0;
int matcount=24;      //matrix row=24,column=26
int once=0;           //loop for the first tym when the file is read
int vancount;        //no of vans
int prow=0,pcol=0;   //player postion
int erow=0,ecol=0;   //exit position
int movecount=0;     //no of moves player has made||score
int bot2go=0;
int bot2_run=0;
char name[20];      //user name
//int bnk=0;

int highscores(int);           //calculates highscore
int  home();                   //first page
void scorecard();
void gotoxy(int x,int y);
void matrix();
node* retmin(node*array[900],int);
void remove(node*,node*array[900],int);
node*retbot(int,int);

void emptypath()
{
int i,j;
for(j=0;j<matcount;j++)
{
for(i=0;i<matcount+2;i++)
{
//a[j][i]->p=new path;
a[j][i]->p->h_value=0;
a[j][i]->p->g_value=0;
a[j][i]->p->f_value=1000;
a[j][i]->parent=NULL;
a[j][i]->p->added=0;

a[j][i]->parent=NULL;
}
}

}

void empty()
{
int i,j;
for(j=0;j<matcount;j++)
{
for(i=0;i<matcount+2;i++)
{
a[j][i]=new node;
a[j][i]->v=0;
a[j][i]->cx=0;
a[j][i]->cy=0;
a[j][i]->p=new path;
}
}
emptypath();

a[j][i]=NULL;
}

void pathcolor(node *temp,int color)
{
		setcolor(color) ;
		setfillstyle(SOLID_FILL,color);
		circle(temp->cx,temp->cy,3);
		floodfill(temp->cx,temp->cy,color);
	//	getch();
}

void vancolor(node *temp)
{
	setcolor(4);
	setfillstyle(SOLID_FILL,4);
	circle(temp->cx,temp->cy,5);
	floodfill(temp->cx,temp->cy,4);
}

void exit()
{
 int i=0,j,val,count;

 randomize();
 for(j=0;j<4;j++)
 {
	i=random(18);        //to make it more random
 }

 for(j=1;j<25;j++)
 {
	val=a[1][j]->v;
	if(val!=0)
	{
		if(j==i)    //finding the i'th open block
		{
			erow=0;
			ecol=j;
			break;
		}
	}
 }
 return;
}

void bot2_dp()
{
int i,j;
int count=0,get=0;
int row,col;
FILE *fp;
fp=fopen("bot2ai.txt","r");
for(i=0;i<11;i++)
{
	get=fscanf(fp,"%d %d",&row,&col);
	if(get!=2)
	{
		break;
	}
	else
		bot2_array[i]=a[row][col];
}
	//bot2_array[i]=count;
	fclose(fp);
}

node*give_bot2_goal()
{
node*temp;
int outer;
if(bot2go==0)
{
randomize();
outer=random(11);
bot2go=outer;
//outer=1;
temp=bot2_array[bot2go++];
}
else
{
	temp=bot2_array[bot2go++];
	if(bot2go==11)
		bot2go=0;
}
return temp;
}

void bot3_dp()
{
int i,j,row,col;
int count=0,get=0;
FILE *fp;
int flag=1;
fp=fopen("botai.txt","r");
for(i=0;i<8;i++)
{       //flag=1;
	bot3_array[i]=new bot3;
	bot3_array[i]->region=i;
	count=0;
	while(flag==1)
	{
		get=fscanf(fp,"%d %d",&row,&col);
	       //	cout<<row<<" "<<col<<",";
		if(get!=2 || (row==0 && col==0))
		{
			break;
		}
		else
			bot3_array[i]->dest[count++]=a[row][col];
	//		cout<<bot3_array[i]->dest[count-1]->mrow<<" "<<bot3_array[i]->dest[count-1]->mcol<<",";

	}
      //	getch();
	bot3_array[i]->l=count;

}
fclose(fp);
}

node* givegoal()
{
int i,j,outer,inner;

bot3*temp;
node*tempin;
if(bot3_goalno==0)
{
randomize;
outer=random(8);
//outer=1;
temp=bot3_array[outer];
randomize;

inner=random(temp->l);
//inner=0;
}
j=0;

int tcount=0;
if(bot3_goalno<=0)
{
	for(i=inner;i<temp->l;i++)
	{       //cout<<"here ";
      //		cout<<" "<<temp->dest[i]->mrow<<" "<<temp->dest[i]->mcol;
		bot3_goal[j]=temp->dest[i];
		j++;
	}
	bot3_goalno=j;
	tempin=bot3_goal[--bot3_goalno];

}
else
{
	tempin=bot3_goal[--bot3_goalno];

	gotoxy(70,4);
		//gotoxy(70,8+tcount);
      //	cout<<tempin->mrow<<","<<tempin->mcol<<" ";
      //	bot3_goal[bot3_goalno]=NULL;
}
return tempin;
}


int heuristic(node*current,node*start,node*goal)
{
node*temp;
temp=current;
int oldg,oldh,oldf;
oldg=temp->p->g_value;
oldh=temp->p->h_value;
oldf=temp->p->f_value;
//cout<<current->mrow<<" ";
//cout<<start->mrow<<" ";
gotoxy(70,10);
//cout<<"before ";
long double x2=(current->mrow - start->mrow)*(current->mrow - start->mrow);
//cout<<" "<<x2<<" ";
//cout<<" "<<(current->mrow - start->mrow)*(current->mrow - start->mrow);
//cout<<" - "<<current->mcol;
//cout<<" "<<start->mcol;

long double y2=(current->mcol - start->mcol)*(current->mcol - start->mcol);
//cout<<" "<<y2<<" ";
//cout<<" "<<(current->mcol - start->mcol)*(current->mcol - start->mcol);
//cout<<" "<<x2<<" "<<y2;

temp->p->g_value=start->p->g_value+10.0*sqrt(x2+y2);
//cout<<"("<<temp->mrow<<","<<temp->mcol<<") "<<temp->p->g_value<<" "; //here

//cout<<current->mrow<<" " ;
//cout<<" "<< goal->mrow<<" ";
//cout<<" "<<current->mrow - goal->mrow;
x2=(current->mrow - goal->mrow)*(current->mrow - goal->mrow);
//cout<<" "<<(current->mrow - goal->mrow)*(current->mrow - goal->mrow);

//cout<<" "<<x2<<" ";

//cout<<current->mcol<<" ";
//cout<<" "<<goal->mcol<<" ";

//cout<<current->mcol-goal->mcol<<" ";

y2=(current->mcol- goal->mcol)*(current->mcol-goal->mcol);
//cout<<" "<<(current->mcol- goal->mcol)*(current->mcol-goal->mcol);
//cout<<" "<<y2<<" ";
//getch();

temp->p->h_value=10.0*sqrt(x2+y2);
//cout<<temp->p->h_value<<" ";                   //here
//cout<<endl;
//getch();

gotoxy(70,12);
//cout<<"after ";

temp->p->f_value=temp->p->g_value+temp->p->h_value;
if(oldf < temp->p->f_value)
{
temp->p->g_value=oldg;
temp->p->h_value=oldh;
temp->p->f_value=oldf;
}
else
{
temp->parent=start;
}
if(temp->p->added==0)
{
	temp->p->added=1;
	return 1;
}
else
	return 0;
}

node* makevans(int count,int row,int col)
{
node*temp,*start,*goal,*current;
int list=0,done=0,k=0,success=0,i,flag=0,j;
i=0;j=0;

if(count==0  || count==1 || (count==3 && bot3_run==0) || count==2)
{
	int success=0, add;
	if(count==3)
	{
		if(bot3_run==0)
		{
			start=a[row][col];
			goal=givegoal();

		}
	}
	if(count==0)
	{
		start=a[row][col];
		goal=a[prow][pcol];
	}

	else if(count==1)
	{
		start=a[row][col];
		goal=a[prow][pcol];
	}
	else if(count==2)
	{
		start=a[row][col];
		if(bot2_run==0)
		{
			goal=give_bot2_goal();
			same=goal;
			bot2_run=1;
		}
		else
		goal=same;
	}
	if(start==goal)
	 {	temp=start;
		return temp;
	 }
//	cout<<start->mrow<<","<<start->mcol;
  //	gotoxy(70,20);
//	cout<<goal->mrow<<","<<goal->mcol;
    //	getch();
	next[list++]=start;
	//closed[done++]=start;
	start->p->g_value=0;
	start->p->h_value=0;
	start->p->f_value=0;
	start->p->added=1;
	//current=start;
	int tcount=0;
	while(success==0)
	{
	  flag=0;
	 // current=NULL;
	  current=retmin(next,list);
	  gotoxy(70,3+tcount);
	  cout<<"";
	 // cout<<current->mrow<<" "<<current->mcol<<" "<<endl;

	 //getch();
		closed[done++]=current;
		closed[done]=NULL;
	   if(count==0)
	   {
		//pathcolor(closed[done-1],6);
	     //	getch();
	   }

		remove(current,next,list);
		i=current->mrow;
		j=current->mcol;
		//cout<<i-1<<","<<j-1 ;
	       //	getch();
		if(current->mrow==goal->mrow && current->mcol==goal->mcol)
		{
			closed[done]=NULL;
			success=1;
			break;
		}
		if(count==0)// && (prow-erow)<=10 || (pcol>=13 && abs(prow-row)<15))
		{
			for(k=0;k<3;k++)
			{
				if(a[i-1][j-1+k]->v!=0 && j<26)
				{
				add=heuristic(a[i-1][j-1+k],current,goal);
					if(add==1 && j<26)
					{
						next[list++]=a[i-1][j-1+k];
						flag=1;
					//	pathcolor(next[list-1],15);
					}
				}
			}
			if(a[i][j+1]->v!=0 && j<26)
			{
				add=heuristic(a[i][j+1],current,goal);
				if(add==1 && j<26)
				{
					next[list++]=a[i][j+1];
					flag=1;
				      //	pathcolor(next[list-1],15);
				}
			}
			for(k=0;k<3;k++)
			{
				if(a[i+1][j+1-k]->v!=0 && j<26)
				{
				add=heuristic(a[i+1][j+1-k],current,goal);
					if(add==1 && j<26)
					{
						next[list++]=a[i+1][j+1-k];
						flag=1;
					  //	pathcolor(next[list-1],15);
					}
				}
			}
			if(a[i][j-1]->v!=0 && j<26)
			{
				add=heuristic(a[i][j-1],current,goal);
				if(add==1 && j<26)
				{
					next[list++]=a[i][j-1];
					flag=1;
					//pathcolor(next[list-1],15);
				}
			}
			if(flag==1)
				next[list]=NULL;
		    //	getch();


		}
		else if(count==1 || count==0 || count==3 || count==2)
		{
			if(a[i-1][j]->v!=0 && j<26)
			{
				add=heuristic(a[i-1][j],current,goal);
				if(add==1 && j<26)
				{
					next[list++]=a[i-1][j];
					flag=1;
				   //	pathcolor(next[list-1],15);
				}
			}
			if(a[i][j+1]->v!=0 && j<26)
			{
				add=heuristic(a[i][j+1],current,goal);
				if(add==1 && j<26)
				{
					next[list++]=a[i][j+1];
					flag=1;
				   //	pathcolor(next[list-1],15);
				}
			}

			if(a[i+1][j]->v!=0 && j<26)
			{
				add=heuristic(a[i+1][j],current,goal);
				if(add==1 && j<26)
				{
					next[list++]=a[i+1][j];
					flag=1;
				 //	pathcolor(next[list-1],15);
				}
			}
			if(a[i][j-1]->v!=0 && j<26)
			{
				add=heuristic(a[i][j-1],current,goal);
				if(add==1 && j<26)
				{
					next[list++]=a[i][j-1];
					flag=1;
				   //	pathcolor(next[list-1],15);
				}
			}
			if(flag==1)
				next[list]=NULL;
				//getch();
		  //	gotoxy(70,15);
		    //	cout<<"prob after";
		}
       }
       if(count==3 && bot3_run==0)
       {
		gotoxy	(70,7);
		//cout<<goal->mrow<<" ,"<<goal->mcol;
		bot3_path[0]=goal;
		//bot3_path[bot3_diff]=NULL;


		temp=retbot(done,count);
		temp->v=2;
		a[row][col]->v=1;

		tcount=0;
		gotoxy(70,8);
		bot3_path[bot3_diff-1]=NULL;
		bot3_diff--;

		bot3_run=1;
		//getch();
       }
       else if(count==3 && bot3_run==1)
       {
		temp=bot3_path[bot3_diff-1];
		temp->v=2;
		bot3_diff--;
		a[row][col]->v=1;
		//getch();
       }
       //getch();
       else
       {
       temp=retbot(done,count);
       temp->v=2;
       a[row][col]->v=1;
       if(count==2)
       {
		if(temp->mrow==goal->mrow && temp->mcol==goal->mcol)
			bot2_run=0;
       }
       //if(count==0)
       //getch();
       }
}
else if(count==3 && bot3_run==1)
{
		temp=bot3_path[bot3_diff-1];
		gotoxy(70,8);
		temp->v=2;
		bot3_diff--;
		a[row][col]->v=1;
		if(bot3_diff==0)
			bot3_run=0;
		//getch();
}

return temp;
}

node *retmin(node *array[900],int l)
{
	int i,start;
	node*min;
	for(i=0;i<l;i++)
	{
		if(array[i]!=NULL)
		{
			min=array[i];
			break;
		}
	}
	start=i;

	for(i=start+1;i<l;i++)
	{
		if(array[i]!=NULL)
		{
			if(array[i]->p->f_value<min->p->f_value)
				min=array[i];
			else if(array[i]->p->f_value==min->p->f_value)
			{
				if(array[i]->p->h_value<min->p->h_value)
				{
					min=array[i];
				}
			}
		}
	}
return min;
}

void remove(node*toremove,node*array[900],int l)
{
	int i;
	for(i=0;i<l;i++)
	{
	if(array[i]->mrow==toremove->mrow && array[i]->mcol==toremove->mcol)
		{
			array[i]=NULL;
			break;
		}
	}
}

node*retbot(int l,int count)
{
int i,j;
int flag=0;
node *temp=closed[l-1];
node *temp1;
	temp1=temp;
//       cout<<"("<< temp->mrow<<","<<temp->mcol<<")";

	temp=temp->parent;
	while(temp->parent!=NULL)
	{
		if(count==0)
		{
	 //	setcolor(10) ;
	   //	setfillstyle(SOLID_FILL,10);
	 //	circle(temp->cx,temp->cy,3);
	   //	floodfill(temp->cx,temp->cy,10);
		}
	       //	getch();

		temp1=temp;

		if(bot3_run==0 && count==3)
		{

			//bot3_diff++;
			bot3_path[++bot3_diff]=temp1;
			flag=1;
		}

		temp=temp->parent;
	}
	if(flag==1)
		bot3_path[++bot3_diff]=NULL;

	//getch();
return temp1;
}

void map()
{
clrscr();
cleardevice();

matrix();
//scorecard();

//emptypath();
int i,j,k;
i=0;j=0,k=0;
int flag1=0;

setcolor(4);
int value=99,flag=0;
int bnk=0;
int player=0;
FILE*fp;
//int ex;

if(once==0)
{
	emptypath();
	fp=fopen("map25.txt","r");
 randomize();
 player=random(4);
 vancount=0;
}
else
{
	int y=0;
	/*while(y<vancount)
	{
		emptypath();

			bots[y++]=makevans(y,bots[y]->mrow,bots[y]->mcol);
	} */

	while(y<vancount)
	{
		emptypath();
		if(y==0)
		{
			for(i=1;i<4;i++)
			{
				if(bots[i]->mrow==bots[y]->mrow && bots[i]->mcol==bots[y]->mcol)
					flag1=1;

			}
		}
		if(y==1)
		{
			for(i=2;i<4;i++)
			{
				if(bots[i]->mrow==bots[y]->mrow && bots[i]->mcol==bots[y]->mcol)
					flag1=1;
			}
			if(bots[0]->mrow==bots[y]->mrow && bots[0]->mcol==bots[y]->mcol)
					flag1=1;

		}
		if(y==2)
		{
			for(i=0;i<2;i++)
			{
				if(bots[i]->mrow==bots[y]->mrow && bots[i]->mcol==bots[y]->mcol)
					flag1=1;
			}
			if(bots[3]->mrow==bots[y]->mrow && bots[3]->mcol==bots[y]->mcol)
					flag1=1;

		}
		if(y==3)
		{
			for(i=0;i<3;i++)
			{
				if(bots[i]->mrow==bots[y]->mrow && bots[i]->mcol==bots[y]->mcol)
					flag1=1;
			}

		}

		if(flag==0)
			bots[y++]=makevans(y,bots[y]->mrow,bots[y]->mcol);
		flag1=0;
	}
		emptypath();
}


//player=0;
for(i=0;i<matcount;i++)
{
for(j=0;j<matcount+2;j++)
{
	node*temp=a[i][j];
	node*temp1;
	int get=0,value;
	if(once==0)
	{
		temp->mrow=i;
		temp->mcol=j;

		get=fscanf(fp,"%d",&value);
		a[i][j]->v=value;
		if(get!=1)
		{
			flag=1;
			break;
		}
	}
	if(temp->v==1)                       //blank
	{
	}
	else if(temp->v==2 && once==0)            //vans
	{

		bots[vancount++]=temp;
		vancolor(bots[vancount-1]);
	}
	else if(temp->v==2)
	{
		vancolor(temp);
	//	makevans(vancount++,i,j);
 //		getch();
	}
	else if(temp->v==3)                  //bank
	{
	      if(player==bnk && once==0)
	      {
		setcolor(1) ;
		setfillstyle(SOLID_FILL,1);
		circle(temp->cx,temp->cy,5);
		floodfill(temp->cx,temp->cy,1);
		bnk=10;
		prow=i;
		pcol=j;
	      }

	      setcolor(WHITE);
	      int poly[14];
	      poly[0]=temp->cx-8;
	      poly[1]=temp->cy+6;
	      poly[2]=poly[0];
	      poly[3]=temp->cy-8;
	      poly[4]=poly[2]+4;
	      poly[5]=poly[3];
	      poly[6]=temp->cx-4;
	      poly[7]=temp->cy;
	      poly[8]=temp->cx+4;
	      poly[9]=poly[7];
	      poly[10]=poly[0]+8;
	      poly[11]=poly[1];
	      poly[12]=poly[0];
	      poly[13]=poly[1];
	      drawpoly(7,poly);
	  //    floodfill(poly[0]+1,poly[1]-1,RED);

	      bnk++;
	}
	//*/
	else if(temp->v==0)    //         RED
	//if((temp->v)%2!=0)// || temp->v%7==0)
	{

	      if(j==ecol && i==0 && once!=0)
	      {
			temp->v=1;
	      }
	      else
	      {

	      setcolor(8);
	      setfillstyle(SOLID_FILL,8);
	      rectangle(temp->cx - 9,temp->cy -9,temp->cx + 9,temp->cy +9);

	      floodfill(temp->cx,temp->cy,8);
	      }
	}
	else if(temp->v==5)
	{
		setcolor(1) ;
		setfillstyle(SOLID_FILL,1);
		circle(temp->cx,temp->cy,5);
		floodfill(temp->cx,temp->cy,1);
	}
//	getch();
}
if(flag==1)
break;
}
fclose(fp);
scorecard();
}

void matrix()
{
  setcolor(YELLOW);
 // settextstyle(3,HORIZ_DIR,5);

 rectangle(0,0,getmaxx(),getmaxy());
 int i,j=0,k,l;
 j=0;
 //gotoxy(10,10);
 for(k=0;k<matcount;k++)
 {
	l=0;
	for(i=0;i<matcount+2;i++)
	{
		rectangle(0+l,0+j,20+l,20+j);
		a[k][i]->cx=(1+l+20+l-1)/2;
		a[k][i]->cy=(1+j+20+j-1)/2;
		l=l+20;
	}
	j=j+20;
	//getch();
 }
}

int home()
{
int matcount=29,i,j,k,l,c;
setcolor(YELLOW);
 l=20;
 j=20;
 //setcolor(6) ;    //red
 //setfillstyle(SOLID_FILL,6);
 for(k=0;k<=matcount-8;k++)
 {
	setcolor(YELLOW);
	c=0;
	if(k%2 == 0)
	{
		l=20;
		for(i=0;i<=matcount;i++)
		{
		  setcolor(8);
		  setfillstyle(SOLID_FILL,8);
		  rectangle(0+l,0+j,20+l,20+j);
		  if(c%2 == 0)
		  {
			floodfill(l+2,j+2,8);
		  }
		  l=l+20;
		  c++;
		  delay(5);
		}
	}
	else
	{
		l=l-20;
		for(i=0;i<=matcount;i++)
		{
		  setcolor(8);
		  setfillstyle(SOLID_FILL,8);
		  rectangle(0+l,0+j,20+l,20+j);
		  if(c%2 == 0)
		  {
			floodfill(l+2,j+2,8);floodfill(l+1,j+1,8);
		  }
		  l=l-20;
		  c++;
		  delay(5);
		}
	}
	j=j+20;
 }
 setcolor(BROWN);
 setfillstyle(SOLID_FILL,BROWN);
 rectangle(getmaxx()/2-200,getmaxy()/2-200,getmaxx()/2+200,getmaxy()/2+200);
 floodfill(121,41,6);

 setcolor(GREEN);
 settextstyle(8,HORIZ_DIR,6);
 outtextxy(190,50,"ONE STEP");

 setcolor(BLUE);
 setfillstyle(SOLID_FILL,BLUE);
 rectangle(220,130,420,180);
 floodfill(221,131,1);
 setcolor(BLUE);
 setfillstyle(SOLID_FILL,BLUE);
 rectangle(220,240,420,290);
 floodfill(221,241,1);
 setcolor(BLUE);
 setfillstyle(SOLID_FILL,BLUE);
 rectangle(220,350,420,400);
 floodfill(221,351,1);

 setcolor(RED);
 settextstyle(1,HORIZ_DIR,2);
 outtextxy(290,135,"START");
 gotoxy(33,11);
 cout<<"(PRESS SPACEBAR)";
 outtextxy(270,245,"HIGHSCORE");
 gotoxy(36,18);
 cout<<"(PRESS H)";
 outtextxy(290,355,"EXIT");
 gotoxy(35,25);
 cout<<"(PRESS Esc)";
 int x=getch();
 if(x==27)
 return 2;
 else if(x==104)
 return 1;
 else if(x==32)
 return 0;
 else
 return 2;
}

void scorecard()
{
settextstyle(0,HORIZ_DIR,0);

outtextxy(530,20,"SCORECARD");
circle(530,50,5);
setfillstyle(SOLID_FILL,BLUE);
floodfill(531,51,8);
outtextxy(550,47,"-USER");
circle(530,70,5);
setfillstyle(SOLID_FILL,RED);
floodfill(531,71,8);
outtextxy(550,67,"-BOT");
/*setcolor(WHITE);
int poly[14];
poly[0]=527;
poly[1]=105;
poly[2]=poly[0];
poly[3]=105-8;
poly[4]=poly[2]+4;
poly[5]=poly[3];
poly[6]=527-4;
poly[7]=105;
poly[8]=527+4;
poly[9]=poly[7];
poly[10]=poly[0]+8;
poly[11]=poly[1];
poly[12]=poly[0];
poly[13]=poly[1];
drawpoly(7,poly);*/
rectangle(527,105,537,115);
setfillstyle(SOLID_FILL,WHITE);
floodfill(528,106,8);
outtextxy(550,107,"-FINISH");
gotoxy(67,9);
cout<<"Name-"<<name;

gotoxy(67,11);
cout<<"Moves- "<<movecount;

setfillstyle(SOLID_FILL,RED);
setcolor(GREEN);
rectangle(540,200,610,230);
outtextxy(560,210,"HINT");
rectangle(540,400,610,430);
outtextxy(560,410,"EXIT");
}

int usermove()
{
int check=1,t;
char ch[2];
while(check==1)
{
t=getch();
	if(t==27)  //esc || exit
	check=-1;
	else
	{
		if(t==119)//move up || w
		{
			if(a[prow-1][pcol]->v!=0)
			{
				//playercolor(a[prow-1][pcol]);
				a[prow-1][pcol]->v=5;
				a[prow][pcol]->v=1;
				prow=prow-1;
				pcol=pcol;
				check=0;
			}
		}
		else if(t==97)//move left || a
		{
	   //		cout<<" "<<a[prow][pcol-1]->v;
			if(a[prow][pcol-1]->v!=0)
			{
				a[prow][pcol-1]->v=5;
				a[prow][pcol]->v=1;
				prow=prow;
				pcol=pcol-1;

				check=0;
			}

		}
		else if(t==115)//move down  || s
		{
			if(a[prow+1][pcol]->v!=0)
			{
				//playercolor(a[prow-1][pcol]);
				a[prow+1][pcol]->v=5;
				a[prow][pcol]->v=1;
				prow=prow+1;
				pcol=pcol;
				check=0;
			}
		}
		else if(t==100)//move right  || d
		{
	       //		cout<<" "<<a[prow][pcol+1]->v;
			if(a[prow][pcol+1]->v!=0)
			{
				//playercolor(a[prow-1][pcol]);
				a[prow][pcol+1]->v=5;
				a[prow][pcol]->v=1;
				prow=prow;
				pcol=pcol+1;
				check=0;
			}
		}
		//cout<<" " <<check;
	}

}
return check;
}


int check()
{
if(prow==erow && pcol==ecol)
{
	gotoxy(70,18);
	cout<<" You Win ";
	return 0;
}
else
{
for(int i=0;i<vancount;i++)
{
	if(bots[i]->mrow==prow && bots[i]->mcol==pcol)
	{
		gotoxy(70,18);
		cout<<"BusteD ";
		return 0;
	}
	else
		return 1;
}
}

}



int play()
{
int x=usermove();
//cout<<" "<<x;
if(x==-1)
return 0;
else
return 1;
}

void main()
{
 clrscr();
 int game=1;
 int gd=DETECT,gm;
 FILE*fp;
 initgraph(&gd,&gm,"C:\\TC\\BGI");
 once=0;
 int run=0;
 int i,s;
 map();
//exit();
//once=1;
//clrscr();
bot2_dp();
bot3_dp();
exit();
once=1;
 clrscr();
 cleardevice();
 once=0;
 matcount=26;
 run=home();
 //once=1;
 matcount=24;
 if(run==1)
 {
   fp=fopen("highscor.txt","r");
   clrscr();
   cleardevice();
   gotoxy(0,0);
   cout<<"HIGH SCORES:"<<endl;
   for(i=0;i<5;i++)
   {
    fscanf(fp,"%d",&s);
    cout<<s<<endl;
   }
   getch();
   clrscr();
 }
 else if(run==0)
 {
 clrscr();
 cleardevice();
 cout<<"Enter Your Name :- ";
 gets(name);
 gotoxy(1,1);
 empty();			       //assign null to all nodes
 //matrix();			       //creates plane matrix;
 //gotoxy(10,10);

/* for(int k=0;k<3;k++)
 {
 for(int i=0;i<3;i++)
 {
	 cout<<a[i][k]->cx<<" "<<a[i][k]->cy<<endl;
 }
 }
  */
//scorecard();
map();
//exit();
//once=1;
//clrscr();
bot2_dp();
bot3_dp();
exit();
once=1;
//map();

//getch();
//clrscr();

//map();
//cout<<"DONE";
//getch();
int i,j;
while(game==1)
{
gotoxy(70,18);
cout<<"Move";
int x=check();
if(x==0)
break;

x=play();
movecount++;
if(x==0)
break;
else
{
x=check();
if(x==0)
break;
map();
x=check();
if(x==0)
break;
}
}
}
//highscores(movecount);
if(run==0)
{
 gotoxy(66,19);
 int high=highscores(1000-movecount);
 if(high==1)
 cout<<"New high score!";
 else
 cout<<"Nothing...";
 //getch();

gotoxy(70,20) ;
cout<<"Game Over";
gotoxy(65,22);
cout<<"Press any key to";
gotoxy(70,23);
cout<<"ExiT";
}
getch();
}

int highscores(int score)      //takes the user score as parameter
{
 int i,j,value,list[5],temp1,temp2;
 FILE*fp;
 fp=fopen("highscor.txt","r");
 for(i=0;i<5;i++)
 {
  fscanf(fp,"%d",&value);        //scanning the file and adding it in list
  list[i]=value;
 }


 for(i=0;i<5;i++)
 {
  if(score>=list[i])
  {
   temp1=score;
   for(j=i;j<5;j++)             //adds the new high score in list
   {
    temp2=list[j];
    list[j]=temp1;
    temp1=temp2;
   }
   fp=fopen("highscor.txt","wt");  //deletes records and opens in write mode
   for(j=0;j<5;j++)
   {
    fprintf(fp,"%d",list[j]);     //prints the edited list in text file
    fprintf(fp," ");
   }

   return 1;       //new high score is added
  }
 }
 return 0;      //no changes
}

