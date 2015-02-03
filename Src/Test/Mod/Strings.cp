MODULE TestStrings;
	IMPORT
		Log:=Out;
	
	TYPE
		List = ARRAY 64 OF ARRAY 256 OF CHAR;
		
	VAR 
		l: List;
		count: INTEGER;
	
	PROCEDURE Sort;
		VAR i, j: INTEGER; buf: ARRAY 256 OF CHAR;
	BEGIN
		i:=0;
		WHILE i<count DO
			j:=0;
			WHILE j<count-i-1 DO
				IF l[j]>l[j+1] THEN
					buf:=l[j]$;
					l[j]:=l[j+1];
					l[j+1]:=buf$;
				END;
				INC(j);
			END;
			INC(i);
		END; 
	END Sort;
	
	PROCEDURE Dump;
		VAR i: INTEGER;
	BEGIN
		Log.String("TEST dump"); Log.Ln;
		i:=0;
		WHILE i<count DO Log.String(l[i]); Log.Ln; INC(i) END;
	END Dump;
	
	PROCEDURE Do;
		VAR s: ARRAY 256 OF CHAR; buf: ARRAY 50 OF CHAR; i, j, len, x: INTEGER;
	BEGIN
		s:="Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nunc dignissim vitae eros nec gravida. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus.";
		len:=LEN(s$);
		Log.Int(len); Log.Ln;
		i:=0; j:=0; buf:="";
		WHILE i<len DO
			CASE s[i] OF
			" ", ",", ".": IF buf$#"" THEN l[j]:=buf$; Log.String(buf$); Log.Ln; buf:=""; INC(j); INC(count); END; |
			ELSE
				x:=LEN(buf$);
				buf[x]:=s[i];
				buf[x+1]:=0X;
			END;
			INC(i);
		END;
	END Do;
	
BEGIN
	Log.String("TEST strings"); Log.Ln;
	count:=0;
	Do;
	Dump;
	Log.String("TEST sort"); Log.Ln;
	Sort;
	Dump;
END TestStrings.