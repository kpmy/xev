MODULE JSonFormatter;

	

(**

	project	= "JSon"

	organization	= ""

	contributors	= ""

	version	= "System/Rsrc/About"

	copyright	= "Kushnir Piotr Michailovich"

	license	= "Docu/BB-License"

	purpose	= "генерация json вручную, этот способ полезен тем, кто не использует YSonModels, по каким-то причинам"

	changes	= "

	- 20130112, pk, автогенерация заголовка

	- 20150204, pk, порт для fw

"

	issues	= ""

**)

	

	IMPORT

		JSonGenerator,

		Str, Out;

	

	CONST

		objBegin* = 1;

		objEnd* = 2;

		arrBegin* = 3;

		arrEnd* = 4;

		name = 5;

		value = 6;

		

		stringVal* = 1;

		unicodeStringVal* = 10;

		intVal* = 2;

		realVal* =3;

		atomVal*= 4;

		

		errWrongSym* = -1;

		errUnexpected* = -2;

		

	TYPE

		Formatter* = POINTER TO ABSTRACT RECORD END;

		

		Directory* = POINTER TO ABSTRACT RECORD END;

		

		StdDir = POINTER TO RECORD (Directory) END;

		

		StdF = POINTER TO RECORD (Formatter) 

			wr: JSonGenerator.Writer;

			this: StackItem;

		END;

		

		StackItem = POINTER TO RECORD

			depth: INTEGER;

			expect: SET;

			values: LONGINT;

			next: StackItem;

		END;

		

	VAR 

		dir-, prev-, stdDir-: Directory;

		

	PROCEDURE (d: Directory) New* (wr: JSonGenerator.Writer): Formatter, NEW, ABSTRACT;

	

	PROCEDURE (f: Formatter) ConnectTo- (wr: JSonGenerator.Writer), NEW, ABSTRACT;

	PROCEDURE (f: Formatter) WriteSym* (sym: INTEGER; OUT res: INTEGER), NEW, ABSTRACT;

	PROCEDURE (f: Formatter) WriteName* (IN s: ARRAY OF CHAR; OUT res: INTEGER), NEW, ABSTRACT;

	PROCEDURE (f: Formatter) WriteValue* (type: INTEGER; IN val: ARRAY OF CHAR; OUT res: INTEGER), NEW, ABSTRACT;

	

	PROCEDURE NewStackItem(): StackItem;

		VAR s: StackItem;

	BEGIN

		NEW(s);

		s.depth:=1;

		s.expect:={};

		s.values:=0;

	RETURN s;

	END NewStackItem;

	

	PROCEDURE Push(VAR root: StackItem);

		VAR new: StackItem;

	BEGIN

		new:=NewStackItem();

		new.next:=root;

		root:=new;

		IF root.next#NIL THEN root.depth:=root.next.depth+1 END

	END Push;

	

	PROCEDURE Pop(VAR root: StackItem);

		VAR old: StackItem;

	BEGIN

		IF root#NIL THEN

			old:=root;

			root:=old.next;

			old:=NIL;

		END;

	END Pop;

	

	PROCEDURE (f: StdF) ConnectTo (wr: JSonGenerator.Writer);

	BEGIN

		ASSERT(wr#NIL, 20); ASSERT(wr.Base()#NIL, 21);

		f.wr:=wr;

		f.this:=NIL;

	END ConnectTo;

	

	PROCEDURE String (wr: JSonGenerator.Writer; IN s: ARRAY OF CHAR);

		VAR i: INTEGER;

	BEGIN

		i:=0;

		WHILE i<LEN(s$) DO

			CASE s[i] OF

				'"': wr.Char('\'); wr.Char('"');

				|'\': wr.Char('\'); wr.Char('\');

				|09X: wr.Char('\'); wr.Char('t');

				|0DX: wr.Char('\'); wr.Char('r');

				|0AX: wr.Char('\'); wr.Char('n');

				|0CX: wr.Char('\'); wr.Char('f');

				|08X: wr.Char('\'); wr.Char('b');

			ELSE wr.Char(s[i]) END;

			INC(i)

		END;

	END String;

	

	PROCEDURE UniString (wr: JSonGenerator.Writer; IN s: ARRAY OF CHAR);

		VAR i: INTEGER;

	BEGIN

		i:=0;

		WHILE i<LEN(s$) DO

			CASE s[i] OF

				'"': wr.Char('\'); wr.Char('"');

				|'\': wr.Char('\'); wr.Char('\');

				|09X: wr.Char('\'); wr.Char('t');

				|0DX: wr.Char('\'); wr.Char('r');

				|0AX: wr.Char('\'); wr.Char('n');

				|0CX: wr.Char('\'); wr.Char('f');

				|08X: wr.Char('\'); wr.Char('b');

			ELSE wr.UnicodeChar(s[i]) END;

			INC(i)

		END;

	END UniString;

	

	PROCEDURE (f: StdF) WriteName (IN s: ARRAY OF CHAR; OUT res: INTEGER);

		VAR i: INTEGER;

	BEGIN

		ASSERT(s$#'', 20);

		res:=0;

		IF (f.this#NIL) & (name IN f.this.expect) THEN

			IF (objEnd IN f.this.expect) THEN 

				IF f.this.values > 0 THEN 

					f.wr.Char(','); 

					f.wr.Whitespace(0DX);

				END;

				FOR i:=1 TO f.this.depth DO f.wr.Whitespace(09X) END;

			END;

			f.wr.Char('"'); String(f.wr, s); f.wr.Char('"'); f.wr.Char(':'); f.wr.Whitespace(' '); f.this.expect:={value, arrBegin, objBegin};

			INC(f.this.values);

		ELSE

			res:=errUnexpected

		END;

	END WriteName;

	

	PROCEDURE (f: StdF) WriteValue (type: INTEGER; IN v: ARRAY OF CHAR; OUT res: INTEGER);

		VAR i: INTEGER; vs: ARRAY 20 OF CHAR;

	BEGIN

		ASSERT(type IN {stringVal, atomVal, intVal, realVal, unicodeStringVal}, 20);

		ASSERT((type IN {stringVal, unicodeStringVal}) OR (v$#''), 21); 

		res:=0;

		IF (f.this#NIL) & (value IN f.this.expect) THEN

			IF (arrEnd IN f.this.expect) THEN

				IF f.this.values > 0 THEN 

					f.wr.Char(','); 

					f.wr.Whitespace(0DX);

				END;

				FOR i:=1 TO f.this.depth DO f.wr.Whitespace(09X) END;

			END;

			CASE type OF

				stringVal: f.wr.Char('"'); String(f.wr, v); f.wr.Char('"');

				|unicodeStringVal: f.wr.Char('"'); UniString(f.wr, v); f.wr.Char('"');

				|intVal, realVal: String(f.wr, v);

				|atomVal: Str.ToLower(v, vs); String(f.wr, vs); (* null, true, false точно поместятся в 20 символов *)

			ELSE HALT(100) END;	

			INC(f.this.values);

			IF ~(arrEnd IN f.this.expect) THEN f.this.expect:={objEnd, name} END;

		ELSE

			res:=errUnexpected

		END;

	END WriteValue;

	

	PROCEDURE (f: StdF) WriteSym (sym: INTEGER; OUT res: INTEGER);

		VAR expect: SET; i: INTEGER;

	BEGIN

		res:=0;

		IF f.this#NIL THEN expect:=f.this.expect ELSE expect:={objBegin, arrBegin} END;
		IF (sym IN expect) THEN

			CASE sym OF

				objBegin: 

					IF (f.this#NIL) THEN

						IF ~(arrEnd IN f.this.expect) THEN 

							f.this.expect:={objEnd, name} 

						ELSE

							IF f.this.values > 0 THEN 

								f.wr.Char(','); 

								f.wr.Whitespace(0DX);

							END;

							FOR i:=1 TO f.this.depth DO f.wr.Whitespace(09X) END;

						END;

					END;

					Push(f.this); f.wr.Char('{'); f.wr.Whitespace(0DX); f.this.expect:={objEnd, name}

				|arrBegin: 

					IF (f.this#NIL) THEN

						IF ~(arrEnd IN f.this.expect) THEN f.this.expect:={objEnd, name} 

						ELSE

							IF f.this.values > 0 THEN 

								f.wr.Char(','); 

								f.wr.Whitespace(0DX);

							END;

							FOR i:=1 TO f.this.depth DO f.wr.Whitespace(09X) END;

						END;

					END;

					Push(f.this); f.wr.Char('['); f.wr.Whitespace(0DX); f.this.expect:={arrEnd, objBegin, arrBegin, value}

				|objEnd: 

					f.wr.Whitespace(0DX); FOR i:=1 TO f.this.depth-1 DO f.wr.Whitespace(09X) END;

					f.wr.Char('}'); Pop(f.this);

				|arrEnd: 

					f.wr.Whitespace(0DX); FOR i:=1 TO f.this.depth-1 DO f.wr.Whitespace(09X) END; 

					f.wr.Char(']'); Pop(f.this)

			ELSE HALT(101) END;

		ELSE

			res:=errWrongSym;

			HALT(100);

		END;

	END WriteSym;

	

	PROCEDURE (d: StdDir) New (wr: JSonGenerator.Writer): Formatter;

		VAR f: StdF;

	BEGIN

		ASSERT(wr#NIL, 20); ASSERT(wr.Base()#NIL, 21);

		NEW(f);

		f.ConnectTo(wr);

	RETURN f

	END New;

	

	PROCEDURE Install* (d: Directory);

	BEGIN

		ASSERT(d#NIL, 20);

		prev:=dir;

		dir:=d;

	END Install;

	

	PROCEDURE Init;

		VAR d: StdDir;

	BEGIN

		NEW(d);

		Install(d);

		stdDir:=d;

	END Init;

	

BEGIN

	Init

END JSonFormatter.