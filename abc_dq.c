/*== SOURCE file list of “abc_dq.c” with Structure A ==*/
#define S_FUNCTION_LEVEL 2                                                      
#define S_FUNCTION_NAME  abc_dq                                              
#include "simstruc.h"                                                                          
#include <math.h>                                                                                
                                                                                                                
#define U(element) (*uPtrs[element])  /*Pointer to Input Port0*/  
                                                                                                               
static void mdlInitializeSizes(SimStruct *S){                                     
  if (!ssSetNumInputPorts(S, 1)) return;                                             
  ssSetInputPortWidth(S, 0, 4);                                                          
  ssSetInputPortDirectFeedThrough(S, 0, 1);                                   
  ssSetInputPortOverWritable(S, 0, 1);                                             
  if (!ssSetNumOutputPorts(S, 1)) return;                                         
  ssSetOutputPortWidth(S, 0, 2);                                                       
  ssSetNumSampleTimes(S, 1);                                                          
                                                                                                              
  ssSetOptions(S, SS_OPTION_EXCEPTION_FREE_CODE); } 
                                                                                                              
static void mdlInitializeSampleTimes(SimStruct *S) {                   
  ssSetSampleTime(S, 0, CONTINUOUS_SAMPLE_TIME);       
  ssSetOffsetTime(S, 0, 0.0); }   

static void mdlOutputs(SimStruct *S, int_T tid) {                         
  real_T *Y = ssGetOutputPortRealSignal(S,0);                            
InputRealPtrsType uPtrs = ssGetInputPortRealSignalPtrs(S,0); 
                                                                                                                                                                                                         
  real_T t = ssGetT(S);                                                                       
  real_T ia,ib,ic,isd,isq,i_alpha,i_beta,teta,K,L;
  ia = U(0);
  ib = U(1);
  ic = U(2);
  teta = U(3);
  L = 0.866025;
  K = 0.816496;
  
  /* current alpha beta */
  i_alpha = K*(ia - 0.5*ib - 0.5*ic);
  i_beta = K*(L*ib - L*ic);
  
 /*current d q*/
 isd = i_alpha*cos(teta) + i_beta*sin(teta);
 isq = -i_alpha*sin(teta) + i_beta*cos(teta);
 
 Y[0] = isd;
 Y[1] = isq;
         
} 
  
                                                                                                                                                            
		                                                             
static void mdlTerminate(SimStruct *S)                                         
{ } /*Keep this function empty since no memory is allocated*/     
                                                                                                             
#ifdef  MATLAB_MEX_FILE                                                         
    /* Is this file being compiled as a MEX-file? */                           
#include "simulink.c"    /* MEX-file interface mechanism */       
#else                                                                                                     
#include "cg_sfun.h"  /*Code generation registration function*/ 
#endif                                                                                                  
                                                                                                             
