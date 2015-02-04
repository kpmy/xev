MODULE TestA0;

	IMPORT Log:=Out, Core, Str;
	
	TYPE
		Item = POINTER TO RECORD
			x: INTEGER;
			next: Item;
		END;
	
	VAR
		test: Item;
	
	PROCEDURE (i: Item) FINALIZE;
	BEGIN
		Log.String(".");
	END FINALIZE;
			
	PROCEDURE Do*;
		VAR root, item: Item; i: INTEGER;
	BEGIN 
		i:=0;
		WHILE i<20 DO
			NEW(item);
			item.next:=root;
			root:=item;
			INC(i);
			item.x:=i			
		END;
		item:=root;
		WHILE item#NIL DO
			Log.Int(item.x);
			item:=item.next
		END;
		Log.Ln;
	END Do;
	
	PROCEDURE Do0;
		VAR p: Item; vs: ARRAY 15 OF CHAR;
	BEGIN
		IF p#NIL THEN
			p.next:=NIL (* NIL trap here? *)
		END;
		Str.IntToStringForm(505, Str.hexadecimal, 4, '0', Str.hideBase, vs);
		Log.String("IntToStringForm "); Log.String(vs); Log.Ln;
	END Do0;
	
	PROCEDURE Do1;
		VAR x: BOOLEAN;
		
		PROCEDURE A(): BOOLEAN;
		BEGIN
			RETURN TRUE;
		END A;
		
		PROCEDURE B(): BOOLEAN;
		BEGIN
			RETURN FALSE;
		END B;
		
		PROCEDURE C(): BOOLEAN;
		BEGIN
			HALT(100);
			RETURN FALSE;
		END C;
		
	BEGIN
		x:= A() & B();
		Log.Bool(x); Log.Ln;
		x:=B() & C();
		Log.Bool(x); Log.Ln;
		x:=A() OR C();
		Log.Bool(x); Log.Ln;
	END Do1;
	
	PROCEDURE Do2;
		VAR i: INTEGER;
	BEGIN
		Log.String("cycle start"); Log.Ln;
		i:=0;
		WHILE i<10000 DO INC(i) END;
		Log.String("cycle stop"); Log.Ln;
	END Do2;
	
BEGIN
	NEW(test);
	Log.String("Hello, world!");
	Log.Int(1945);
	Log.Real(0.34423);
	Log.Ln;
	test:=NIL;
	Do;
	Do1;
	Do2;
	Do0;
END TestA0.