MODULE JSonGenerator;
(**
	project	= "JSon"
	organization	= ""
	contributors	= ""
	version	= "System/Rsrc/About"
	copyright	= "Kushnir Piotr Michailovich"
	license	= "Docu/BB-License"
	purpose	= "Реализация генератора"
	changes	= "
	- 20130102, pk, автогенерация заголовка
"
	issues	= "-
	 20150204, порт для fw
	"
**)
	
	IMPORT 
		Str;
		
	TYPE
		Writer* = POINTER TO ABSTRACT RECORD 
			base: ANYPTR;
		END;
		
	PROCEDURE (w: Writer) Whitespace* (s: ARRAY OF CHAR), NEW, EMPTY;
	PROCEDURE (w: Writer) Char* (ch: CHAR), NEW, ABSTRACT;
	
	PROCEDURE (w: Writer) UnicodeChar* (ch: CHAR), NEW, EXTENSIBLE;
		VAR x: INTEGER; vs: ARRAY 12 OF CHAR;
	BEGIN
		x:=ORD(ch);
		Str.IntToStringForm(x, Str.hexadecimal, 4, '0', Str.hideBase, vs);
		w.Char('\'); w.Char('u'); w.Char(vs[0]); w.Char(vs[1]); w.Char(vs[2]); w.Char(vs[3]);  
	END UnicodeChar;
	
	PROCEDURE (w: Writer) Base* (): ANYPTR, NEW;
	BEGIN
		RETURN w.base;
	END Base;
	
	PROCEDURE (w: Writer) SetBase* (base: ANYPTR), NEW, EXTENSIBLE;
	BEGIN
		ASSERT(base#NIL, 20);
		w.base:=base;
	END SetBase;
	
END JSonGenerator.