int eq(int x, int y){
	if(x==y) goto label_eq;
	return 0;
	label_eq:
	return 1;
}

int noEq(int x, int y){
	if(x!=y) goto label_noEq;
	return 0;
	label_noEq:
	return 1;
}

int sup(int x, int y){
	if(x>y) goto label_sup;
	return 0;
	label_sup:
	return 1;
}

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
	int usr_j;
	
	int _t0;
	int _t1;
	int _t2;
	
	usr_i = 450 ;

	usr_j = -123 ;

	_t0 = usr_j + 0;
	_t1 = usr_i + 1;
	_t0 = inf(_t1,_t0);
	if(_t0) goto if0;
	goto else0;
	if0:
	usr_printd(usr_i);

	goto continue0;
	else0:
	usr_printd(usr_j);

	continue0:

	_t1 = usr_i + 1;
	_t0 = inf(_t1,123);
	if(_t0) goto if1;
	goto else1;
	if1:
	usr_printd(usr_i);

	goto continue1;
	else1:
	usr_printd(usr_j);

	continue1:

	_t1 = usr_i + 1;
	_t0 = inf(_t1,usr_j);
	if(_t0) goto if2;
	goto else2;
	if2:
	usr_printd(usr_i);

	goto continue2;
	else2:
	usr_printd(usr_j);

	continue2:

	_t1 = usr_j + 0;
	_t0 = inf(45,_t1);
	if(_t0) goto if3;
	goto else3;
	if3:
	usr_printd(usr_i);

	goto continue3;
	else3:
	usr_printd(usr_j);

	continue3:

	_t0 = inf(45,123);
	if(_t0) goto if4;
	goto else4;
	if4:
	usr_printd(usr_i);

	goto continue4;
	else4:
	usr_printd(usr_j);

	continue4:

	_t0 = inf(45,usr_j);
	if(_t0) goto if5;
	goto else5;
	if5:
	usr_printd(usr_i);

	goto continue5;
	else5:
	usr_printd(usr_j);

	continue5:

	_t1 = usr_j + 0;
	_t0 = inf(usr_i,_t1);
	if(_t0) goto if6;
	goto else6;
	if6:
	usr_printd(usr_i);

	goto continue6;
	else6:
	usr_printd(usr_j);

	continue6:

	_t0 = inf(usr_i,123);
	if(_t0) goto if7;
	goto else7;
	if7:
	usr_printd(usr_i);

	goto continue7;
	else7:
	usr_printd(usr_j);

	continue7:

	_t0 = inf(usr_i,usr_j);
	if(_t0) goto if8;
	goto else8;
	if8:
	usr_printd(usr_i);

	goto continue8;
	else8:
	usr_printd(usr_j);

	continue8:

	_t1 = usr_j + 0;
	_t2 = usr_i + 1;
	_t0 = sup(_t2,_t1);
	if(_t0) goto if9;
	goto else9;
	if9:
	usr_printd(usr_i);

	goto continue9;
	else9:
	usr_printd(usr_j);

	continue9:

	_t1 = usr_i + 1;
	_t0 = sup(_t1,123);
	if(_t0) goto if10;
	goto else10;
	if10:
	usr_printd(usr_i);

	goto continue10;
	else10:
	usr_printd(usr_j);

	continue10:

	_t1 = usr_i + 1;
	_t0 = sup(_t1,usr_j);
	if(_t0) goto if11;
	goto else11;
	if11:
	usr_printd(usr_i);

	goto continue11;
	else11:
	usr_printd(usr_j);

	continue11:

	_t1 = usr_j + 0;
	_t0 = sup(45,_t1);
	if(_t0) goto if12;
	goto else12;
	if12:
	usr_printd(usr_i);

	goto continue12;
	else12:
	usr_printd(usr_j);

	continue12:

	_t0 = sup(45,123);
	if(_t0) goto if13;
	goto else13;
	if13:
	usr_printd(usr_i);

	goto continue13;
	else13:
	usr_printd(usr_j);

	continue13:

	_t0 = sup(45,usr_j);
	if(_t0) goto if14;
	goto else14;
	if14:
	usr_printd(usr_i);

	goto continue14;
	else14:
	usr_printd(usr_j);

	continue14:

	_t1 = usr_j + 0;
	_t0 = sup(usr_i,_t1);
	if(_t0) goto if15;
	goto else15;
	if15:
	usr_printd(usr_i);

	goto continue15;
	else15:
	usr_printd(usr_j);

	continue15:

	_t0 = sup(usr_i,123);
	if(_t0) goto if16;
	goto else16;
	if16:
	usr_printd(usr_i);

	goto continue16;
	else16:
	usr_printd(usr_j);

	continue16:

	_t0 = sup(usr_i,usr_j);
	if(_t0) goto if17;
	goto else17;
	if17:
	usr_printd(usr_i);

	goto continue17;
	else17:
	usr_printd(usr_j);

	continue17:

	_t1 = usr_j + 0;
	_t2 = usr_i + 1;
	_t0 = supEq(_t2,_t1);
	if(_t0) goto if18;
	goto else18;
	if18:
	usr_printd(usr_i);

	goto continue18;
	else18:
	usr_printd(usr_j);

	continue18:

	_t1 = usr_i + 1;
	_t0 = supEq(_t1,123);
	if(_t0) goto if19;
	goto else19;
	if19:
	usr_printd(usr_i);

	goto continue19;
	else19:
	usr_printd(usr_j);

	continue19:

	_t1 = usr_i + 1;
	_t0 = supEq(_t1,usr_j);
	if(_t0) goto if20;
	goto else20;
	if20:
	usr_printd(usr_i);

	goto continue20;
	else20:
	usr_printd(usr_j);

	continue20:

	_t1 = usr_j + 0;
	_t0 = supEq(45,_t1);
	if(_t0) goto if21;
	goto else21;
	if21:
	usr_printd(usr_i);

	goto continue21;
	else21:
	usr_printd(usr_j);

	continue21:

	_t0 = supEq(45,123);
	if(_t0) goto if22;
	goto else22;
	if22:
	usr_printd(usr_i);

	goto continue22;
	else22:
	usr_printd(usr_j);

	continue22:

	_t0 = supEq(45,usr_j);
	if(_t0) goto if23;
	goto else23;
	if23:
	usr_printd(usr_i);

	goto continue23;
	else23:
	usr_printd(usr_j);

	continue23:

	_t1 = usr_j + 0;
	_t0 = supEq(usr_i,_t1);
	if(_t0) goto if24;
	goto else24;
	if24:
	usr_printd(usr_i);

	goto continue24;
	else24:
	usr_printd(usr_j);

	continue24:

	_t0 = supEq(usr_i,123);
	if(_t0) goto if25;
	goto else25;
	if25:
	usr_printd(usr_i);

	goto continue25;
	else25:
	usr_printd(usr_j);

	continue25:

	_t0 = supEq(usr_i,usr_j);
	if(_t0) goto if26;
	goto else26;
	if26:
	usr_printd(usr_i);

	goto continue26;
	else26:
	usr_printd(usr_j);

	continue26:

	_t1 = usr_j + 0;
	_t2 = usr_i + 1;
	_t0 = infEq(_t2,_t1);
	if(_t0) goto if27;
	goto else27;
	if27:
	usr_printd(usr_i);

	goto continue27;
	else27:
	usr_printd(usr_j);

	continue27:

	_t1 = usr_i + 1;
	_t0 = infEq(_t1,123);
	if(_t0) goto if28;
	goto else28;
	if28:
	usr_printd(usr_i);

	goto continue28;
	else28:
	usr_printd(usr_j);

	continue28:

	_t1 = usr_i + 1;
	_t0 = infEq(_t1,usr_j);
	if(_t0) goto if29;
	goto else29;
	if29:
	usr_printd(usr_i);

	goto continue29;
	else29:
	usr_printd(usr_j);

	continue29:

	_t1 = usr_j + 0;
	_t0 = infEq(45,_t1);
	if(_t0) goto if30;
	goto else30;
	if30:
	usr_printd(usr_i);

	goto continue30;
	else30:
	usr_printd(usr_j);

	continue30:

	_t0 = infEq(45,123);
	if(_t0) goto if31;
	goto else31;
	if31:
	usr_printd(usr_i);

	goto continue31;
	else31:
	usr_printd(usr_j);

	continue31:

	_t0 = infEq(45,usr_j);
	if(_t0) goto if32;
	goto else32;
	if32:
	usr_printd(usr_i);

	goto continue32;
	else32:
	usr_printd(usr_j);

	continue32:

	_t1 = usr_j + 0;
	_t0 = infEq(usr_i,_t1);
	if(_t0) goto if33;
	goto else33;
	if33:
	usr_printd(usr_i);

	goto continue33;
	else33:
	usr_printd(usr_j);

	continue33:

	_t0 = infEq(usr_i,123);
	if(_t0) goto if34;
	goto else34;
	if34:
	usr_printd(usr_i);

	goto continue34;
	else34:
	usr_printd(usr_j);

	continue34:

	_t0 = infEq(usr_i,usr_j);
	if(_t0) goto if35;
	goto else35;
	if35:
	usr_printd(usr_i);

	goto continue35;
	else35:
	usr_printd(usr_j);

	continue35:

	_t1 = usr_j + 0;
	_t2 = usr_i + 1;
	_t0 = eq(_t2,_t1);
	if(_t0) goto if36;
	goto else36;
	if36:
	usr_printd(usr_i);

	goto continue36;
	else36:
	usr_printd(usr_j);

	continue36:

	_t1 = usr_i + 1;
	_t0 = eq(_t1,123);
	if(_t0) goto if37;
	goto else37;
	if37:
	usr_printd(usr_i);

	goto continue37;
	else37:
	usr_printd(usr_j);

	continue37:

	_t1 = usr_i + 1;
	_t0 = eq(_t1,usr_j);
	if(_t0) goto if38;
	goto else38;
	if38:
	usr_printd(usr_i);

	goto continue38;
	else38:
	usr_printd(usr_j);

	continue38:

	_t1 = usr_j + 0;
	_t0 = eq(45,_t1);
	if(_t0) goto if39;
	goto else39;
	if39:
	usr_printd(usr_i);

	goto continue39;
	else39:
	usr_printd(usr_j);

	continue39:

	_t0 = eq(45,123);
	if(_t0) goto if40;
	goto else40;
	if40:
	usr_printd(usr_i);

	goto continue40;
	else40:
	usr_printd(usr_j);

	continue40:

	_t0 = eq(45,usr_j);
	if(_t0) goto if41;
	goto else41;
	if41:
	usr_printd(usr_i);

	goto continue41;
	else41:
	usr_printd(usr_j);

	continue41:

	_t1 = usr_j + 0;
	_t0 = eq(usr_i,_t1);
	if(_t0) goto if42;
	goto else42;
	if42:
	usr_printd(usr_i);

	goto continue42;
	else42:
	usr_printd(usr_j);

	continue42:

	_t0 = eq(usr_i,123);
	if(_t0) goto if43;
	goto else43;
	if43:
	usr_printd(usr_i);

	goto continue43;
	else43:
	usr_printd(usr_j);

	continue43:

	_t0 = eq(usr_i,usr_j);
	if(_t0) goto if44;
	goto else44;
	if44:
	usr_printd(usr_i);

	goto continue44;
	else44:
	usr_printd(usr_j);

	continue44:

	_t1 = usr_j + 0;
	_t2 = usr_i + 1;
	_t0 = noEq(_t2,_t1);
	if(_t0) goto if45;
	goto else45;
	if45:
	usr_printd(usr_i);

	goto continue45;
	else45:
	usr_printd(usr_j);

	continue45:

	_t1 = usr_i + 1;
	_t0 = noEq(_t1,123);
	if(_t0) goto if46;
	goto else46;
	if46:
	usr_printd(usr_i);

	goto continue46;
	else46:
	usr_printd(usr_j);

	continue46:

	_t1 = usr_i + 1;
	_t0 = noEq(_t1,usr_j);
	if(_t0) goto if47;
	goto else47;
	if47:
	usr_printd(usr_i);

	goto continue47;
	else47:
	usr_printd(usr_j);

	continue47:

	_t1 = usr_j + 0;
	_t0 = noEq(45,_t1);
	if(_t0) goto if48;
	goto else48;
	if48:
	usr_printd(usr_i);

	goto continue48;
	else48:
	usr_printd(usr_j);

	continue48:

	_t0 = noEq(45,123);
	if(_t0) goto if49;
	goto else49;
	if49:
	usr_printd(usr_i);

	goto continue49;
	else49:
	usr_printd(usr_j);

	continue49:

	_t0 = noEq(45,usr_j);
	if(_t0) goto if50;
	goto else50;
	if50:
	usr_printd(usr_i);

	goto continue50;
	else50:
	usr_printd(usr_j);

	continue50:

	_t1 = usr_j + 0;
	_t0 = noEq(usr_i,_t1);
	if(_t0) goto if51;
	goto else51;
	if51:
	usr_printd(usr_i);

	goto continue51;
	else51:
	usr_printd(usr_j);

	continue51:

	_t0 = noEq(usr_i,123);
	if(_t0) goto if52;
	goto else52;
	if52:
	usr_printd(usr_i);

	goto continue52;
	else52:
	usr_printd(usr_j);

	continue52:

	_t0 = noEq(usr_i,usr_j);
	if(_t0) goto if53;
	goto else53;
	if53:
	usr_printd(usr_i);

	goto continue53;
	else53:
	usr_printd(usr_j);

	continue53:

	return 0;
}

