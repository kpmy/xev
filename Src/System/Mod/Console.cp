MODULE Console;

	IMPORT
		Str;
		
	VAR
		go_process*: PROCEDURE (IN s: ARRAY OF SHORTCHAR);
		
	PROCEDURE String*(s: ARRAY OF CHAR);
		VAR x: ARRAY 1024 OF CHAR;
	BEGIN
		x:='{"type": "log", "data": "'+s$+'"}';
		go_process(SHORT(x));
	END String;
	
	PROCEDURE Bool*(x: BOOLEAN);
	BEGIN
		IF x THEN String(" $TRUE") ELSE String(" $FALSE") END
	END Bool;
	
	PROCEDURE Int*(i: LONGINT);
		VAR s: ARRAY 32 OF CHAR;
	BEGIN
		Str.IntToString(i, s);
		String(" ");
		String(s)
	END Int;
	
	PROCEDURE Ln*;
	BEGIN
		String("\n")
	END Ln;
	
END Console.

