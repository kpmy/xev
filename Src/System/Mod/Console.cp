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

	

	PROCEDURE Tab*;

	BEGIN

		String("\t")

	END Tab;

	PROCEDURE Char*(ch: CHAR);
		VAR vs: ARRAY 3 OF CHAR;

	BEGIN
		vs[0]:="\";
		vs[2]:=0X; 
		CASE ch OF

				'"', '\': vs[1]:=ch;

				|09X: vs[1]:='t';

				|0DX: vs[1]:='r';

				|0AX: vs[1]:='n';

				|0CX: vs[1]:='f';

				|08X: vs[1]:='b';

			ELSE 		vs[0]:=ch;

		vs[1]:=0X; END;
		String(vs)
	END Char;

END Console.



