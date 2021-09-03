.global log1pf
.type log1pf,@function
log1pf:
	mov 4(%esp),%eax
	fldln2
	and $0x7fffffff,%eax
	flds 4(%esp)
	cmp $0x3e940000,%eax
	ja 1f
	cmp $0x00800000,%eax
	jb 2f
	fyl2xp1
	ret
1:	fld1
	faddp
	fyl2x
	ret
		# subnormal x, return x with underflow
2:	fnstsw %ax
	and $16,%ax
	jnz 1f
	fxch
	fmul %st(1)
	fstps 4(%esp)
1:	ret
