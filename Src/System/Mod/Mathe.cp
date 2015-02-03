MODULE Mathe;

	CONST
		ln10 = 2.3025850929940456840179914546844;
	
	CONST
		LN* = 1;
		MANT* = 2;
		EXP* = 3;
		
	VAR eps, e: REAL;
	VAR go_math*: PROCEDURE (VAR x: ARRAY OF REAL): REAL;
	VAR go: ARRAY 10 OF REAL;
	
	PROCEDURE IntPower* (x: REAL; n: LONGINT): REAL;
		VAR y: REAL;
	BEGIN y := 1.0E+0;
		IF n < 0 THEN x := 1.0E+0 / x; n := - n END;
		WHILE n > 0 DO
			IF ODD(n) THEN y := y * x; DEC(n)
			ELSE x := x * x; n := n DIV 2
			END
		END;
		RETURN y
	END IntPower;

	PROCEDURE Ln* (x: REAL): REAL;
	BEGIN
		go[0]:=LN;
		go[1]:=x;
		RETURN go_math(go)
	END Ln;

	PROCEDURE Log* (x: REAL): REAL;
	BEGIN
		RETURN Ln(x) / ln10
	END Log;
	
	PROCEDURE Mantissa* (x: REAL): REAL;
	BEGIN
		go[0]:=MANT;
		go[1]:=x;
	RETURN go_math(go)
	END Mantissa;

	PROCEDURE Exponent* (x: REAL): INTEGER;
	BEGIN
		go[0]:=EXP;
		go[1]:=x;
	RETURN SHORT(ENTIER(go_math(go)));
	END Exponent;
	
	PROCEDURE Eps* (): REAL;
	BEGIN
		RETURN eps
	END Eps;
	
BEGIN
	eps := 1.0E+0; e := 2.0E+0;
	WHILE e > 1.0E+0 DO eps := eps / 2.0E+0; e := 1.0E+0 + eps END; eps := 2.0E+0 * eps;
END Mathe.