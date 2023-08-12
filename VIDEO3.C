/*

   This is the C version of Charlie's video program.

   Dean, 19 August 1991.

   Last update: 20-11-91

*/




/*****************************/
/* Standard system includes. */
/*****************************/
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <string.h>
#include <dos.h>
#include <dir.h>
#include <conio.h>
#include <ctype.h>


/*********************/
/* Define constants. */
/*********************/
#define TLCG 201      /* Top Left Corner graphics char         */
#define TRCG 187      /* Top Right Corner graphics char        */
#define BLCG 200      /* Bottom Left Corner graphics char      */
#define BRCG 188      /* Bottom Right Corner graphics char     */
#define VLG  186      /* Vertical graphic char                 */
#define HLG  205      /* Horizontal graphic char               */
#define SCRWID 80     /* Screen width                          */
#define SCRHGT 24     /* Screen height                         */
#define F1 59         /* ASCII char for function key 1         */
#define UP 72         /* ASCII char for up arrow               */
#define DOWN 80       /*       char for down arrow             */
#define LEFT 75       /*                left arrow             */
#define RIGHT 77      /*              right arrow              */
#define CR 13         /*       char for ENTER key              */
#define ESC 27        /*       char for ESC key                */
#define BS 8          /*       char for BACKSPACE              */
#define HOME 71       /*       char for the HOME key           */
#define END 79        /*       char for the END key            */
#define DEL 83        /*       char for the DEL key            */
#define BLOCK	219		/* Solid block character			   */
#define ARROW 174      /* Symbol to mark CR when defining data */
#define MAX_LINE_LEN 8192
#define RECORD_SIZE 50      /* Size of each disk record        */
#define MOVIE_LEN   40      /* Length of movie name            */

#define TAPE_NUMBER  1		/* Used to determine search mode   */
#define MOVIE_NAME   2
#define COUNTER      3
#define CATEGORY     4

#define SWINDOWH	 20		/* Height of search display window        */
#define SWINDOWW	 76		/* Width of search display window         */

#define TRUE		1
#define FALSE		0
#define OK			1

#define SCREEN_SIZE 4096




/**********************/
/* Global structures. */
/**********************/
struct MENU_OPTIONS
  {
	   char   option[50];
  };


struct MOVIE_STRUCTURE
  {
	   int tape_number;
	   char movie_name[41];
	   int counter;
	   char category[3];
  };


static struct DISK_RECORD
  {
	   char tape_number[4];
	   char movie_name[40];
	   char counter[4];
	   char category[2];
  };


struct NEW_RECORD
  {
	   char tape_number[5];
	   char movie_name[41];
	   char counter[5];
	   char category[3];
  };



/************************/
/* Function prototypes. */
/************************/
char *get_field(int);
char *set_search_prompt(int);
int display_menu(void);
int main_menu(void);
int pull_down(int,int,struct MENU_OPTIONS [],int,char[],int);
int check_record(struct MOVIE_STRUCTURE *, char *, int);
int display_entry(struct MOVIE_STRUCTURE *,int,int,int,int,int);
void append_spaces(char *, int);
void draw_window(int,int,int,int);
void initialise_database(void);
void error(char *);
void new_film(void);
void search(int);
void time_on_tapes(void);
void delete_movie(void);
void change_details(void);
void enter_movie_count(void);
void print_master_list(void);
void movies_2_hire(void);
void quit(void);
void backup(void);
int write_record(struct NEW_RECORD *,long,int);
void strip_zeros(char *);
void add_zeros(char *);
int get_record(struct MOVIE_STRUCTURE *);
void padd(char *, int);
long int find_next_free(void);
char *get_yesno(void);
void save_screen(void);




/********************/
/* Global variables */
/********************/
FILE *DATA_FILE;
struct MENU_OPTIONS categories[14]=
  {
	{"A   Adventure"},
	{"C   Comedy"},
	{"D   Drama"},
	{"DE  Detective"},
	{"DO  Documentary"},
	{"H   Horror"},
	{"M   Martial Arts"},
	{"MS  Mystery"},
	{"MU  Musical"},
	{"MY  Mythology"},
	{"P   Pornographic"},
	{"R   Romance"},
	{"SF  Sci-Fi"},
	{"W   Westerns"}
  };

  int current_x, current_y;
