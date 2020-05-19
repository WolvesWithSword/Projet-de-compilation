int inf(int x, int y){
	if(x<y) goto label_inf;
	return 0;
	label_inf:
	return 1;
}

extern int usr_printd(int usr_i);

int usr_main(){
	int usr_i;
	
	int _t0;
	
	usr_i = 0 ;

	goto test0;
	for0:
	{
		
		usr_printd(usr_i);

	}

	usr_i = usr_i + 1 ;

	test0:
	_t0 = inf(usr_i,1000);
	if(_t0) goto for0;

	return 0;
}

