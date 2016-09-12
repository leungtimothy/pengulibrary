#ifndef PID_H_
#define PID_H_

#include "main.h"

typedef struct {
	float kp;
	float ki;
	float kd;
	int error;
	int errorLast;
	int errorSum;
	int errorDiff;
	int limitUpper;
	int limitLower;
} PID;

void pidInit(PID pid, float kp, float ki, float kd, int limitUpper, int limitLower);

int pidCalculate(PID pid, int input, int target);

#endif