int num_categories=14;
int num_found=0;
int found_deleted_movie=FALSE;
int found_next_free=FALSE;
long int free_movie_pos=0L;
char screen_buffer[SCREEN_SIZE];


/*************************************************************************/
/* Displays a string on the bottom line of the screen, above the border. */
/* General purpose routine used by almost every function.                */
/*************************************************************************/
void display(string)
    char string[];
{
   int len=0;

   /* Redefine active window. */
   window(2,3,SCRWID-1,SCRHGT-1);

   /* Display users string. */
   len=strlen(string);
   gotoxy(SCRWID/2-len/2,SCRHGT-3);
   textcolor(GREEN);
   cprintf("%s",string);
   gotoxy(SCRWID/2+len/2+1,SCRHGT-3);      /* Reposition the cursor to the
                                           end of the users prompt.      */

}



char *get_yesno()
{
  int x=9,y=0;
  char ch=' ';

  while (ch != 'Y' && ch != 'N' && ch != ESC)
	 {
		x=wherex();
		y=wherey();
		gotoxy(x,y);
		printf(" \b");
		ch=toupper(getch());
	 }
  printf("%c",ch);
  return (&ch);
}



/*****************************************/
/* Grab the users attention with a beep. */
/*****************************************/
void beep()
{
   int x,y;

   /* Where's the cursor ? */
   x=wherex();
   y=wherey();

   /* Beep !! */
   gotoxy(x,y);
   printf("\a");

}




/************************************************************/
/* General purpose routine for displaying an error message. */
/************************************************************/
void error(message)
     char message[];
{
   int mlen=0;
   char new_mess[80];            /* New buffer for the error message. */

   /* Get length of error message, then create new error message.    */
   strcpy(new_mess,message);
   mlen=strlen(strcat(new_mess," - Press a key"));

   /* Display error message. */
   window(2,3,SCRWID-1,SCRHGT-1);
   gotoxy(2,SCRHGT-2);
   clreol();
   textcolor(LIGHTGREEN+BLINK);
   gotoxy(SCRWID/2-mlen/2,SCRHGT-3);
   cprintf("%s\a",new_mess);

   /* Wait for input */
   gotoxy(SCRWID/2+mlen/2+1,SCRHGT-3);
   getch();

   /* Clear error message */
   gotoxy(2,SCRHGT-3);
   clreol();
}



/********************************/
/* Draw a window on the screen. */
/********************************/
void draw_window(x,y,x1,y1)
     int x,y,x1,y1;
{
     int nc;

     /* Top line */
     gotoxy(x,y);      cprintf("%c",TLCG);
     for (nc=x+1;nc<x1;nc++){
         gotoxy(nc,y);
         cprintf("%c",HLG);
     }

     /* Right side */
     gotoxy(y1,x1);      cprintf("%c",TRCG);
     for (nc=y+1;nc<y1;nc++){
         gotoxy(x1,nc);
         cprintf("%c",VLG);
     }

     /* Bottom line */
     gotoxy(x1,y1);      cprintf("%c",BRCG);
     for (nc=x1-1;nc>x;nc--){
         gotoxy(nc,y1);
         cprintf("%c",HLG);
     }

     /* Left hand side */
     gotoxy(x,y1);      cprintf("%c",BLCG);
     for (nc=y1-1;nc>y;nc--){
         gotoxy(x,nc);
         cprintf("%c",VLG);
     }
}





