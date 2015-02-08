MODULE Cons;

	

	IMPORT 

		Log:=Out, Console, Str;

		

	TYPE

		Hook = POINTER TO RECORD (Log.Hook) END;



	PROCEDURE (log: Hook) ClearBuf;

	BEGIN

	

	END ClearBuf;

	

	PROCEDURE (log: Hook) FlushBuf;

	BEGIN

	

	END FlushBuf;

	

	PROCEDURE (log: Hook) Beep;

	BEGIN

	

	END Beep;

	

	PROCEDURE (log: Hook) Char (ch: CHAR);

	BEGIN

		Console.Char(ch)

	END Char;

	

	PROCEDURE (log: Hook) Int (n: LONGINT);

	BEGIN

		Console.Int(n)

	END Int;

	

	PROCEDURE (log: Hook) Real (x: REAL);

		VAR vs: ARRAY 64 OF CHAR;

	BEGIN

		Str.RealToString(x, vs);

		Console.String(" ");

		Console.String(vs);

	END Real;

	

	PROCEDURE (log: Hook) String (IN str: ARRAY OF CHAR);

	BEGIN

		Console.String(str)

	END String;

	

	PROCEDURE (log: Hook) Bool (x: BOOLEAN);

	BEGIN

		Console.Bool(x)

	END Bool;

	

	PROCEDURE (log: Hook) Set (x: SET);

		VAR i: INTEGER; ret: ARRAY 100 OF CHAR; vs: ARRAY 15 OF CHAR;

	BEGIN

		ret := "{"; i := MIN(SET);

		WHILE x # {} DO

			IF i IN x THEN 

				Str.IntToString(i, vs);

				ret:=ret$+vs$; 

				EXCL(x, i);

				IF (i + 2 <= MAX(SET)) & (i+1 IN x) & (i+2 IN x) THEN 

					ret:=ret$+"..";

					x := x - {i+1, i+2}; INC(i, 3);

					WHILE (i <= MAX(SET)) & (i IN x) DO EXCL(x, i); INC(i) END;

					Str.IntToString(i-1, vs);

					ret:=ret$+vs$;

				END;

				IF x # {} THEN ret:=ret+", "; END

			END;

			INC(i);

		END;

		ret:=ret$+"}";

		Console.String(" ");

		Console.String(ret);

	END Set;

	

	PROCEDURE (log: Hook) Tab;

	BEGIN

		Console.Tab;

	END Tab;

	

	PROCEDURE (log: Hook) Ln;

	BEGIN

		Console.Ln;

	END Ln;

	

	PROCEDURE (log: Hook) ParamMsg (IN s, p0, p1, p2: ARRAY OF CHAR);

	BEGIN

	

	END ParamMsg;

	

	PROCEDURE (log: Hook) IntForm (x: INTEGER; base, minWidth: INTEGER; fillCh: CHAR; showBase: BOOLEAN);

	BEGIN

	

	END IntForm;

	

	PROCEDURE (log: Hook) RealForm (x: REAL; precision, minW, expW: INTEGER; fillCh: CHAR);

	BEGIN

	

	END RealForm;

	

	PROCEDURE Init;

		VAR h: Hook;

	BEGIN

		NEW(h);

		Log.SetHook(h);

	END Init;

	

BEGIN

	Init

END Cons.