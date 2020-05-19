int and(int x, int y){
	if(x==0) goto invalide;
	if(y==0) goto invalide;
	return 1;
	invalide:
	return 0;
}

int eq(int x, int y){
	if(x==y) goto label_eq;
	return 0;
	label_eq:
	return 1;
}

int inf(int x, int y){
	if(x<y) goto label_inf;
	return 0;
	label_inf:
	return 1;
}

extern int usr_printd(int usr_i);

int usr_main(){
	int usr_i;
	int usr_x;
	
	int _t0;
	int _t1;
	int _t2;
	
	usr_i = 0 ;

	goto test0;
	for0:
	{
		
		usr_printd(usr_i);

	}

	usr_i = usr_i + 1 ;

	test0:
	_t0 = inf(usr_i,10);
	if(_t0) goto for0;

	_t1 = and(usr_i,5);
	usr_x = and(_t1,6) ;

	_t1 = 9 * 1;
	_t0 = 5 * 4;
	_t0 = _t0 + 9;
	_t0 = _t0 + 7;
	usr_i = _t0 + _t1 ;

	usr_i = 9 ;

	_t0 = 5 * 2;
	_t1 = _t0 - 1;
	_t2 = eq(usr_i,_t1);
	if(_t2) goto if0;
	goto continue0;
	if0:
	{
		
		usr_printd(666);

	}

	continue0:

	return 0;
}