/***************************/
/* Pull down menu routine. */
/***************************/
int pull_down(x,y,options,num_opts,func_name,last_option)
    int x,y,num_opts;
    struct MENU_OPTIONS options[];
	char func_name[];
	int last_option;
{
	  char blanks[80];
	  int next_opt,max_len=0,key=0,opt_len=0, nc=0;

	  /* First, get max width of longest option. */
	  for (next_opt=0;next_opt<num_opts;next_opt++)
		  if (strlen(options[next_opt].option) > max_len)
			 max_len=strlen(options[next_opt].option);

	  opt_len=max_len;
	  memset(blanks,' ',opt_len);
	  blanks[opt_len]='\0';

	  /* Make sure that the heading itself isn't wider.              */
	  if (strlen(func_name) > max_len) max_len=strlen(func_name);

	  if ((max_len % 2) != 0) max_len++;  /* Make sure max_len is even. */
	  max_len += 2;                       /* Add 2 for borders.          */
	  if (max_len > 60) max_len=60;          /* Max length for max_len.        */

	  /* Now display window on the screen. */
	  textbackground(BLACK);
	  draw_window(x,y,x+max_len+1,y+num_opts+1);

	  /* Display function name. */
	  textcolor(MAGENTA);
	  textbackground(DARKGRAY);
	  gotoxy(1+x+max_len/2-strlen(func_name)/2,y);
	  cprintf("%s",func_name);
	  textbackground(BLACK);

	  /* Redefine active window & clear new active screen. */
	  window(x+1,y+1,x+max_len,y+num_opts);
	  clrscr();
	  window(x+2,y+1,x+max_len,y+num_opts);

	  /* Display options within window, highlight the last option that
		 the user picked.  */

	  textcolor(GREEN);
	  for (next_opt=0;next_opt<num_opts;next_opt++){
			gotoxy(1,next_opt+1);
			if (next_opt == last_option){
			   textbackground(LIGHTGRAY);
			   textcolor(WHITE);
			   gotoxy(1,1+next_opt);
			   for (nc=1;nc<opt_len+1;nc++)
				   cprintf(" ");
			}
			else {
			   textcolor(GREEN);
			   textbackground(BLACK);
			}

			gotoxy(1,next_opt+1);
			cprintf("%s",options[next_opt].option);
	  }


	  /* Put menu bar on first item, get response. */
	  next_opt=last_option; key=0;
	  while ((key != CR) && (key != ESC)){
		  gotoxy(opt_len,1+next_opt);
		  key=getch();
		  if (key == 0) key=getch();
		  if (key == UP){            /* Move menu bar up 1 entry      */
			 textcolor(GREEN);
			 textbackground(BLACK);
			 gotoxy(1,1+next_opt);
			 cprintf("%s",blanks);
			 gotoxy(1,1+next_opt);
			 cprintf("%s",options[next_opt].option);
			 if (next_opt > 0) next_opt--;
			 else next_opt=num_opts-1;
			 gotoxy(1,1+next_opt);
			 textcolor(WHITE);
			 textbackground(LIGHTGRAY);
			 cprintf("%s",blanks);
			 gotoxy(1,1+next_opt);
			 cprintf("%s",options[next_opt].option);
		  }
		  if (key == DOWN){             /* Move menu bar down 1 entry      */
			 textcolor(GREEN);
			 textbackground(BLACK);
			 gotoxy(1,1+next_opt);
			 cprintf("%s",blanks);
			 gotoxy(1,1+next_opt);
			 cprintf("%s",options[next_opt].option);
			 if (next_opt < num_opts-1) next_opt++;
			 else next_opt=0;
			 gotoxy(1,1+next_opt);
			 textcolor(WHITE);
			 textbackground(LIGHTGRAY);
			 cprintf("%s",blanks);
			 gotoxy(1,1+next_opt);
			 cprintf("%s",options[next_opt].option);
		  }
	  }

	  /* Reset active window. Return value of key parent function.    */
	  window(2,3,SCRWID-1,SCRHGT-1);
	  textbackground(BLACK);
	  if (key == ESC)
		 return(-1);
	  else
		 return(next_opt);
}






void main()
{
   int finished=0;

   current_x=wherex();
   current_y=wherey();
   gettext(1,1,80,25,screen_buffer);
   initialise_database();

   while (! finished)
	  finished=main_menu();

   quit();

}





