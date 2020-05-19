extern int usr_printd(int usr_i);

int usr_main(){
	int usr_i;
	int usr_j;
	int usr_k;
	
	int _t0;
	int _t1;
	int _t2;
	int _t3;
	int _t4;
	int _t5;
	int _t6;
	
	usr_i = 45000 ;

	usr_j = -123 ;

	usr_k = 43 ;

	_t4 = usr_j * 2;
	_t5 = usr_k - _t4;
	_t3 = usr_j / usr_k;
	_t1 = usr_j * usr_k;
	_t2 = _t1 * usr_i;
	_t0 = usr_i + usr_j;
	_t0 = _t0 * usr_k;
	_t0 = _t0 / 100;
	_t0 = _t0 + _t2;
	_t0 = _t0 - _t3;
	_t6 = _t0 / _t5;
	usr_printd(_t6);

	return 0;
}

