MODULE TestA1;
	IMPORT Log:=Out;
	
	PROCEDURE Do;
		VAR a, b, c: SET;
	BEGIN
		a:={1..4, 7, 15..30};
		b:={1};
		c:=a-b;
		Log.Set(a); Log.String("-"); Log.Set(b); Log.Set(c); Log.Ln;

		a:={1..4, 7, 15..30};
		b:={8, 16};
		c:=a+b;
		Log.Set(a); Log.String("+"); Log.Set(b); Log.Set(c); Log.Ln;
		
		a:={1..4, 7, 15..30};
		b:=BITS(3467);
		c:=a*b;
		Log.Set(a); Log.String("*"); Log.Set(b); Log.Set(c); Log.Ln;
		
		a:={1..4, 7, 15..30};
		b:=BITS(3467);
		c:=a/b;
		Log.Set(a); Log.String("/"); Log.Set(b); Log.Set(c); Log.Ln;

	END Do;
	
BEGIN
	Log.String("Test SET's"); Log.Ln;
	Do;
END TestA1.