int main_menu()
{
   struct MENU_OPTIONS option_list[12]=
	  {
		 {"Enter a new film"},
		 {"Search for a particular film"},
		 {"List the films on a particular tape"},
		 {"List the films under a particular category"},
		 {"Time available on tapes"},
		 {"Delete a movie"},
		 {"Change the details of a movie"},
		 {"Enter movie counter only"},
		 {"Produce a master list of all movies"},
		 {"Enter movies to be hired"},
		 {"Quit VIDEO and return to DOS"},
		 {"Backup all data files"}
	  };

   int num_options=12;
   static int choice=0;
   int result=0;


   textbackground(BLACK);
   window(1,1,80,25);
   clrscr();

   choice=pull_down(20,4,option_list,num_options," VIDEO BASE ",choice);

   switch (choice)
	 {
		case 0 :
		   new_film();
		   break;
		case 1 :
		   search(MOVIE_NAME);
		   break;
		case 2 :
		   search(TAPE_NUMBER);
		   break;
		case 3 :
		   search(CATEGORY);
		   break;
		case 4 :
		   time_on_tapes();
		   break;
		case 5 :
		   delete_movie();
		   break;
		case 6 :
		   change_details();
		   break;
		case 7 :
		   enter_movie_count();
		   break;
		case 8 :
		   print_master_list();
		   break;
		case 9 :
		   movies_2_hire();
		   break;
		case 10 :
		   quit();
		   result=1;
		   break;
		case 11 :
		   backup();
		   break;
		default :
		   result=1;
		   break;
	 }

   return(result);

}




void initialise_database()
{

   if ((DATA_FILE=fopen("vidat.dat","rb+")) == NULL)
	  {
		 error("Can not open data file. Fatal error !!");
		 exit(1);
	  }

}




void new_film()
{
   static int cat=0;
   int finished=0;
   char ch;
   struct NEW_RECORD new_record;

   /* Get the details from the user. */
   while (! finished)
	 {

		/* Define active window and clear the screen. */
		textbackground(BLACK);
		window(1,1,80,25);
		draw_window(1,2,62,10);
		window(3,3,61,9);
		clrscr();

		/* Display the prompts that we'll be asking for input from. */
		textcolor(YELLOW);
		gotoxy(1,1);   cprintf("Movie number   : ");
		gotoxy(1,2);   cprintf("Tape number    ? ");
		gotoxy(1,3);   cprintf("Movie name     ? ");
		gotoxy(1,4);   cprintf("Tape counter   ? ");
		gotoxy(1,5);   cprintf("Movie category ? ");


		/* Collect the input from the user. */
		gotoxy(19,2);	strcpy(new_record.tape_number,get_field(4));
		gotoxy(19,3);	strcpy(new_record.movie_name,get_field(40));
		gotoxy(19,4);	strcpy(new_record.counter,get_field(4));

		/* Display pull-down menu of available movie categories. */
		window(1,1,80,25);
		if ((cat=pull_down(60,1,categories,14," Categories ",cat)) < 0)
		   cat=0;
		strncpy(new_record.category,categories[cat].option,2);
		new_record.category[2]='\0';
		window(3,3,61,9);
		gotoxy(19,5);
		printf("%s",categories[cat].option);


		/* Ask them to verify their answers. */
		gotoxy(10,7);
		textcolor(GREEN);
		cprintf("Is this information correct ? ");
		do
		{
			gotoxy(40,7);
			ch=toupper(getch());
		} while (ch != 'Y' && ch != 'N' && ch != ESC);

		finished=0;
		if (ch == 'Y' || ch == ESC) finished=1;

	 }

	 /* Write the record to disk. */
	 if (ch != ESC)
		write_record(&new_record,0L,FALSE);

}





char *get_field(field_len)
	int field_len;
{
   int x,y,num=0;
   char tmp[80],*t,ch;


   t=tmp;
   x=wherex();
   y=wherey();


   while ((ch=getch()) != '\r' && (num < field_len || ch == '\b'))
	 {
	   if (ch == ESC)
		  return NULL;
	   if (ch == '\b')
		 {
		   if (wherex() > x)
			  {
				gotoxy(wherex()-1,y);
				clreol();
				--t;
				--num;
			  }
		 }
	   else
		 {

		   printf("%c",ch);
		   *t++=ch;
		   gotoxy(x+(++num),y);
		 }
	 }
   *t='\0';

   return(tmp);
}



/* Routine which compares data in the disk record against the desired
   search string. Returns TRUE if a match is found.                   */

