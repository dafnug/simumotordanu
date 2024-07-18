/* ==SOURCE file list of “isd_decoupled.c” with Structure C == */ 
#define S_FUNCTION_LEVEL 2                                                       
#define S_FUNCTION_NAME  isd_decoupled                                                   
#include "simstruc.h"                                                                           
#include <math.h>                                                                                
                                                                                                                
#define U(element) (*uPtrs[element])  /*Pointer to Input Port0*/  
                                                                                                                
static void mdlInitializeSizes(SimStruct *S){                                    
ssSetNumContStates(S, 1);                                                                  
if (!ssSetNumInputPorts(S, 1)) return;                                             
ssSetInputPortWidth(S, 0, 2);                                                           
ssSetInputPortDirectFeedThrough(S, 0, 1);                                    
ssSetInputPortOverWritable(S, 0, 1);                                              
if (!ssSetNumOutputPorts(S, 1)) return;                                          
ssSetOutputPortWidth(S, 0, 1);                                                         
ssSetNumSampleTimes(S, 1);                                                            
                                                                                                              
ssSetOptions(S, SS_OPTION_EXCEPTION_FREE_CODE); }    
                                                                                                              
static void mdlInitializeSampleTimes(SimStruct *S) {                   
ssSetSampleTime(S, 0, CONTINUOUS_SAMPLE_TIME);         
ssSetOffsetTime(S, 0, 0.0); }                                                              
                                                                                                             
#define MDL_INITIALIZE_CONDITIONS                                  
static void mdlInitializeConditions(SimStruct *S) {                      
                                                                                                            
  real_T 	*X0 = ssGetContStates(S);                                       
  int_T 	nStates = ssGetNumContStates(S);                         
  int_T 	i;                                                                                  

/* initialize the states to 0.0 */                                                            
  for (i=0; i < nStates; i++) {X0[i] = 0.0;} }                                       
                                                                                                              
static void mdlOutputs(SimStruct *S, int_T tid) {                          
  real_T            *Y = ssGetOutputPortRealSignal(S,0);                  
  real_T            *X = ssGetContStates(S);                                         
InputRealPtrsType uPtrs = ssGetInputPortRealSignalPtrs(S,0);  
                                                                                                        
  real_T isd, omega, Lsd, N; 
  
  omega = U(1);
  Lsd = 16.61e-3;
  N = 4;
  isd = X[0];
 
  Y[0] = N*omega*Lsd*isd;
 }                                                                                     

#define MDL_DERIVATIVES                                                          
static void mdlDerivatives(SimStruct *S) {                                      
  real_T  *dX = ssGetdX(S);                                                               
  real_T  *X = ssGetContStates(S);                                                    
InputRealPtrsType uPtrs = ssGetInputPortRealSignalPtrs(S,0);  

  real_T Td, isd, isd_ref, isd_dot;
  
  Td = 0.01;
  isd_ref = U(0);
  isd = X[0];
  
  isd_dot = (1/Td)*(isd_ref - isd);
  
  dX[0] = isd_dot;
  }                                                                     
                                                                                                              
static void mdlTerminate(SimStruct *S)                                         
{} /*Keep this function empty since no memory is allocated*/    
                                                                                                            
#ifdef  MATLAB_MEX_FILE                                                        
/* Is this file being compiled as a MEX-file? */                             
#include "simulink.c"   /* MEX-file interface mechanism */      
#else                                                                                                   
#include "cg_sfun.h" /*Code generation registration function*/ 
#endif                                                                                                 

