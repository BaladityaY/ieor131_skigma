/******************************************/
/* SIGMA RUN DEFAULTS FOR TRANSLATED MODEL */
/*******************************************/
/*

         MODEL DEFAULTS
         --------------

Model Name:         SkiV7
Model Description:  
Output File:        untitled.out
Output Plot Style:  NOAUTO_FIT
Run Mode:           HI_SPEED
Trace Vars:         Q[0],Q[1]
Random Number Seed: 12345
Initial Values:     10,12,8,6,.5,.5,.5,.5
Ending Condition:   STOP_ON_TIME
Ending Time:        240.000
Trace Events:       RUN,START,RETURN,ENTER
Hide Edges:         

*/
/*******************************************/


#include "sigmafns.h"
#include "sigmalib.h"

/* EVENT FUNCTIONS USED IN THE SIMULATION MODEL */
void   RUN(void);	/*** Initialize simulation ***/
void   SKILIFT(void);	/*** Initialize all ski lifts ***/
void   START(void);	/*** Chairs arrive and begin ascent up slope ***/
void   GETOFF(void);	/*** Customers begin leaving their ski lifts and begin going down ski runs, decide on next ski lift ***/
void   RETURN(void);	/*** Return to a new ski lift ***/
void   ENTER(void);	/*** Customers enter the ski resort and begin at Ski lift i ***/
void   EXIT(void);	/*** Close after end of day ***/

/*   STATE VARIABLE DELARATIONS   */
long   Q[2];	/***  Queue size for ski lifts  ***/
long   B[2];	/***  Batch size for ski lifts  ***/
double TA[2];	/***  New skiier arrival time distribution  ***/
double LAMBDA[2];	/***  Chair interarrival times  ***/
double TS[2];	/***  Ski lift time of service  ***/
double SKI[2][2];	/***  Lift to lift ski time  ***/
double P;	/***  uniform random var from 0 to 1  ***/
long   I;	/***  Ski lift index  ***/
long   J;	/***  Skiier index  ***/
long   PAX;	/***  # of passengers boarding each chair  ***/
long   NEXTRIDE;	/***  Next ski lift taken by skiier  ***/
double PR[2][2];	/***  Probability transition matrix  ***/
long   N;	/***  Number of lifts  ***/
long   CLOSE;	/***  Time when no more skiiers enter system  ***/
long   LEAVE;	/***  Time when skiiers begin leaving resort  ***/

/*   EVENT FUNCTIONS   */
enum
   {
   run_end_event,
   RUN_event,
   SKILIFT_event,
   START_event,
   GETOFF_event,
   RETURN_event,
   ENTER_event,
   EXIT_event,
   };

/*    MAIN PROGRAM     */
int main(int argc, char** argv)
{
  int  next_event;
  char keytoclose = 'p';

  if(!startup_check(0))
    return -1;

  /* Initialize csiglib and simulation */
  while (initialize(argc, (const char * *)argv)) {;

  /* Schedule beginning of simulation */
  event_time = current_time;
  event_type = RUN_event;
  schedule_event();

  /* Schedule end of simulation */
  event_time = stop_time;
  event_type = run_end_event;
  event_priority = 9999;
  schedule_event();

/*  EVENT EXECUTION CONTROL LOOP */
  while (!run_error && !done) {
    /* Pull next event from event list */
    next_event = c_timing();

    /* increment the event count for this event */
    event_count[next_event]++;

    /* Call appropriate event routine */
    switch ( next_event ) {
      case run_end_event:  run_end();
               break;

      case RUN_event:  RUN();
               event_trace("RUN",event_count[next_event]);
               break;

      case SKILIFT_event:  SKILIFT();
               break;

      case START_event:  START();
               event_trace("START",event_count[next_event]);
               break;

      case GETOFF_event:  GETOFF();
               break;

      case RETURN_event:  RETURN();
               event_trace("RETURN",event_count[next_event]);
               break;

      case ENTER_event:  ENTER();
               event_trace("ENTER",event_count[next_event]);
               break;

      case EXIT_event:  EXIT();
               break;

      }
    }
  }
// experiments terminated
printf("Experiments ended! If runs end early: \n\r1. check fields in *.exp file. \n\r2. check if output file was already open. \n\r");
return 0;
}