int check_record(record,search_string,search_field)
	struct MOVIE_STRUCTURE *record;
	char *search_string;
	int search_field;
{
	int found_entry=FALSE;

	/* First, check to see if the tape number is equal to 0, or if the
	   movie name is deleted. If either case is true, skip record.     */

	if (record->tape_number == 0 ||
		strncmp(record->movie_name,"Deleted",7) == 0)
		return (FALSE);


	/* Otherwise, compare the appropriate field against the search
	   string and look for a match.                                    */

	switch (search_field)
		{
		   case TAPE_NUMBER :
			  if (record->tape_number == atoi(search_string))
				 found_entry=TRUE;
			  break;
		   case MOVIE_NAME  :
			  if (strstr(record->movie_name,search_string) != NULL)
				 found_entry=TRUE;
			  break;
		   case COUNTER     :
			  if (record->counter == atoi(search_string))
				 found_entry=TRUE;
			  break;
		   case CATEGORY    :
			  if (strlen(search_string) < 2)
				 search_string[1]='\0';
			  else
				 search_string[2]='\0';
			  if (strstr(record->category,search_string) != NULL)
				 found_entry=TRUE;
			  break;
		   default :
			  break;
		}

	return (found_entry);
}



int display_entry(record,film_number,x1,y1,x2,y2)
	struct MOVIE_STRUCTURE *record;
	int film_number;
	int x1,y1,x2,y2;
{
   num_found++;
   gotoxy(2,num_found);
   clreol();
   cprintf("%4d  %4d  %7d  %-10s  %-40s\n",record->tape_number,film_number,
		 record->counter,record->category,record->movie_name);
   if (num_found > (y2-y1-2))
	 {
	   num_found=0;
	   gotoxy(20,y2-y1);
	   textcolor(LIGHTGREEN);
	   cprintf("Press any key to continue: ");
	   textcolor(LIGHTCYAN);
	   if (getch() == ESC)
		  return(-1);
	   clrscr();
	 }
   return(OK);
}




char *set_search_prompt(search_field)
	 int search_field;
{
   char *message;

   switch (search_field)
	 {
		case TAPE_NUMBER :
		   message="Tape number to search for  :";
		   break;
		case MOVIE_NAME  :
		   message="Movie name to search for   :";
		   break;
		case COUNTER     :
		   message="Tape counter to search for :";
		   break;
		case CATEGORY    :
		   message="Film category to search for: ";
		   break;
		default          :
		   ;
	 }
   return(message);
}




/* Search - Accepts an integer which tells the function the field that
			we are searching on. This allows us to have only one function
			for all search routines.
*/

