MODULE TestMath;
	
	IMPORT
		Log:=Out;
	
	PROCEDURE Do;
		VAR a, b: INTEGER; 
	BEGIN
		a:=5; b:=3;
		Log.Int(a); Log.String(" DIV"); Log.Int(b); Log.Int(a DIV b); Log.Ln; ASSERT((a DIV b) = 1, 60);
		Log.Int(a); Log.String(" MOD"); Log.Int(b); Log.Int(a MOD b); Log.Ln; ASSERT((a MOD b) = 2, 61);
		Log.Ln;
		
		a:=-5; b:=3;
		Log.Int(a); Log.String(" DIV"); Log.Int(b); Log.Int(a DIV b); Log.Ln; ASSERT((a DIV b) = -2, 62);
		Log.Int(a); Log.String(" MOD"); Log.Int(b); Log.Int(a MOD b); Log.Ln; ASSERT((a MOD b) = 1, 63);
		Log.Ln;
		
		a:=5; b:=-3;
		Log.Int(a); Log.String(" DIV"); Log.Int(b); Log.Int(a DIV b); Log.Ln; ASSERT((a DIV b) = -2, 64);
		Log.Int(a); Log.String(" MOD"); Log.Int(b); Log.Int(a MOD b); Log.Ln; ASSERT((a MOD b) = -1, 65);
		Log.Ln;
		
		a:=-5; b:=-3;
		Log.Int(a); Log.String(" DIV"); Log.Int(b); Log.Int(a DIV b); Log.Ln; ASSERT((a DIV b) = 1, 66);
		Log.Int(a); Log.String(" MOD"); Log.Int(b); Log.Int(a MOD b); Log.Ln; ASSERT((a MOD b) = -2, 67);
		
		ASSERT(a = (a DIV b) * b + (a MOD b), 68);
		ASSERT(LONG(a DIV b) = ENTIER(a / b), 69);
	END Do;
	
BEGIN
	Log.String("TEST math"); Log.Ln;
	Do;	
END TestMath.