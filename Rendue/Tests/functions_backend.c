int infEq(int x, int y){
	if(x<=y) goto label_infEq;
	return 0;
	label_infEq:
	return 1;
}

extern int usr_printd(int usr_i);

int (*usr_fact)(int usr_n);

int usr_foo(int usr_n){
	
	int _t0;
	int _t1;
	void* _t2;
	int _t3;
	
	_t0 = infEq(usr_n,1);
	if(_t0) goto if0;
	goto continue0;
	if0:
	return 1;
	continue0:

	_t2 = *usr_fact;
	_t1 = usr_n - 1;
	_t3 = _t2(_t1);
	return usr_n * _t3;
}

int usr_main(){
	
	void* _t0;
	int _t1;
	
	usr_fact = &usr_foo ;

	_t0 = *usr_fact;
	_t1 = _t0(10);
	usr_printd(_t1);

	return 0;
}

