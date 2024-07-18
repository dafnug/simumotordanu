/*== SOURCE file list of “dq_abc.c” with Structure A ==*/
#define S_FUNCTION_LEVEL 2                                                      
#define S_FUNCTION_NAME  dq_abc                                             
#include "simstruc.h"                                                                          
#include <math.h>                                                                                
                                                                                                                
#define U(element) (*uPtrs[element])  /*Pointer to Input Port0*/  
                                                                                                               
static void mdlInitializeSizes(SimStruct *S){                                     
  if (!ssSetNumInputPorts(S, 1)) return;                                             
  ssSetInputPortWidth(S, 0, 3);                                                          
  ssSetInputPortDirectFeedThrough(S, 0, 1);                                   
  ssSetInputPortOverWritable(S, 0, 1);                                             
  if (!ssSetNumOutputPorts(S, 1)) return;                                         
  ssSetOutputPortWidth(S, 0, 3);                                                       
  ssSetNumSampleTimes(S, 1);                                                          
                                                                                                              
  ssSetOptions(S, SS_OPTION_EXCEPTION_FREE_CODE); } 
                                                                                                              
static void mdlInitializeSampleTimes(SimStruct *S) {                   
  ssSetSampleTime(S, 0, CONTINUOUS_SAMPLE_TIME);       
  ssSetOffsetTime(S, 0, 0.0); }   

static void mdlOutputs(SimStruct *S, int_T tid) {                         
  real_T *Y = ssGetOutputPortRealSignal(S,0);                            
InputRealPtrsType uPtrs = ssGetInputPortRealSignalPtrs(S,0); 
                                                                                                                                                                                                         
  real_T t = ssGetT(S);                                                                       
  real_T va,vb,vc,vsd,vsq,v_alpha,v_beta,teta,K,L;
  
  vsd = U(0);
  vsq = U(1);
  teta = U(2);
  
  L = 0.866025;
  K = 0.816496;
  
  v_alpha = vsd*cos(teta)-vsq*sin(teta);
  v_beta = vsd*sin(teta)+ vsq*cos(teta);
  
  va = K*v_alpha;
  vb = K*(-0.5*v_alpha + L*v_beta);
  vc = K*(-0.5*v_alpha - L*v_beta);
  
  Y[0]=va;
  Y[1]=vb;
  Y[2]=vc;
           
} 
  
                                                                                                                                                            
		                                                             
static void mdlTerminate(SimStruct *S)                                         
{ } /*Keep this function empty since no memory is allocated*/     
                                                                                                             
#ifdef  MATLAB_MEX_FILE                                                         
    /* Is this file being compiled as a MEX-file? */                           
#include "simulink.c"    /* MEX-file interface mechanism */       
#else                                                                                                     
#include "cg_sfun.h"  /*Code generation registration function*/ 
#endif                                                                                                  
                                                                                                             
