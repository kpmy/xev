MODULE JSonParser;

(**
	project	= "JSon"
	organization	= ""
	contributors	= ""
	version	= "System/Rsrc/About"
	copyright	= "Kushnir Piotr Michailovich"
	license	= "Docu/BB-License"
	purpose	= "Парсер"
	changes	= "
	- 20130103, pk, автогенерация заголовка
	- 20130103, pk, реализовал парсинг структур пока без вложенности и обработки ошибок
	- 20130109, pk, переделал сканер с учётом экранирования символов и вайтспейсов
	-20130112, pk, поправил баг с неочищаемым буфером
	- 20120117, pk, добавил дополнительный разбор на целые и вещественные числа в StdTarget, добавил пошаговый парсинг для источников, пополняющихся постепенно
	- 20130718, pk, убрал зависимость от ypk
	- 20140109, pk, пофиксил парсинг спецсимволов
	- 20140117, pk, исправил обработку пустых строковых полей
	- 20140301, pk, модифицировал типы переменных в Target, ведь Holder теперь скрыт
	- 20150209, pk, портирование для fw
"
	issues	= ""
**)
	
	IMPORT 
		Out, Str;
	
	CONST
		eot* = 1;
		err* = 2;
		continue* = 0;
		
		none = 0;
		objBegin = 1;
		objEnd = 2;
		arrBegin = 3;
		arrEnd = 4;
		valueSep = 5;
		nameSep = 6;
		
		obj* = 1;
		arr* = 2;
		
	TYPE
		Reader* = POINTER TO ABSTRACT RECORD END;
		
		Parser* = POINTER TO ABSTRACT RECORD END;
		
		Target* = POINTER TO ABSTRACT RECORD END;
		
		Directory* = POINTER TO ABSTRACT RECORD END;
		
		StdDir = POINTER TO RECORD (Directory) END;
		
		StdParser = POINTER TO RECORD (Parser) 
			sc: Scanner;
			root: StackItem;
			t: Target;
		END;
		
		(* StdTarget = POINTER TO RECORD (Target) 
			res, root, this: YSonModels.Value;
			name: ARRAY 256 OF CHAR;
		END; *)
		
		StackItem = POINTER TO RECORD
			pos, type: INTEGER;
			next: StackItem;
		END;
		
		Scanner = RECORD
			rd: Reader;
			res: INTEGER;
			this: RECORD
				inQuotes: BOOLEAN;
				buf: Buffer;
				sym: INTEGER;
			END;
		END;
		
		Buffer = RECORD
			empty: BOOLEAN;
			quoted: BOOLEAN;
			x: Str.Dyn;
		END;
		
		Char = RECORD
			x: CHAR;
			esc: BOOLEAN;
		END;
		
	VAR
		dir-, prev-, stdDir-: Directory;

	PROCEDURE (r: Reader) Read- (OUT ch: CHAR): BOOLEAN, NEW, ABSTRACT;
	PROCEDURE (r: Reader) SetPos- (x: INTEGER), NEW, ABSTRACT;
	PROCEDURE (r: Reader) Pos* (): INTEGER, NEW, ABSTRACT;
	PROCEDURE (r: Reader) ConnectTo* (source: ANYPTR), NEW, ABSTRACT;
	PROCEDURE (r: Reader) Base* (): ANYPTR, NEW, ABSTRACT;
	PROCEDURE (r: Reader) Eot*(): BOOLEAN, NEW, ABSTRACT;
	
	PROCEDURE (d: Directory) New* (rd: Reader): Parser, NEW, ABSTRACT;
	
	PROCEDURE (p: Parser) Parse*(OUT res: INTEGER): ANYPTR, NEW, ABSTRACT;
	PROCEDURE (p: Parser) SetTarget* (t: Target), NEW, EMPTY;
	PROCEDURE (p: Parser) Result*(): ANYPTR, NEW, ABSTRACT;
	PROCEDURE (p: Parser) Begin*, NEW, ABSTRACT;	
	PROCEDURE (p: Parser) Step* (OUT res: INTEGER), NEW, ABSTRACT;
	
	PROCEDURE (t: Target) LevelDown* (id, type: INTEGER), NEW, ABSTRACT;
	PROCEDURE (t: Target) NextName* (IN s: ARRAY OF CHAR), NEW, ABSTRACT;
	PROCEDURE (t: Target) LevelUp*, NEW, ABSTRACT;
	PROCEDURE (t: Target) ThisValue* (IN x: ARRAY OF CHAR; quoted: BOOLEAN), NEW, ABSTRACT;
	PROCEDURE (t: Target) Result- (): ANYPTR, NEW, ABSTRACT;
	
