/* == SOURCE file list of “PI.c” with Structure B == */
#define S_FUNCTION_LEVEL 2                                                      
#define S_FUNCTION_NAME  PI                                       
#include "simstruc.h"                                                                          
#include <math.h>                                                                                
                                                                                                                
#define U(element) (*uPtrs[element]) /*Pointer to Input Port0*/   
                                                                                                               
static void mdlInitializeSizes(SimStruct *S){                                    
  ssSetNumDiscStates(S, 3);                                                                 
  if (!ssSetNumInputPorts(S, 1)) return;                                           
  ssSetInputPortWidth(S, 0, 4);                                                         
  ssSetInputPortDirectFeedThrough(S, 0, 1);                                  
  ssSetInputPortOverWritable(S, 0, 1);                                            
  if (!ssSetNumOutputPorts(S, 1)) return;                                        
  ssSetOutputPortWidth(S, 0, 1);                                                       
  ssSetNumSampleTimes(S, 1);                                                          
                                                                                                              
  ssSetOptions(S, (SS_OPTION_EXCEPTION_FREE_CODE     
          | SS_OPTION_DISCRETE_VALUED_OUTPUT));}          
                                                                                                              
static void mdlInitializeSampleTimes(SimStruct *S){                    
  ssSetSampleTime(S, 0, 1e-3);                                                           
  ssSetOffsetTime(S, 0, 0.0);}                                                              

#define MDL_INITIALIZE_CONDITIONS                                  
static void mdlInitializeConditions(SimStruct *S){                        
  real_T   *X0 = ssGetRealDiscStates(S);                                         
  int_T    nXStates = ssGetNumDiscStates(S);                                 
InputRealPtrsType uPtrs = ssGetInputPortRealSignalPtrs(S,0);  
  int_T i;                                                                                               
                                                                                                              
/* initialize the states to 0.0 */                                                            
  for (i=0; i < nXStates; i++) {                                                            
	X0[i] = 0.0; } }                                                             
                                                                                                              
static void mdlOutputs(SimStruct *S, int_T tid){                           
  real_T   *Y = ssGetOutputPortRealSignal(S,0);                           
  real_T   *X = ssGetRealDiscStates(S);                                           
  real_T   Tref;                                                                                    
                                                                                                             
  Tref = X[1];                                                                                      
  Y[0] = Tref; }                                                                                   

#define MDL_UPDATE                                                                    
static void mdlUpdate(SimStruct *S, int_T tid) {                            
  real_T    *X = ssGetRealDiscStates(S);                                           
InputRealPtrsType uPtrs = ssGetInputPortRealSignalPtrs(S,0);  
                                                                                                              
  real_T    dt = 1e-3;	                                                              
  real_T    Tmax = 1000.0;
  real_T    Tmin = -1000.0;                                                                        
  real_T    Tref, Wref, Wr, xWr_new, xWr_prv, Kspi, Kspp;       
                                                                                                              
Kspp = U(0); Kspi = U(1); Wref = U(2);                                          
Wr = U(3); xWr_prv = X[0];                                                             
                                                                                                              
  xWr_new = xWr_prv + dt * ( Wref - Wr );                                   
  Tref = Kspp * ( Wref - Wr ) + Kspi * xWr_new  ;  /* PI Controller */                                        
  if( Tref >= Tmax ){Tref = Tmax; xWr_new = xWr_prv;}          
  if( Tref <= Tmin ){Tref = Tmin; xWr_new = xWr_prv;}        
                                                                                                             
  X[0] = xWr_new;  X[1] = Tref;}                                                     

static void mdlTerminate(SimStruct *S)                                         
{ } /*Keep this function empty since no memory is allocated*/     
                                                                                                             
#ifdef  MATLAB_MEX_FILE                                                         
    /* Is this file being compiled as a MEX-file? */                          
#include "simulink.c"      /*MEX-file interface mechanism*/      
#else                                                                                                    
#include "cg_sfun.h"   /*Code generation registration function*/ 
#endif                                                                                                  

