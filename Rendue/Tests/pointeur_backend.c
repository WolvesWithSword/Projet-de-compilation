extern int usr_printd(int usr_i);

extern void* usr_malloc(int usr_n);

int usr_main(){
	void* usr_i;
	void* usr_j;
	
	void* _t0;
	int _t1;
	int _t2;
	int _t3;
	
	_t0 = usr_i + 1;
	*usr_i = _t0 ;
	_t1 = *usr_i;
	_t2 = *usr_j;
	_t3 = _t2 + _t1;
	*usr_j = _t3 ;

	return 0;
}

