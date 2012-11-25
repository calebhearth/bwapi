#ifndef BWAPI_PARAM
#define BWAPI_PARAM(x)
#endif

#ifdef COL_Z
  #ifndef BWAPI_DEF
  #define BWAPI_DEF(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,...) BWAPI_PARAM(z)
  #endif
#endif
#ifdef COL_Y
  #ifndef BWAPI_DEF
  #define BWAPI_DEF(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,...) BWAPI_PARAM(y)
  #endif
#endif
#ifdef COL_X
  #ifndef BWAPI_DEF
  #define BWAPI_DEF(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,...) BWAPI_PARAM(x)
  #endif
#endif
#ifdef COL_W
  #ifndef BWAPI_DEF
  #define BWAPI_DEF(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,...) BWAPI_PARAM(w)
  #endif
#endif
#ifdef COL_V
  #ifndef BWAPI_DEF
  #define BWAPI_DEF(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,...) BWAPI_PARAM(v)
  #endif
#endif
#ifdef COL_U
  #ifndef BWAPI_DEF
  #define BWAPI_DEF(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,...) BWAPI_PARAM(u)
  #endif
#endif
#ifdef COL_T
  #ifndef BWAPI_DEF
  #define BWAPI_DEF(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,...) BWAPI_PARAM(t)
  #endif
#endif
#ifdef COL_S
  #ifndef BWAPI_DEF
  #define BWAPI_DEF(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,...) BWAPI_PARAM(s)
  #endif
#endif
#ifdef COL_R
  #ifndef BWAPI_DEF
  #define BWAPI_DEF(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,...) BWAPI_PARAM(r)
  #endif
#endif
#ifdef COL_Q
  #ifndef BWAPI_DEF
  #define BWAPI_DEF(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,...) BWAPI_PARAM(q)
  #endif
#endif
#ifdef COL_P
  #ifndef BWAPI_DEF
  #define BWAPI_DEF(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,...) BWAPI_PARAM(p)
  #endif
#endif
#ifdef COL_O
  #ifndef BWAPI_DEF
  #define BWAPI_DEF(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,...) BWAPI_PARAM(o)
  #endif
#endif
#ifdef COL_N
  #ifndef BWAPI_DEF
  #define BWAPI_DEF(a,b,c,d,e,f,g,h,i,j,k,l,m,n,...) BWAPI_PARAM(n)
  #endif
#endif
#ifdef COL_M
  #ifndef BWAPI_DEF
  #define BWAPI_DEF(a,b,c,d,e,f,g,h,i,j,k,l,m,...) BWAPI_PARAM(m)
  #endif
#endif
#ifdef COL_L
  #ifndef BWAPI_DEF
  #define BWAPI_DEF(a,b,c,d,e,f,g,h,i,j,k,l,...) BWAPI_PARAM(l)
  #endif
#endif
#ifdef COL_K
  #ifndef BWAPI_DEF
  #define BWAPI_DEF(a,b,c,d,e,f,g,h,i,j,k,...) BWAPI_PARAM(k)
  #endif
#endif
#ifdef COL_J
  #ifndef BWAPI_DEF
  #define BWAPI_DEF(a,b,c,d,e,f,g,h,i,j,...) BWAPI_PARAM(j)
  #endif
#endif
#ifdef COL_I
  #ifndef BWAPI_DEF
  #define BWAPI_DEF(a,b,c,d,e,f,g,h,i,...) BWAPI_PARAM(i)
  #endif
#endif
#ifdef COL_H
  #ifndef BWAPI_DEF
  #define BWAPI_DEF(a,b,c,d,e,f,g,h,...) BWAPI_PARAM(h)
  #endif
#endif
#ifdef COL_G
  #ifndef BWAPI_DEF
  #define BWAPI_DEF(a,b,c,d,e,f,g,...) BWAPI_PARAM(g)
  #endif
#endif
#ifdef COL_F
  #ifndef BWAPI_DEF
  #define BWAPI_DEF(a,b,c,d,e,f,...) BWAPI_PARAM(f)
  #endif
#endif
#ifdef COL_E
  #ifndef BWAPI_DEF
  #define BWAPI_DEF(a,b,c,d,e,...) BWAPI_PARAM(e)
  #endif
#endif
#ifdef COL_D
  #ifndef BWAPI_DEF
  #define BWAPI_DEF(a,b,c,d,...) BWAPI_PARAM(d)
  #endif
#endif
#ifdef COL_C
  #ifndef BWAPI_DEF
  #define BWAPI_DEF(a,b,c,...) BWAPI_PARAM(c)
  #endif
#endif
#ifdef COL_B
  #ifndef BWAPI_DEF
  #define BWAPI_DEF(a,b,...) BWAPI_PARAM(b)
  #endif
#endif
#ifdef COL_A
  #ifndef BWAPI_DEF
  #define BWAPI_DEF(a,...) BWAPI_PARAM(a)
  #endif
#endif


#ifndef BWAPI_DEF
  #define BWAPI_DEF(...)
#endif