void
event_trace(const char * name_of_event,const long count)
{
  c_timest(Q[0], 1, 0);
  c_sampst(Q[0], 1, 0);
  c_timest(Q[1], 2, 0);
  c_sampst(Q[1], 2, 0);
  if(trace_flag) fprintf(output_fp, "%9.3f\t %6s\t%6d ",current_time,name_of_event,count);
  if(trace_flag) fprintf(output_fp, "	%7.3g 	%7.3g \n"
,(double)Q[0], (double)Q[1]);
}



int
initialize(int argc, const char** argv)
{
static int first_time = 1;     /* First time in initialize? */
static FILE *input_fp;     /* For reading from the input file */
char *exp_file_name;       /* For constructing input file name */
char y_n = 'p';            /* yes/no for file overwrite*/

       char dir[256];
       char fname[256];
       char ext[256];
       char simulation[1024];
       char experient_name[1024];
        _splitpath( argv[0], NULL, dir, fname, ext );
       strcpy(simulation, fname);
       strcat(simulation, ext);
       strcpy(experient_name, fname);
       strcat(experient_name, ".exp");
     printf("Running the simulation: %s\n", simulation);
    if(strlen(dir) !=0)
       printf("In Path: %s\n",dir);
  if (first_time) {
    exp_file_name = _strdup(argv[0]);
    exp_file_name[strlen(exp_file_name)-1] = 'p';
    printf("\nLooking for experiment file: %s\n",experient_name);
    }

  if ((first_time && (input_fp=fopen(exp_file_name,"r"))!=NULL)
                                           || input_fp!=NULL) {
  if (first_time) {
     first_time = 0; /* Reset for next time into initialize */
     printf("Found. Use [Control]-C to abort replications.\n");
     }

  /* We have run control file of type *.exp          */
  /* Read next set of data from run control file.    */
  if (fscanf(input_fp,"%s %1s %ld %lf %d", output_file_name, &y_n, &rndsd, &stop_time, &trace_flag)<4
     || fscanf(input_fp,"%lf", &SKI[0][0])<1
     || fscanf(input_fp,"%lf", &SKI[0][1])<1
     || fscanf(input_fp,"%lf", &SKI[1][0])<1
     || fscanf(input_fp,"%lf", &SKI[1][1])<1
     || fscanf(input_fp,"%lf", &PR[0][0])<1
     || fscanf(input_fp,"%lf", &PR[0][1])<1
     || fscanf(input_fp,"%lf", &PR[1][0])<1
     || fscanf(input_fp,"%lf", &PR[1][1])<1
     ) {
     /* End of run control file */
     fclose(input_fp);
     return 0;
     }

  if (y_n != 'y' && y_n != 'Y' && y_n != 'n' && y_n != 'N') { 
  fprintf(stderr,"INPUT ERROR: Invalid append file flag in *.exp file: (y=append, n=overwrite old file)\n"); 
  return 0; 
  }

  if (y_n == 'y' || y_n == 'Y') {
     if ((output_fp = fopen(output_file_name,"a"))==NULL) { 
     /* Output file can't be opened. */
     fprintf(stderr,"\nINPUT ERROR: Cannot open output file %s in *.exp file\n",argv[1]);
  return 0; 
  }
  }
if (y_n == 'n' || y_n == 'N') {
     if ((output_fp = fopen(output_file_name,"w"))==NULL) { 
     /* Output file can't be opened. */
     fprintf(stderr,"\nINPUT ERROR: Cannot open output file %s in *.exp file\n",argv[1]);
     return 0;
     }
     }

  if (rndsd < 1 || rndsd > 65534) {
     fprintf(stderr,"\nINPUT ERROR: Random seed %ld is not between 0 and 65534\n",rndsd);
     return 0;
     }

  if (stop_time <= 0.0) {
     fprintf(stderr,"\nINPUT ERROR: Stopping time %lf is negative!\n",stop_time);
     return 0;
     }

  if (trace_flag != 0 && trace_flag != 1) {
     fprintf(stderr,"\nINPUT ERROR: Invalid trace_flag=%d: (1=full trace, 0=summary only)\n",trace_flag);
     return 0;
     }

  done = 0;
     }

 else if (first_time) { /* And open failed, implies data from stdin */
    first_time = 0; /* Reset for next time into initialize */
    printf("Not found, input data at the keyboard.\n");
     /* Give output file name */
     while(y_n != 'y' && y_n != 'Y' && y_n != 'n' && y_n != 'N')
         {
         printf("\nOUTPUT FILE (Enter File Name with Path):\n");
         scanf("%s", output_file_name);
         fflush(stdin);
         sprintf(filename,"%.20s", output_file_name);
         printf("WARNING:File %.20s must not be open!!\n If file does not exist it will be created.\n",filename);
         printf("Do you want the new output appended to this file? (yes/[no])\n");
         scanf("%1s",&y_n);
         fflush(stdin);
         }
     if(y_n == 'y' || y_n == 'Y') output_fp = fopen(filename,"a");
     if(y_n == 'n' || y_n == 'N') output_fp = fopen(filename,"w");

     /* Read in random number seed */
     printf("\n\nRANDOM NUMBER SEED (Enter Integer Between 0 and 65534):\n");
     scanf("%ld", &rndsd);
     fflush(stdin);

     /* Read in run stopping time */
     printf("\nSTOPPING TIME (Enter number of time units until run termination):\n");
     scanf("%lf", &stop_time);
     fflush(stdin);

     /* Read in trace_flag */
     printf("\n\nTRACE (1 = Event Trace, 0 = Summary Only):\n");
     scanf("%d", &trace_flag);
     fflush(stdin);

     /* Parameters for the initial event */;
     printf ( "\nEnter initial value for SKI[0][0]: \n");
     scanf  ( "%lf", &SKI[0][0]);
     printf ( "\nEnter initial value for SKI[0][1]: \n");
     scanf  ( "%lf", &SKI[0][1]);
     printf ( "\nEnter initial value for SKI[1][0]: \n");
     scanf  ( "%lf", &SKI[1][0]);
     printf ( "\nEnter initial value for SKI[1][1]: \n");
     scanf  ( "%lf", &SKI[1][1]);
     printf ( "\nEnter initial value for PR[0][0]: \n");
     scanf  ( "%lf", &PR[0][0]);
     printf ( "\nEnter initial value for PR[0][1]: \n");
     scanf  ( "%lf", &PR[0][1]);
     printf ( "\nEnter initial value for PR[1][0]: \n");
     scanf  ( "%lf", &PR[1][0]);
     printf ( "\nEnter initial value for PR[1][1]: \n");
     scanf  ( "%lf", &PR[1][1]);
   }

 else {
    /* this is not the first time and there is no .exp file */
    return 0;
    }
  
   /* PLACE CUSTOMIZED INITIALIZATIONS HERE */

if (trace_flag)
   {
   fprintf(output_fp,"    Time\t  Event\t Count");
   fprintf(output_fp,"	            Q[0]");
   fprintf(output_fp,"	              Q[1] ");
   fprintf(output_fp,"\n");
   }
  /* Initialize CSIGLIB variables and files */
  c_initlk(rndsd);
  c_initfiles();

  return(1);
}



