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

		Gen:=JSonGenerator, F:=JSonFormatter,

		Out;

	

	TYPE

		Wr = POINTER TO RECORD (Gen.Writer) END; (* дефолтный вывод - в лог *)

		

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

	

BEGIN



END JSonObxNoModel.



(*JediPlain*gvFEZXZDb21tYW5kZXJzLlN0ZFZpZXdEZXNjAPFEZXZDb21tYW5kZXJzLlZpZXdEZXNjAPFWaWV3cy5WaWV3RGVzYwDwU3RvcmVzLlN0b3JlRGVzYwAAAAAAAAAAAAAAAAAGAAAAAAAAFQAA*)YSonObxNoModel.Do