/*
 * fixedpoint.h
 *
 *  Created on: 18.08.2011
 *      Author: 129
 */

#ifndef FIXEDPOINT_H_
#define FIXEDPOINT_H_
/* A Simple C program to illustrate the use of Fixed Point Operations */

	/* DEFINE THE MACROS */

	/* The basic operations perfomed on two numbers a and b of fixed

	 point q format returning the answer in q format */

	#define FADD(a,b) ((a)+(b))

	#define FSUB(a,b) ((a)-(b))

	#define FMUL(a,b,q) (((a)*(b))>>(q))

	#define FDIV(a,b,q) (((a)<<(q))/(b))

	/* The basic operations where a is of fixed point q format and b is

	 an integer */

	#define FADDI(a,b,q) ((a)+((b)<<(q)))

	#define FSUBI(a,b,q) ((a)-((b)<<(q)))

	#define FMULI(a,b) ((a)*(b))

	#define FDIVI(a,b) ((a)/(b))

	/* convert a from q1 format to q2 format */

	#define FCONV(a, q1, q2) (((q2)>(q1)) ? (a)<<((q2)-(q1)) : (a)>>((q1)-(q2)))

	/* the general operation between a in q1 format and b in q2 format

	 returning the result in q3 format */

	#define FADDG(a,b,q1,q2,q3) (FCONV(a,q1,q3)+FCONV(b,q2,q3))

	#define FSUBG(a,b,q1,q2,q3) (FCONV(a,q1,q3)-FCONV(b,q2,q3))

	#define FMULG(a,b,q1,q2,q3) FCONV((a)*(b), (q1)+(q2), q3)

	#define FDIVG(a,b,q1,q2,q3) (FCONV(a, q1, (q2)+(q3))/(b))


#endif /* FIXEDPOINT_H_ */
