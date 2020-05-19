int supEq(int x, int y){
	if(x>=y) goto label_supEq;
	return 0;
	label_supEq:
	return 1;
}

int inf(int x, int y){
	if(x<y) goto label_inf;
	return 0;
	label_inf:
	return 1;
}

int infEq(int x, int y){
	if(x<=y) goto label_infEq;
	return 0;
	label_infEq:
	return 1;
}

extern int usr_printd(int usr_i);

int usr_main(){
	int usr_i;
	
	int _t0;
	int _t1;
	
	usr_i = 0 ;

	while0:
	_t0 = inf(usr_i,10);
	if(_t0) goto body0;
	goto continue0;
	body0:
	{
		
		usr_printd(usr_i);

		usr_i = usr_i + 2 ;

	}

	goto while0;
	continue0:

	usr_i = -10 ;

	goto test0;
	for0:
	usr_printd(usr_i);

	usr_i = usr_i + 1 ;

	test0:
	_t0 = infEq(usr_i,10);
	if(_t0) goto for0;

	usr_i = 0 ;

	while1:
	_t0 = -20;
	_t1 = supEq(usr_i,_t0);
	if(_t1) goto body1;
	goto continue1;
	body1:
	{
		
		usr_printd(usr_i);

		usr_i = usr_i - 1 ;

	}

	goto while1;
	continue1:

	return 0;
}