void search(search_field)
   int search_field;
{
   int search_pattern_found=0,
	   film_number=0,
	   found_eof=0,
	   found_entry=0;

   int x1,x2,y1,y2;
   int cat=0;

   struct MOVIE_STRUCTURE record_data;

   char search_string[MOVIE_LEN],
		*message;


   /* Set up the initial window and screen details. */
   textbackground(BLACK);
   textcolor(YELLOW);
   window(1,1,80,25);
   clrscr();
   x1=(80-SWINDOWW)/2;
   x2=80-x1;
   y1=(25-SWINDOWH)/2;
   y2=25-y1;
   draw_window(x1,y1,x2,y2);	/* Draw the actual window				   */
   window(x1+1,y1+1,x2-1,y2-1);	/* Redfine active window to within borders */
   gotoxy(2,1);


   /* Determine which field we're going to search on. */
   message=set_search_prompt(search_field);


   /* Display appropriate prompt, depending on field to be seacrhed.     */
   cprintf("%s",message);
   gotoxy(31,1);


   /* Get search string from user & convert to uppercase. If they are
	  searching on CATEGORY, display appropriate menu and get their
	  choice.                                                            */

   if (search_field == CATEGORY)
	  {
		window(1,1,80,25);
		if ((cat=pull_down(50,5,categories,14," Categories ",cat)) < 0)
		   cat=0;
		window(x1+1,y1+3,x2-1,y2-1);
		clrscr();
		strncpy(search_string,strupr(categories[cat].option),2);
		search_string[2]='\0';
		window(x1+1,y1+1,x2-1,y2-1);
		gotoxy(31,1);
		printf("%s",categories[cat].option);
	  }
   else
		strcpy(search_string,strupr(get_field(40)));


   /* Display headings for the result of our searching.	*/
   gotoxy(2,3);
   textcolor(YELLOW);
   cprintf("%-5s %-5s %-8s %-10s  %-40s","TAPE","FILM","COUNTER","CATEGORY",
		   "MOVIE");
   gotoxy(2,4);
   for (found_eof=0;found_eof<SWINDOWW-3;found_eof++)
	   cprintf("~");

   /* Display a message on the top line of the window */
   gotoxy(SWINDOWW-11,1);
   textcolor(LIGHTGREEN+BLINK);
   cprintf("SEARCHING");

   /* Redefine active window to protect our headings */
   y1 += 5;
   window(x1+1,y1,x2-1,y2-1);
   textcolor(LIGHTCYAN);

   /* Reset file pointer to the beginning of the file */
   fseek(DATA_FILE,0L,SEEK_SET);
   found_eof=0;

   /* Read the entire file & look for the desired records */
   num_found=0;
   while (! found_eof)
	 {
		found_eof=get_record(&record_data);
		found_entry=check_record(&record_data,search_string,search_field);
		film_number++;

		/* Display the entry if it matches search criteria */
		if (found_entry)
		   {
			 search_pattern_found=1;
			 if (display_entry(&record_data,film_number,x1,y1,x2,y2) == -1)
				break;
		   }
	 }

   /* Clear status line */
   gotoxy(1,y2-y1);
   clreol();

   if (! search_pattern_found)
	  {
		 gotoxy(2,y2-y1);
		 textcolor(RED);
		 cprintf("Film not found");
	  }

   gotoxy(24,y2-y1);
   textcolor(LIGHTGREEN);
   cprintf("Press any key for menu:");
   getch();

}




void time_on_tapes()
{
}



void delete_movie()
{
   char answer=' ';
   int n=0;
   int abort=FALSE, verified=FALSE;
   int x1,x2,y1,y2;
   long offset=0L, film_number=0L;
   struct NEW_RECORD deleted;
   struct MOVIE_STRUCTURE record;


   /* Set up the initial window and screen details. */
   textbackground(BLACK);
   textcolor(YELLOW);
   window(1,1,80,25);
   clrscr();
   x1=(80-SWINDOWW)/2;
   x2=80-x1;
   y1=(25-SWINDOWH)/2;
   y2=25-y1;
   draw_window(x1,y1,x2,y2);	/* Draw the actual window				   */
   window(x1+1,y1+1,x2-1,y2-1);	/* Redfine active window to within borders */
   gotoxy(2,1);


   /* Create the deleted record entry */
   memset(&deleted.tape_number[0],'\0',54);
   strcpy(deleted.tape_number,"0000");
   strcpy(deleted.counter,"0000");
   strcpy(deleted.movie_name,"Deleted");
   strcpy(deleted.category,"  ");


   /* Display headings */
   gotoxy(2,3);
   textcolor(YELLOW);
   cprintf("%-5s %-5s %-8s %-10s  %-40s","TAPE","FILM","COUNTER","CATEGORY",
		"MOVIE");
   gotoxy(2,4);
   for (n=0;n<SWINDOWW-3;n++)
	   cprintf("~");


   /* Redefine active window to protect our headings */
   y1 += 5;
   window(x1+1,y1,x2-1,y2-1);
   textcolor(LIGHTCYAN);


   /* Ask for number of film to delete. */
   while (! verified && ! abort)
	  {

		/* Re-set number of movies found */
		num_found=0;

		/* Display prompt for movie number */
		gotoxy(2,1);
		clreol();
		cprintf("Enter film NUMBER to delete: ");
		gotoxy(32,1);

		/* Get the film number from the user. */
		film_number=atol(get_field(4));

		/* Calculate offset for the film's data */
		offset=(long) (film_number - 1L) * (long) sizeof(struct DISK_RECORD);

		/* Get the file details & display them */
		fseek(DATA_FILE,(long) offset,SEEK_SET);
		get_record(&record);
		display_entry(&record,3,x1,y1,x2,y2);

		/* Is this the correct film to delete ?? */
		gotoxy(20,10);
		textcolor(LIGHTGREEN);
		cprintf("Correct film to delete ? ");
/*		answer=get_yesno(); */

		/* Check answer */
		switch (answer)
		   {
			  case 'Y' :
				   verified=TRUE;
				   break;
			  default  :
				   abort=TRUE;
				   break;
		   };
	  }


   /* If verified, write the data to the file */
   if (verified)
	  write_record(&deleted,(long) offset,TRUE);

}






