/* Copyright (c) 2000-2008 Zilog, Inc. */
#include <math.h>

float _atan2f_c(float arg1,float arg2) {
	float satan(float);

	if((arg1+arg2)==arg1) {
		if(arg1 >= 0.f) {
			return(((float)M_PI_2));
		} else {
			return(-((float)M_PI_2));
		}
	} else if(arg2 < 0.f) {
		if(arg1 >= 0.f) {
			return(((float)M_PI) - satan(-arg1/arg2));
		} else {
			return(-((float)M_PI) + satan(arg1/arg2));
		}
	} else if(arg1 > 0.f) {
		return(satan(arg1/arg2));
	} else {
		return(-satan(-arg1/arg2));
	}
}

double _atan2_c(double, double) __attribute__((alias("_atan2f_c")));
