/* == SOURCE file list of “SMC.c” with Structure B == */
#define S_FUNCTION_LEVEL 2                                                      
#define S_FUNCTION_NAME  SMC                                      
#include "simstruc.h"                                                                          
#include <math.h>                                                                                
                                                                                                                
#define U(element) (*uPtrs[element]) /*Pointer to Input Port0*/   
                                                                                                               
static void mdlInitializeSizes(SimStruct *S){                                    
  ssSetNumDiscStates(S, 3);                                                                 
  if (!ssSetNumInputPorts(S, 1)) return;                                           
  ssSetInputPortWidth(S, 0, 6);                                                         
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
  real_T   IqRef;                                                                                    
                                                                                                             
  IqRef = X[2];                                                                                      
  Y[0] = IqRef; }                                                                                   

#define MDL_UPDATE                                                                    
static void mdlUpdate(SimStruct *S, int_T tid) {                            
  real_T    *X = ssGetRealDiscStates(S);                                           
InputRealPtrsType uPtrs = ssGetInputPortRealSignalPtrs(S,0);  
                                                                                                              
  real_T    dt = 1e-3;                                                               
  real_T    alpha,beta,Ki,zeta,iq,wRef,w,c;
  real_T    iqRefPrev,iqRef,x1,x2,wPrev;
  real_T    H,N,psi,J,Tl,sDot,sign,s,iqRefIntPrev,iqRefIntNew,D;
  real_T    miu, fal;
          
  alpha = U(0);
  zeta = U(1);
  miu = U(2);
  c = U(3);
  w = U(4);
  wRef = U(5);
  
  iqRefIntPrev = X[0];
  wPrev = X[1];
  
  N = 4;
  psi = 0.121;
  J = 0.01;
  Tl = 0;
       
  
  x1 = wRef - w;
  x2 = -(w - wPrev)*dt;
  
  s = c*x1 + x2;
  
  if(s > 0){sign = 1;}
  else if (s<=0){sign = -1;}
  
  if(abs(s) > 1){fal = pow(abs(s),1+alpha)*sign;}
  else if(abs(s) < 1){fal = s;}
 
  sDot = -zeta*fal-miu*pow(abs(s),1-alpha)*sign;
  D = (N*psi)/J;
  
  iqRefIntNew = iqRefIntPrev + (-sDot + c*x2)*dt;
  iqRef = (1/D)*iqRefIntNew;
  
  X[0] = iqRefIntNew;
  X[1] = w;
  X[2] = iqRef;
  
                                                                                                                
}                                                     

static void mdlTerminate(SimStruct *S)                                         
{ } /*Keep this function empty since no memory is allocated*/     
                                                                                                             
#ifdef  MATLAB_MEX_FILE                                                         
    /* Is this file being compiled as a MEX-file? */                          
#include "simulink.c"      /*MEX-file interface mechanism*/      
#else                                                                                                    
#include "cg_sfun.h"   /*Code generation registration function*/ 
#endif