void change_details()
{
}


void enter_movie_count()
{
}



void print_master_list()
{
}



void movies_2_hire()
{
}




void save_screen()
{
   gettext(1,1,80,25,screen_buffer);
}



void quit()
{

   window(1,1,80,25);
   clrscr();
   puttext(1,1,80,25,screen_buffer);
   gotoxy(current_x,current_y-1);
   exit (0);

}



void backup()
{
}



void strip_zeros(string)
   char *string;
{
   int nd=0;
   int found_first_digit=0;
   char tmp[10], *t;

   t=tmp;

   /* Remove leading whitespace and zeros */
   while (! found_first_digit)
	  if (*string == '0' || isspace(*string))
		 *string++;
	  else
		 found_first_digit=1;

   /* Copy remainder of string into temporary buffer */
   for (;(*t++ = *string++) != 0 && nd++ < 4;)
	  ;

   /* Terminate string & remove trailing spaces */
   tmp[4]='\0';
   t=&tmp[0];
   for (;*t;)
	   if (isspace(*t))
		  *t='\0';
	   else
		  *t++;

   /* Copy temp buffer back to original string */
   strcpy(string,tmp);

}




/* This is designed to add leading zeros to a four digit character string
   that contains an integer (ie. "  34" will be converted to  "0034").    */

void add_zeros(string)
     char *string;
{
   char tmp[5], *t, *b;
   int num_digits,n,null_pos=0;

   /* Initialise our temporary string. */
   memset(tmp,'0',5);		/* Temp. string containing "0000\0" 		*/
   tmp[4]='\0';

   t=&tmp[0];
   b=&string[0];            /* Remember where the original string starts */

   /* Terminate the original string at the first blank character
      ie. if string ="1   " reduce it to "1"                */
   for(n=4;n>=0;n--)
      if (string[n] == ' ')
         string[n]='\0';

   /* How long is the original string ? */
   num_digits=strlen(string);

   /* Where do we terminate our string of 0's ?? */
   null_pos=4-num_digits;

   /* Insert NULL at appropriate point in string of zeros */
   tmp[null_pos]='\0';

   /* Concatenate our orig. string onto the temp string */
   strcat(tmp,string);

   /* Finally, replace the original string with the temporary string */
   strcpy(b,&tmp[0]);

}



void padd(char *string, int length)
{
   int cur_len=0;
   int n=0;
   int nblanks=0;
   char blanks[50];

   /* Init vars */
   memset(blanks,' ',50);

   /* How long is the current string ?? */
   cur_len=strlen(string);

   /* How many blanks to we need to append ?? */
   nblanks=length - cur_len - 1;

   /* Insert null terminator into appropriate point of temp string. */
   if (nblanks < 50)
	  {
		 blanks[nblanks]='\0';
		 strcat(string,blanks);
	  }
}



long int find_next_free()
{
   long int insert_pos=0L;
   int found_position=FALSE;
   int found_eof=FALSE;
   long num_films=0L;
   struct MOVIE_STRUCTURE next_record;

   /* Reset file to the beginning */
   fseek(DATA_FILE,0L,SEEK_SET);

   /* Keep getting disk records until we find an entry that has a tape
	  number of 0, or a movie name of "Deleted", or until we hit EOF.   */

   while (! feof(DATA_FILE) && ! found_eof && ! found_position)
	  {
		 found_eof=get_record(&next_record);

		 if (! found_eof)
			{
			   if (next_record.tape_number == 0 ||
				  (strcmp(next_record.movie_name,"DELETED") == 0))
				  found_position=TRUE;
			   else
				  num_films++;
			}
	  }


   /* If we found an insert pos, return it to the function, otherwise
	  return the end of file position.                                 */

   insert_pos= (long) num_films * (long) sizeof(struct DISK_RECORD);

   fseek(DATA_FILE,0L,SEEK_SET);

   printf("Inserting film at film #: %ld\n",num_films);
   printf("Beginning of file value : %ld\n",ftell(DATA_FILE));
   printf("Should be inserted at   : %ld\n",insert_pos);

   return((long) insert_pos);

}