void
run_end()
{
  printf("\r\nNormal completion after %f time units\n",current_time);
  printf("The Next Seed In the Random Input Stream is %ld\n",rndsd);

  ///  Summary statistics ///
  fprintf(output_fp,"SUMMARY STATISTICS\n");
  printf("SUMMARY STATISTICS\n");
  c_timest(Q[0], 1, 1);
   fprintf(output_fp, "Q[0]:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("Q[0]:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(Q[0], 1, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  c_timest(Q[1], 2, 1);
   fprintf(output_fp, "Q[1]:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("Q[1]:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(Q[1], 2, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  printf("Output written to, %s\n",output_file_name);
  fclose(output_fp);
  c_closedisk();
  done = 1;
}



/****************************/
/*     EVENT FUNCTIONS      */
/****************************/

/*** Initialize simulation ***/
void
RUN()
{
int  _edge_condition[1];

  /* Attribute Value(s) Passed to this Event */

  /* state changes */
  I=0;
  N=2;
  LAMBDA[0]=.1;
  LAMBDA[1]=.1;
  TS[0]=2;
  TS[1]=3;
  B[0]=2;
  B[1]=2;
  TA[0]=((.3333)+(.3333)*RND);
  TA[1]=((.3333)+(.3333)*RND);
  CLOSE=120;
  LEAVE=180;

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( 1==1 );

  /* schedule future events */
  if (_edge_condition[0])
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = I;
    for ( t_index=4; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = SKILIFT_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** Initialize all ski lifts ***/
void
SKILIFT()
{
int  _edge_condition[3];

  /* Attribute Value(s) Passed to this Event */
  I = (long) transfer[3];

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( 1==1 );
  _edge_condition[1] = ( 1==1 );
  _edge_condition[2] = ( I<N-1 );

  /* schedule future events */
  if (_edge_condition[0])
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = I;
    for ( t_index=4; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = START_event;
    event_priority = 3;
    schedule_event();
    }

  if (_edge_condition[1])
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = I;
    for ( t_index=4; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = ENTER_event;
    event_priority = 4;
    schedule_event();
    }

  if (_edge_condition[2])
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = I+1;
    for ( t_index=4; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = SKILIFT_event;
    event_priority = 2;
    schedule_event();
    }

}


/*** Chairs arrive and begin ascent up slope ***/
void
START()
{
int  _edge_condition[2];

  /* Attribute Value(s) Passed to this Event */
  I = (long) transfer[3];

  /* state changes */
  PAX=MIN(Q[I],B[I]);
  Q[I]=Q[I]-PAX;

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( PAX>0 );
  _edge_condition[1] = ( 1==1 );

  /* schedule future events */
  if (_edge_condition[0])
    /***  If there are passengers, they will enter and begin  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = 1;
    transfer[4] = I;
    transfer[5] = PAX;
    for ( t_index=6; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + TS[I];
    event_type = GETOFF_event;
    event_priority = 5;
    schedule_event();
    }

  if (_edge_condition[1])
    /***  Chairs arrive with interarrival times lambda  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = I;
    for ( t_index=4; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + LAMBDA[I];
    event_type = START_event;
    event_priority = 1;
    schedule_event();
    }

}


/*** Customers begin leaving their ski lifts and begin going down ski runs, decide on next ski lift ***/
void
GETOFF()
{
int  _edge_condition[3];

  /* Attribute Value(s) Passed to this Event */
  J = (long) transfer[3];
  I = (long) transfer[4];
  PAX = (long) transfer[5];

  /* state changes */
  P=RND;
  NEXTRIDE=(P>PR[I][0]);

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( CLK<LEAVE );
  _edge_condition[1] = ( CLK>=LEAVE );
  _edge_condition[2] = ( J<PAX );

  /* schedule future events */
  if (_edge_condition[0])
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = NEXTRIDE;
    for ( t_index=4; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + SKI[I][NEXTRIDE];
    event_type = RETURN_event;
    event_priority = 5;
    schedule_event();
    }

  if (_edge_condition[1])
    {
    for ( t_index=3; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = EXIT_event;
    event_priority = 5;
    schedule_event();
    }

  if (_edge_condition[2])
    /***  Assign all passengers to new lifts  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = J+1;
    transfer[4] = I;
    transfer[5] = PAX;
    for ( t_index=6; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = GETOFF_event;
    event_priority = 3;
    schedule_event();
    }

}


/*** Return to a new ski lift ***/
void
RETURN()
{
  /* Attribute Value(s) Passed to this Event */
  I = (long) transfer[3];

  /* state changes */
  Q[I]=Q[I]+1;
}


/*** Customers enter the ski resort and begin at Ski lift i ***/
void
ENTER()
{
int  _edge_condition[1];

  /* Attribute Value(s) Passed to this Event */
  I = (long) transfer[3];

  /* state changes */
  Q[I]=Q[I]+1;

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( CLK<CLOSE );

  /* schedule future events */
  if (_edge_condition[0])
    /***  Enter event  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = I;
    for ( t_index=4; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + TA[I];
    event_type = ENTER_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** Close after end of day ***/
void
EXIT()
{
}

