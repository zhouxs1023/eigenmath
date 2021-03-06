#include "defs.h"

// natural log

void
eval_log(void)
{
	push(cadr(p1));
	eval();
	logarithm();
}

void
logarithm(void)
{
	save();
	log_nib();
	restore();
}

void
log_nib(void)
{
	int h;

	p1 = pop();

	// log(0), log(0.0) unchanged

	if (iszero(p1)) {
		push_symbol(LOG);
		push(p1);
		list(2);
		return;
	}

	if (isdouble(p1)) {
		if (p1->u.d > 0.0)
			push_double(log(p1->u.d));
		else {
			push_double(log(-p1->u.d));
			push(imaginaryunit);
			push_double(M_PI);
			multiply();
			add();
		}
		return;
	}

	// log(z) -> log(mag(z)) + i arg(z)

	if (isdoublez(p1)) {
		push(p1);
		mag();
		logarithm();
		push(imaginaryunit);
		push(p1);
		arg();
		multiply();
		add();
		return;
	}

	// log(1) -> 0

	if (equaln(p1, 1)) {
		push(zero);
		return;
	}

	// log(e) -> 1

	if (p1 == symbol(EXP1)) {
		push(one);
		return;
	}

	if (isnegativenumber(p1)) {
		push(p1);
		negate();
		logarithm();
		push(imaginaryunit);
		push_symbol(PI);
		multiply();
		add();
		return;
	}

	// rational number and not an integer?

	if (isfraction(p1)) {
		push(p1);
		numerator();
		logarithm();
		push(p1);
		denominator();
		logarithm();
		subtract();
		return;
	}

	// log(a ^ b) -> b log(a)

	if (car(p1) == symbol(POWER)) {
		push(caddr(p1));
		push(cadr(p1));
		logarithm();
		multiply();
		return;
	}

	// log(a * b) -> log(a) + log(b)

	if (car(p1) == symbol(MULTIPLY)) {
		h = tos;
		p1 = cdr(p1);
		while (iscons(p1)) {
			push(car(p1));
			logarithm();
			p1 = cdr(p1);
		}
		add_terms(tos - h);
		return;
	}

	push_symbol(LOG);
	push(p1);
	list(2);
}
