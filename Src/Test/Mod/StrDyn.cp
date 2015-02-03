MODULE TestStrDyn;
	IMPORT 
		Out, Str;
	
	PROCEDURE Init;
		VAR i: INTEGER; d: Str.Dyn;
	BEGIN
		d:=Str.New();
		i:=ORD("A");
		WHILE i<=ORD("Z") DO
			Out.Char(CHR(i));
			d.Add(CHR(i));
			INC(i);
		END;
		Out.Ln;
	END Init;
	
BEGIN
	Out.String("TEST dyn strings"); Out.Ln;
	Init
END TestStrDyn.