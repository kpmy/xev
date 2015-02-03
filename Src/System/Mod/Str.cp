MODULE Str;
	(* по идее, Strings должен полностью работать *)
	
	IMPORT
		Mathe;
		
	CONST
		minLongIntRev = "8085774586302733229";	(* reversed string of -MIN(LONGINT) *)
		digitspace* = 08FX;
	
	VAR
		maxExp: INTEGER;
		maxDig: INTEGER;
		factor: REAL;	(* 10^maxDig *)	
	
	(* integer conversions *)

	PROCEDURE IntToString* (x: LONGINT; OUT s: ARRAY OF CHAR);
		VAR j, k: INTEGER; ch: CHAR; a: ARRAY 32 OF CHAR;
	BEGIN
		IF x # MIN(LONGINT) THEN
			IF x < 0 THEN s[0] := "-"; k := 1; x := -x ELSE k := 0 END;
			j := 0; REPEAT a[j] := CHR(x MOD 10 + ORD("0")); x := x DIV 10; INC(j) UNTIL x = 0
		ELSE
			a := minLongIntRev; s[0] := "-"; k := 1;
			j := 0; WHILE a[j] # 0X DO INC(j) END
		END;
		ASSERT(k + j < LEN(s), 23);
		REPEAT DEC(j); ch := a[j]; s[k] := ch; INC(k) UNTIL j = 0;
		s[k] := 0X
	END IntToString;
	
		(* real conversions *)

	PROCEDURE RealToStringForm* (x: REAL; precision, minW, expW: INTEGER; fillCh: CHAR;
														OUT s: ARRAY OF CHAR);
		VAR exp, len, i, j, n, k, p: INTEGER; m: ARRAY 80 OF CHAR; neg: BOOLEAN;
	BEGIN
		ASSERT((precision > 0) (*& (precision <= 18)*), 20);
		ASSERT((minW >= 0) & (minW < LEN(s)), 21);
		ASSERT((expW > -LEN(s)) & (expW <= 3), 22);
		exp := Mathe.Exponent(x);
		IF exp = MAX(INTEGER) THEN
			IF fillCh = "0" THEN fillCh := digitspace END;
			x := Mathe.Mantissa(x);
			IF x = -1 THEN m := "-inf"; n := 4
			ELSIF x = 1 THEN m := "inf"; n := 3
			ELSE m := "nan"; n := 3
			END;
			i := 0; j := 0;
			WHILE minW > n DO s[i] := fillCh; INC(i); DEC(minW) END;
			WHILE (j <= n) & (i < LEN(s)) DO s[i] := m[j]; INC(i); INC(j) END
		ELSE
			neg := FALSE; len := 1; m := "00";
			IF x < 0 THEN x := -x; neg := TRUE; DEC(minW) END;
			IF x # 0 THEN
				exp := (exp - 8) * 30103 DIV 100000;	(* * log(2) *)
				IF exp > 0 THEN
					n := SHORT(ENTIER(x / Mathe.IntPower(10, exp)));
					x := x / Mathe.IntPower(10, exp) - n
				ELSIF exp > -maxExp THEN
					n := SHORT(ENTIER(x * Mathe.IntPower(10, -exp)));
					x := x * Mathe.IntPower(10, -exp) - n
				ELSE
					n := SHORT(ENTIER(x * Mathe.IntPower(10, -exp - 2 * maxDig) * factor * factor));
					x := x * Mathe.IntPower(10, -exp - 2 * maxDig) * factor * factor - n
				END;
				(* x0 = (n + x) * 10^exp, 200 < n < 5000 *)
				p := precision - 4;
				IF n < 1000 THEN INC(p) END;
				IF (expW < 0) & (p > exp - expW) THEN p := exp - expW END;
				IF p >= 0 THEN
					x := x + 0.5 / Mathe.IntPower(10, p);	(* rounding correction *)
					IF x >= 1 THEN INC(n); x := x - 1 END
				ELSIF p = -1 THEN INC(n, 5)
				ELSIF p = -2 THEN INC(n, 50)
				ELSIF p = -3 THEN INC(n, 500)
				END;
				i := 0; k := 1000; INC(exp, 3);
				IF n < 1000 THEN k := 100; DEC(exp) END;
				WHILE (i < precision) & ((k > 0) OR (x # 0)) DO
					IF k > 0 THEN p := n DIV k; n := n MOD k; k := k DIV 10
					ELSE x := x * 10; p := SHORT(ENTIER(x)); x := x - p
					END;
					m[i] := CHR(p + ORD("0")); INC(i);
					IF p # 0 THEN len := i END
				END
			END;
			(* x0 = m[0].m[1]...m[len-1] * 10^exp *)
			i := 0;
			IF (expW < 0) OR (expW = 0) & (exp >= -3) & (exp <= len + 1) THEN
				n := exp + 1; k := len - n;
				IF n < 1 THEN n := 1 END;
				IF expW < 0 THEN k := -expW ELSIF k < 1 THEN k := 1 END;
				j := minW - n - k - 1; p := -exp;
				IF neg & (p >= MAX(0, n) + MAX(0, k)) THEN neg := FALSE; INC(j) END
			ELSE
				IF ABS(exp) >= 100 THEN expW := 3
				ELSIF (expW < 2) & (ABS(exp) >= 10) THEN expW := 2
				ELSIF expW < 1 THEN expW := 1
				END;
				IF len < 2 THEN len := 2 END;
				j := minW - len - 3 - expW; k := len;
				IF j > 0 THEN
					k := k + j; j := 0;
					IF k > precision THEN j := k - precision; k := precision END
				END;
				n := 1; DEC(k); p := 0
			END;
			IF neg & (fillCh = "0") THEN s[i] := "-"; INC(i); neg := FALSE END;
			WHILE j > 0 DO s[i] := fillCh; INC(i); DEC(j) END;
			IF neg & (i < LEN(s)) THEN s[i] := "-"; INC(i) END;
			j := 0;
			WHILE (n > 0) & (i < LEN(s)) DO
				IF (p <= 0) & (j < len) THEN s[i] := m[j]; INC(j) ELSE s[i] := "0" END;
				INC(i); DEC(n); DEC(p)
			END;
			IF i < LEN(s) THEN s[i] := "."; INC(i) END;
			WHILE (k > 0) & (i < LEN(s)) DO
				IF (p <= 0) & (j < len) THEN s[i] := m[j]; INC(j) ELSE s[i] := "0" END;
				INC(i); DEC(k); DEC(p)
			END;
			IF expW > 0 THEN
				IF i < LEN(s) THEN s[i] := "E"; INC(i) END;
				IF i < LEN(s) THEN
					IF exp < 0 THEN s[i] := "-"; exp := -exp ELSE s[i] := "+" END;
					INC(i)
				END;
				IF (expW = 3) & (i < LEN(s)) THEN s[i] := CHR(exp DIV 100 + ORD("0")); INC(i) END;
				IF (expW >= 2) & (i < LEN(s)) THEN s[i] := CHR(exp DIV 10 MOD 10 + ORD("0")); INC(i) END;
				IF i < LEN(s) THEN s[i] := CHR(exp MOD 10 + ORD("0")); INC(i) END
			END
		END;
		IF i < LEN(s) THEN s[i] := 0X ELSE HALT(23) END
	END RealToStringForm;

	PROCEDURE RealToString* (x: REAL; OUT s: ARRAY OF CHAR);
	BEGIN
		RealToStringForm(x, 16, 0, 0, digitspace, s)
	END RealToString;
	
BEGIN
	maxExp := SHORT(ENTIER(Mathe.Log(MAX(REAL)))) + 1;	
	maxDig := SHORT(ENTIER(-Mathe.Log(Mathe.Eps())));
	factor := Mathe.IntPower(10, maxDig)
END Str.