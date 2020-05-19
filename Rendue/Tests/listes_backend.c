int noEq(int x, int y){
	if(x!=y) goto label_noEq;
	return 0;
	label_noEq:
	return 1;
}

int inf(int x, int y){
	if(x<y) goto label_inf;
	return 0;
	label_inf:
	return 1;
}

extern int usr_printd(int usr_i);

extern void* usr_malloc(int usr_size);

extern void usr_free(void* usr_ptr);

void* usr_affiche(void* usr_p){
	
	int _t0;
	void* _t1;
	
	_t1 = noEq(usr_p,0);
	if(_t1) goto if0;
	goto continue0;
	if0:
	_t0 = *usr_p;
	usr_printd(_t0);

	continue0:

	return usr_p;
}

void* usr_allouer(void* usr_p){
	
	void* _t0;
	
	_t0 = noEq(usr_p,0);
	if(_t0) goto if1;
	goto else0;
	if1:
	{
		
		void* _t0;
		void* _t1;
		
		_t1 = usr_malloc(8);
		_t0 = usr_p + 4;
		*_t0 = _t1 ;
		_t0 = usr_p + 4;
		return *_t0;
	}

	goto continue1;
	else0:
	{
		
		usr_p = usr_malloc(8) ;
		return usr_p;
	}

	continue1:

}

void* usr_desallouer(void* usr_p){
	void* usr_q;
	
	void* _t0;
	
	_t0 = usr_p + 4;
	usr_q = *_t0 ;

	usr_free(usr_p);

	return usr_q;
}

void* usr_parcours(void* usr_l , void* (*usr_f)(void* usr_p)){
	int usr_i;
	void* usr_p;
	void* usr_tete;
	
	int _t0;
	
	usr_p = usr_f(usr_l) ;

	usr_tete = usr_p ;

	usr_i = 0 ;

	goto test0;
	for0:
	{
		
		int _t0;
		void* _t1;
		
		usr_p = usr_f(usr_p) ;

		_t1 = noEq(usr_p,0);
		if(_t1) goto if2;
		goto continue2;
		if2:
		_t0 = *usr_p;
		_t0 = usr_i ;

		continue2:

	}

	usr_i = usr_i + 1 ;

	test0:
	_t0 = inf(usr_i,100);
	if(_t0) goto for0;

	return usr_tete;
}

int usr_main(){
	void* usr_tete;
	void* usr_t;
	
	void* _t0;
	
	_t0 = &usr_allouer;
	usr_tete = usr_parcours(0 , _t0) ;

	usr_t = usr_tete ;

	_t0 = &usr_affiche;
	usr_parcours(usr_t , _t0);

	_t0 = &usr_desallouer;
	usr_parcours(usr_tete , _t0);

	return 0;
}

