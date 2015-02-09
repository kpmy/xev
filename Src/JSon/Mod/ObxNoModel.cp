MODULE JSonObxNoModel;

(**
	project	= "YSon"
	organization	= ""
	contributors	= ""
	version	= "System/Rsrc/About"
	copyright	= "Kushnir Piotr Michailovich"
	license	= "Docu/BB-License"
	purpose	= "демонстрация работы форматтера  без использования динамических моделей данных"
	changes	= "
	- 20130527, pk, автогенерация заголовка, документирование
	- 20150204, pk, порт для fw
"
	issues	= ""
**)

	IMPORT 
		Gen:=JSonGenerator, F:=JSonFormatter, P:=JSonParser,
		Str, Out;

	TYPE
		Wr = POINTER TO RECORD (Gen.Writer) END; (* дефолтный вывод - в лог *)
		
		Reader = POINTER TO RECORD (P.Reader) 
			base: Str.Dyn;
			pos: INTEGER;
		END;
		
		StdTarget = POINTER TO RECORD (P.Target) 
			(* res, root, this: YSonModels.Value; *)
			name: ARRAY 256 OF CHAR;
		END;
	
	PROCEDURE (t: StdTarget) Result(): ANYPTR;
	BEGIN
		RETURN NIL
	END Result;
	
	PROCEDURE (t: StdTarget) LevelDown (id, type: INTEGER);
	BEGIN
		Out.String("down"); Out.Int(id); Out.Int(type); Out.Ln;
	END LevelDown;
	
	PROCEDURE (t: StdTarget) NextName (IN s: ARRAY OF CHAR);
	BEGIN
		t.name:=s$;
		Out.String("name:"); Out.String(s); Out.Ln;
	END NextName;
	
	PROCEDURE (t: StdTarget) LevelUp;
	BEGIN
		Out.String("up"); Out.Ln;
	END LevelUp;
	
	PROCEDURE (t: StdTarget) ThisValue (IN x: ARRAY OF CHAR; quoted: BOOLEAN);
	BEGIN
		Out.String("value:"); Out.String(x); Out.Bool(quoted); Out.Ln;
	END ThisValue;
		
	PROCEDURE (r: Reader) Read (OUT ch: CHAR): BOOLEAN;
		VAR ok: BOOLEAN;
	BEGIN
		ASSERT(r.Base()#NIL, 20); 
		ok:=~r.Eot();
		IF ok THEN ch:=r.base.Char(r.pos); INC(r.pos);END;
	RETURN ok  
	END Read;
	
	PROCEDURE (r: Reader) SetPos (x: INTEGER);
	BEGIN
		r.pos:=x;
	END SetPos;
	
	PROCEDURE (r: Reader) Pos (): INTEGER;
	BEGIN
	RETURN r.pos
	END Pos;
	
	PROCEDURE (r: Reader) ConnectTo (source: ANYPTR);
	BEGIN
		ASSERT(source#NIL, 20); 
		WITH source: Str.Dyn DO
			r.base:=source; r.pos:=0;
		ELSE HALT(100) END;
	END ConnectTo;
	
	PROCEDURE (r: Reader) Base (): ANYPTR;
	BEGIN
	RETURN r.base
	END Base;
	
	PROCEDURE (r: Reader) Eot(): BOOLEAN;
	BEGIN
	RETURN ~(r.pos<r.base.Len());
	END Eot;
		
	PROCEDURE (w: Wr) Char (c: CHAR);
	BEGIN
		Out.Char(c);
	END Char;

	PROCEDURE (w: Wr) Whitespace (s: ARRAY OF CHAR);
	BEGIN
		CASE s[0] OF
		09X: Out.Tab;
		|0DX, 0AX: Out.Ln;
		|' ': Out.Char(' ');
		ELSE Out.String(s$) END
	END Whitespace;

	(* формирование структуры JSON непосредственно при выполнении комманд форматтера *)
	PROCEDURE Do*;
		TYPE Base = POINTER TO RECORD END;
		VAR b: Base; wr: Wr; f: F.Formatter; res: INTEGER;
	BEGIN
		Out.Ln;
		NEW(wr); 
		NEW(b);
		wr.SetBase(b);
		f:=F.dir.New(wr);
(* демонстрация пошагового результата *)
		f.WriteSym(F.arrBegin, res);	(* [ *)
		f.WriteSym(F.objBegin, res);		(* { *)
		f.WriteName('obj', res);			(* "obj" *)
		f.WriteSym(F.objBegin, res);			(* { *)
		f.WriteSym(F.objEnd, res);			(* } *)
		f.WriteName('arr', res);			(* "arr" *)
		f.WriteSym(F.arrBegin, res);				(* [ *)
		f.WriteValue(F.atomVal, 'true', res);					(* true *)
		f.WriteValue(F.atomVal, 'true', res);					(* true *)
		f.WriteValue(F.atomVal, 'true', res);					(* true *)
		f.WriteSym(F.objBegin, res);					(* { *)
		f.WriteSym(F.objEnd, res);					(* } *)
		f.WriteSym(F.arrBegin, res);					(* [ *)
		f.WriteSym(F.arrEnd, res);					(* ] *)
		f.WriteSym(F.arrEnd, res);				(* ] *)
		f.WriteName('sss', res);			(* "sss" *)
		f.WriteValue(F.atomVal, 'true', res);			(* true *)
		f.WriteName('sss1', res);			(* "sss1" *)
		f.WriteValue(F.stringVal, 'hello, world!', res);			(* "hello, world!" *)
		f.WriteSym(F.objEnd, res);		(* } *)
		f.WriteSym(F.arrEnd, res);	(* ] *)
	END Do;
	
	PROCEDURE Do2*;
		VAR source: Str.Dyn; rd: Reader; p: P.Parser; x: ANYPTR; res: INTEGER; t: StdTarget;
	BEGIN
		source:=Str.NewFrom('{"hello":"world"}');
		NEW(rd); rd.ConnectTo(source);
		NEW(t);
		p:=P.dir.New(rd);
		p.SetTarget(t);
		x:=p.Parse(res);
	END Do2;
	
BEGIN
END JSonObxNoModel.
(*JediPlain*gvFEZXZDb21tYW5kZXJzLlN0ZFZpZXdEZXNjAPFEZXZDb21tYW5kZXJzLlZpZXdEZXNjAPFWaWV3cy5WaWV3RGVzYwDwU3RvcmVzLlN0b3JlRGVzYwAAAAAAAAAAAAAAAAAGAAAAAAAAFQAA*)YSonObxNoModel.Do