(*	PROCEDURE (t: StdTarget) Result(): ANYPTR;
	BEGIN
		RETURN t.res
	END Result;
	
	PROCEDURE (t: StdTarget) LevelDown (id, type: INTEGER);
		VAR new, root: YSonModels.Value;
	BEGIN
		CASE type OF 
			obj: new:=YSonModels.NewObject();
			|arr: new:=YSonModels.NewArray(0);
		ELSE HALT(100) END;
		ASSERT(new#NIL, 40);
		IF t.res=NIL THEN
			t.res:=new;
			t.root:=NIL;
			t.this:=new;
		ELSE
			root:=t.this;
			WITH root: YSonModels.Array DO
				root.SetLength(root.NofVal()+1);
				root.Set(root.NofVal()-1, new);
			|root: YSonModels.Object DO
				ASSERT(t.name$#'', 41);
				root.Add(t.name$, new);
			ELSE HALT(100) END;
			t.root:=t.this;
			t.this:=new;
		END;
	END LevelDown;
	
	PROCEDURE (t: StdTarget) NextName (IN s: ARRAY OF CHAR);
	BEGIN
		t.name:=s$;
	END NextName;
	
	PROCEDURE (t: StdTarget) LevelUp;
	BEGIN
		t.this:=t.root;
		IF t.this#NIL THEN
			t.root:=t.this.Owner();
		END;
	END LevelUp;
	
	PROCEDURE (t: StdTarget) ThisValue (IN x: ARRAY OF CHAR; quoted: BOOLEAN);
		CONST decimal = '.';
		VAR root: YSonModels.Value; v: YSonModels.Value; r: REAL; res: INTEGER;
	BEGIN
		root:=t.this;
		ASSERT(root#NIL, 20);
		IF quoted THEN
			v:=YSonModels.NewString(x$)
		ELSE
			IF (x$='true') THEN v:=YSonModels.NewLiteral(YSonModels.true)
			ELSIF (x$='false') THEN v:=YSonModels.NewLiteral(YSonModels.false)
			ELSIF (x$='null') THEN v:=YSonModels.NewLiteral(YSonModels.null)
			ELSIF (x$#'') THEN
				Str.StringToReal(x$, r, res);
				IF res=0 THEN
					Str.Find(x$, decimal, 0, res);
					IF res>-1 THEN
						v:=YSonModels.NewNumber(YSonModels.real, r)
					ELSE 
						v:=YSonModels.NewNumber(YSonModels.int, r)
					END;
				ELSE HALT(100) END;
			ELSE HALT(100) END
		END;
		WITH root: YSonModels.Array DO
			root.SetLength(root.NofVal()+1);
			root.Set(root.NofVal()-1, v);
		|root: YSonModels.Object DO
			ASSERT(t.name$#'', 40);
			root.Add(t.name$, v);
		ELSE HALT(100) END;
	END ThisValue;
	*)
	PROCEDURE NewStackItem(pos: INTEGER): StackItem;
		VAR s: StackItem;
	BEGIN
		NEW(s);
		s.pos:=pos;
	RETURN s;
	END NewStackItem;
	
	PROCEDURE Push(root: StackItem; pos, type: INTEGER);
		VAR new: StackItem;
	BEGIN
		new:=NewStackItem(pos); new.type:=type;
		new.next:=root.next;
		root.next:=new;
	END Push;
	
	PROCEDURE Pop(root: StackItem): StackItem;
		VAR old: StackItem;
	BEGIN
		IF root.next#NIL THEN
			old:=root.next;
			root.next:=old.next;
			old.next:=NIL;
		END;
	RETURN old;
	END Pop;
	
	PROCEDURE (VAR sc: Scanner) ConnectTo (rd: Reader), NEW;
	BEGIN
		ASSERT(rd#NIL, 20); ASSERT(rd.Base()#NIL, 21); ASSERT(~rd.Eot(), 22);
		sc.rd:=rd;
	END ConnectTo;
	
	PROCEDURE (VAR sc: Scanner) Init, NEW;
	BEGIN
		sc.res:=continue;
		sc.this.inQuotes:=FALSE;
	END Init;
	
	PROCEDURE (VAR b: Buffer) Empty, NEW;
	BEGIN
		b.empty:=TRUE;
	END Empty;
	
	PROCEDURE (VAR b: Buffer) AddChar(x: CHAR), NEW;
	BEGIN
		b.empty:=FALSE;
		IF b.x=NIL THEN b.x:=Str.NewFrom('') END;
		b.x.Add(x)
	END AddChar;
	
	PROCEDURE (VAR b: Buffer) Clear, NEW;
	BEGIN
		b.empty:=FALSE;
		b.quoted:=FALSE;
		b.x:=NIL;
	END Clear;
	
	PROCEDURE (VAR b: Buffer) Len(): INTEGER, NEW;
		VAR res: INTEGER;
	BEGIN
		res:=0;
		IF b.x#NIL THEN res:=b.x.Len() END;
	RETURN res
	END Len;
	
	PROCEDURE SkipWhite (rd: Reader; OUT res: INTEGER);
		VAR x: CHAR; stop: BOOLEAN; pos: INTEGER;
	BEGIN
		stop:=FALSE;
		pos:=rd.Pos();
		WHILE~stop & rd.Read(x) DO
			CASE x OF
				09X, 0AX, 0DX, ' ': stop:=FALSE;
			ELSE 
				stop:=TRUE;
				rd.SetPos(pos); 
				res:=continue
			END;
			pos:=rd.Pos();
		END;
		IF ~stop THEN res:=eot END;
	END SkipWhite;
	
	PROCEDURE ReadChar (rd: Reader; OUT ch: Char; OUT res: INTEGER);
		VAR x: CHAR; stop: BOOLEAN; u: ARRAY 6 OF CHAR; uc: INTEGER; 
	BEGIN
		res:=continue; stop:=FALSE; ch.esc:=FALSE; uc:=-1;
		WHILE ~stop & rd.Read(x)  DO
			IF (x='\') & ~ch.esc THEN
				ch.esc:=TRUE;
			ELSIF ch.esc & (uc<0) THEN
				CASE x OF
					'\', '"', '/' (*, 08X, 09X, 0DX, 0AX, 0CX *): ch.x:=x; stop:=TRUE;
					|'u': uc:=0; 
					|'b': ch.x:=08X; stop:=TRUE;
					|'f': ch.x:=0CX; stop:=TRUE;
					|'n': ch.x:=0AX; stop:=TRUE;
					|'r': ch.x:=0DX; stop:=TRUE;
					|'t': ch.x:=09X; stop:=TRUE;
				ELSE 
					res:=err; stop:=TRUE;
				END
			ELSIF ch.esc & (uc>=0) THEN 
				IF uc < 4 THEN 
					u[uc]:=x; INC(uc);
				ELSE 
					u[4]:='H'; u[5]:=0X;
					Str.StringToInt(u$, uc, res);
					IF res=0 THEN
						ch.x:=CHR(uc);
					ELSE 
						res:=err;
					END;
					stop:=TRUE;
					uc:=-1;
				END;
			ELSIF (x#'\') & ~ch.esc THEN 
				ch.x:=x;
				stop:=TRUE;
			END;
		END;
		IF ~stop THEN res:=eot END;
	END ReadChar;
	
	PROCEDURE (VAR sc: Scanner) ClearBuffer, NEW;
	BEGIN
		sc.this.buf.Clear;
	END ClearBuffer;
	
	PROCEDURE (VAR sc: Scanner) Read, NEW;	
		VAR c: Char;
	BEGIN
		ASSERT(sc.rd#NIL, 20);
		sc.res:=continue;
		IF ~sc.this.inQuotes THEN SkipWhite(sc.rd, sc.res) END;
		IF sc.res=continue THEN
			sc.this.sym:=none;
			ReadChar(sc.rd, c, sc.res);
			IF ~c.esc THEN
				CASE c.x OF
					'"': sc.this.inQuotes:=~sc.this.inQuotes; 
						IF sc.this.inQuotes THEN sc.this.buf.Empty; sc.this.buf.quoted:=TRUE END
				ELSE
					IF ~sc.this.inQuotes THEN
						CASE c.x OF
							|'{': sc.this.sym:=objBegin;
							|'[': sc.this.sym:=arrBegin;
							|':': sc.this.sym:=nameSep;
							|',': sc.this.sym:=valueSep;
							|'}': sc.this.sym:=objEnd;
							|']': sc.this.sym:=arrEnd;
						ELSE sc.this.buf.AddChar(c.x) END;
					ELSE sc.this.buf.AddChar(c.x) END;
				END;
			ELSE
				sc.this.buf.AddChar(c.x)
			END;
		END;
	END Read;
	
	PROCEDURE (p: StdParser) Begin;
		(* VAR t: StdTarget; *)
	BEGIN
		p.root:=NewStackItem(-1);
		(* IF p.t=NIL THEN
			NEW(t); p.t:=t;
		END; *)
		ASSERT(p.t#NIL, 60);
		p.sc.Init;
	END Begin;
	
	PROCEDURE (p: StdParser) Step(OUT res: INTEGER);
		VAR type: INTEGER; i: StackItem; x: POINTER TO ARRAY OF CHAR;
		
		PROCEDURE Value;
		BEGIN
			IF p.sc.this.buf.empty THEN NEW(x, 1); x[0]:=0X; 
			ELSIF p.sc.this.buf.Len()>0 THEN x:=p.sc.this.buf.x.CopyOf() END;
			IF x#NIL THEN p.t.ThisValue(x$, p.sc.this.buf.quoted) END;
		END Value;
		
	BEGIN
		ASSERT(p.sc.res=continue, 20);
		p.sc.Read; 
		CASE p.sc.this.sym OF
			|objBegin:
				Push(p.root, p.sc.rd.Pos(), obj);
				p.t.LevelDown(p.sc.rd.Pos(), obj);
			|arrBegin:
				Push(p.root, p.sc.rd.Pos(), arr); 
				p.t.LevelDown(p.sc.rd.Pos(), arr);
			|objEnd:
				Value;
				p.t.LevelUp;
				p.sc.ClearBuffer;
			|arrEnd:
				Value;
				p.t.LevelUp;
				p.sc.ClearBuffer;
			|nameSep:
				IF p.sc.this.buf.Len()>0 THEN 
					x:=p.sc.this.buf.x.CopyOf();
					p.t.NextName(x$)
				END;
				p.sc.ClearBuffer;
			|valueSep:
				Value;
				p.sc.ClearBuffer;
		ELSE END;
		res:=p.sc.res;
	END Step;
	
	PROCEDURE (p: StdParser) Parse(OUT res: INTEGER): ANYPTR;
		VAR x: ANYPTR;
	BEGIN
		p.Begin;
		res:=p.sc.res;
		WHILE ~(res IN {err, eot}) DO 
			p.Step(res);
		END;
		IF res=eot THEN 
			x:=p.Result();
		END;
	RETURN x;
	END Parse;
	
	PROCEDURE (p: StdParser) Result(): ANYPTR;
		VAR x: ANYPTR;
	BEGIN
		IF p.t#NIL THEN x:=p.t.Result() END;
	RETURN x
	END Result;
	
	PROCEDURE (p: StdParser) SetTarget(t: Target);
	BEGIN
		ASSERT(t#NIL, 20);
		p.t:=t;
	END SetTarget;
	
	PROCEDURE (d: StdDir) New (rd: Reader): Parser;
		VAR p: StdParser;
	BEGIN
		ASSERT(rd#NIL, 20); ASSERT(rd.Base()#NIL, 21);
		NEW(p);
		p.sc.ConnectTo(rd);
		RETURN p;
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
		NEW(d); Install(d);
		stdDir:=d;
	END Init;
	
BEGIN
	Init;
END JSonParser.