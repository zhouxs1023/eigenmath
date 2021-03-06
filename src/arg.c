#include "defs.h"

void
eval_arg(void)
{
	push(cadr(p1));
	eval();
	arg();
}

// use numerator and denominator to handle (a+i*b)/(c+i*d)

void
arg(void)
{
	save();
	p1 = pop();
	push(p1);
	numerator();
	save();
	arg_nib();
	restore();
	push(p1);
	denominator();
	save();
	arg_nib();
	restore();
	subtract();
	restore();
}

#undef RE
#undef IM

#define RE p2
#define IM p3

void
arg_nib(void)
{
	int h, i, n;

	p1 = pop();

	if (isnegativenumber(p1)) {
		if (isdouble(p1))
			push_double(M_PI);
		else
			push(symbol(PI));
		negate();
		return;
	}

	if (car(p1) == symbol(POWER) && equaln(cadr(p1), -1)) {
		// -1 to a power
		push(symbol(PI));
		push(caddr(p1));
		multiply();
		return;
	}

	if (car(p1) == symbol(POWER) && cadr(p1) == symbol(EXP1)) {
		// exponential
		push(caddr(p1));
		imag();
		return;
	}

	if (car(p1) == symbol(MULTIPLY)) {
		// product of factors
		h = tos;
		p1 = cdr(p1);
		while (iscons(p1)) {
			push(car(p1));
			arg();
			p1 = cdr(p1);
		}
		n = tos - h;
		for (i = 0; i < n; i++)
			if (isdouble(stack[h + i]))
				break;
		add_terms(n);
		if (i < n)
			float_expr();
		return;
	}

	if (car(p1) == symbol(ADD)) {
		// sum of terms
		push(p1);
		rect(); // convert polar and clock forms
		p1 = pop();
		push(p1);
		real();
		RE = pop();
		push(p1);
		imag();
		IM = pop();
		push(IM);
		push(RE);
		arctan();
		return;
	}

	// pure real

	if (isdouble(p1))
		push_double(0.0);
	else
		push(zero);
}
