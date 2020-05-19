extern int usr_printd(int usr_i);

int usr_main(){
	int usr_i;
	int usr_j;
	
	int _t0;
	int _t1;
	int _t2;
	
	usr_i = 45000 ;

	usr_j = -123 ;

	_t0 = usr_i / usr_j;
	usr_printd(_t0);

	_t0 = 45000 / usr_j;
	usr_printd(_t0);

	_t0 = usr_i / 123;
	usr_printd(_t0);

	_t0 = 45000 / 123;
	usr_printd(_t0);

	_t0 = usr_j + 0;
	_t1 = usr_i / _t0;
	usr_printd(_t1);

	_t0 = usr_i + 0;
	_t1 = _t0 / usr_j;
	usr_printd(_t1);

	_t0 = usr_j + 0;
	_t1 = usr_i + 0;
	_t2 = _t1 / _t0;
	usr_printd(_t2);

	_t0 = usr_i + 0;
	_t1 = _t0 / 123;
	usr_printd(_t1);

	_t0 = usr_j + 0;
	_t1 = 45000 / _t0;
	usr_printd(_t1);

	return 0;
}

