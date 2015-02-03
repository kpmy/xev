MODULE TestA2;
	IMPORT
		Log:=Out;
	
	PROCEDURE Do;
		VAR a, b: BOOLEAN;
	BEGIN
		a:=TRUE; b:=FALSE;
		Log.Bool(a OR b); Log.Ln;
		Log.Bool(a & b); Log.Ln;
		Log.Bool(a # b); Log.Ln;
		Log.Bool(~a); Log.Ln;
		Log.Bool(~b); Log.Ln;
	END Do;
	
BEGIN
	Log.String("TEST logic"); Log.Ln;
	Do;
END TestA2.