int write_record(new_record,offset,DELETING)
   struct NEW_RECORD *new_record;
   long offset;
   int DELETING;
{
   char d_rec[51];
   int write_result=0;
   long insert_pos=0L;

   /* Init vars & reset file pointer to BOF */
   memset(d_rec,' ',51);
   d_rec[0]='\0';
   fseek(DATA_FILE,0L,SEEK_SET);

   /* Padd all strings to the necessary length */
   add_zeros(new_record->tape_number);
   add_zeros(new_record->counter);
   padd(new_record->movie_name,41);
   padd(new_record->category,3);

   /* Concatenate them into one string */
   strcat(d_rec,new_record->tape_number);
   strcat(d_rec,new_record->movie_name);
   strcat(d_rec,new_record->counter);
   strcat(d_rec,new_record->category);

   /* Find a position within the file to insert the new record.
   if (found_deleted_movie || found_next_free)
	   {
		 fseek(DATA_FILE,(long) free_movie_pos,SEEK_SET);
		 found_deleted_movie=FALSE;
		 found_next_free=FALSE;
	   }
   else
	  {  */
		 insert_pos=find_next_free();
		 fseek(DATA_FILE,(long) insert_pos,SEEK_SET);
/*	  }  */

   if (DELETING)
	  {
		fseek(DATA_FILE,0L,SEEK_SET);
		fseek(DATA_FILE,(long) offset,SEEK_SET);
	  }

   printf("Inserting at: %ld\n",ftell(DATA_FILE));
   getch();

   /* Write the data to the file */
   write_result=fwrite(d_rec,sizeof(char),50,DATA_FILE);
   fflush(DATA_FILE);
   fflush(DATA_FILE);

   return(write_result);

}





int get_record(record)
	struct MOVIE_STRUCTURE *record;
{
   struct DISK_RECORD d_rec;
   char tape_number[5];
   char counter[5];

   /* Initialize all variables etc.... */
   memset(record,'\0',sizeof(struct MOVIE_STRUCTURE));
   memset(&d_rec.tape_number[0],'\0',50);

   memset(tape_number,'\0',5);     /* Temp storage for disk rec */
   memset(counter,'\0',5);

   record->tape_number=0;
   record->counter=0;


   /* Physically read the data from the disk file */
   if (feof(DATA_FILE))
	  return(TRUE);
   else
	  if (fread(&d_rec,50,1,DATA_FILE) < 1)
		 return (TRUE);                       /* Hit EOF, so return */


   /* Store integer variables into temporary strings */
   memcpy(tape_number,d_rec.tape_number,4);
   memcpy(counter,d_rec.counter,4);


   /* Convert appropriate string fields to integers */
   record->tape_number=atoi(tape_number);
   record->counter=atoi(counter);


   /* Copy string variables directly into original structure &
	  convert to uppercase.                                           */
   memcpy(record->movie_name,strupr(d_rec.movie_name),40);
   memcpy(record->category,strupr(d_rec.category),2);


   /* Check to see if this movie is deleted, or if we've reached the
	  end of the file. Set flag and position pointer if so. Also, we
	  can now stop searching if the tape_number is 0, as this indicates
	  logical end of data within the file.                             */

   if (strstr(record->movie_name,"Deleted") != NULL)
	   {
		 found_deleted_movie = TRUE;
		 free_movie_pos=ftell(DATA_FILE) - (long) sizeof(struct DISK_RECORD);
	   }

   if (record->tape_number == 0)
	  {
		 free_movie_pos=ftell(DATA_FILE) - (long) sizeof(struct DISK_RECORD);
		 found_next_free = TRUE;
	  }

   /* Obviously haven't hit EOF, so return 0 */
   return(FALSE);

}