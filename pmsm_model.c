/* ==SOURCE file list of “pmsm_model.c” with Structure C == */ 
#define S_FUNCTION_LEVEL 2                                                       
#define S_FUNCTION_NAME  pmsm_model                                                    
#include "simstruc.h"                                                                           
#include <math.h>                                                                                
                                                                                                                
#define U(element) (*uPtrs[element])  /*Pointer to Input Port0*/  
                                                                                                                
static void mdlInitializeSizes(SimStruct *S){                                    
ssSetNumContStates(S, 4);                                                                  
if (!ssSetNumInputPorts(S, 1)) return;                                             
ssSetInputPortWidth(S, 0, 4);                                                           
ssSetInputPortDirectFeedThrough(S, 0, 1);                                    
ssSetInputPortOverWritable(S, 0, 1);                                              
if (!ssSetNumOutputPorts(S, 1)) return;                                          
ssSetOutputPortWidth(S, 0, 5);                                                         
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
  
                                                                                                            
  real_T isd,isq,i_alpha,i_beta,ia,ib,ic,teta,omega,L,K;
  
  isd = X[0];
  isq = X[1];
  omega = X[2];
  teta = X[3];
  
  L = 0.866025;
  K = 0.816496;
  
  
  i_alpha = isd*cos(teta)-isq*sin(teta);
  i_beta = isd*sin(teta)+isq*cos(teta);
  
  ia = K*i_alpha;
  ib = K*(-0.5*i_alpha + L*i_beta);
  ic = K*(-0.5*i_alpha - L*i_beta);
  
  Y[0]=ia;
  Y[1]=ib;
  Y[2]=ic;
  Y[3]=omega;
  Y[4]=teta;
  
 }                                                                                     

#define MDL_DERIVATIVES                                                          
static void mdlDerivatives(SimStruct *S) {                                      
  real_T  *dX = ssGetdX(S);                                                               
  real_T  *X = ssGetContStates(S);                                                    
InputRealPtrsType uPtrs = ssGetInputPortRealSignalPtrs(S,0);  

 real_T va,vb,vc,v_alpha,v_beta,vsd,vsq,N,psi,Lsd,Lsq,Rs,J,tl,K,L,isd_dot,isq_dot,teta_dot,omega_dot,Te;
 real_T isd,isq,teta,omega;
 
  L = 0.866025;
  K = 0.816496;
  
  /*state*/
  isd = X[0];
  isq = X[1];
  omega = X[2];
  teta = X[3];
  
 /* PMSM Specification */
 N = 4.0;
 psi = 0.121;
 Lsd = 16.61e-3;
 Lsq = 16.22e-3;
 Rs = 0.55;
 J = 0.01;
 
 va = U(0);
 vb = U(1);
 vc = U(2);
 
 tl = U(3);
 
 /* voltage alpha beta */
 v_alpha = K*(va - 0.5*vb - 0.5*vc);
 v_beta = K*(L*vb - L*vc);
 
 /*voltage d q*/
 vsd = v_alpha*cos(teta) + v_beta*sin(teta);
 vsq = -v_alpha*sin(teta) + v_beta*cos(teta);
 
 
 /*model derrivative isd and isq*/
 isd_dot = (1/Lsd)*(vsd - Rs*isd + N*omega*Lsq*isq); 
 isq_dot = (1/Lsq)*(vsq - Rs*isq - N*omega*Lsd*isd - N*omega*psi);
 
 /*model derrivative omega nad teta*/
 Te = N*(psi + (Lsd - Lsq)*isd)*isq;
 omega_dot = (Te - tl)/J;
 teta_dot = N*omega;
 
 dX[0] = isd_dot;
 dX[1] = isq_dot;
 dX[2] = omega_dot;
 dX[3] = teta_dot;
 
  }                                                                     
                                                                                                              
static void mdlTerminate(SimStruct *S)                                         
{} /*Keep this function empty since no memory is allocated*/    
                                                                                                            
#ifdef  MATLAB_MEX_FILE                                                        
/* Is this file being compiled as a MEX-file? */                             
#include "simulink.c"   /* MEX-file interface mechanism */      
#else                                                                                                   
#include "cg_sfun.h" /*Code generation registration function*/ 
#endif                                                                                                 

