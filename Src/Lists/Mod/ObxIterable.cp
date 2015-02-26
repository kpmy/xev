MODULE ListsObxIterable;
(**
	project	= "BlackBox"
	subproject	= "Lists library"
	organization	= ""
	contributors	= "(*JediPlain*gvFTdGRMaW5rcy5MaW5rRGVzYwDxVmlld3MuVmlld0Rlc2MA8FN0b3Jlcy5TdG9yZURlc2MAAAAAAAAAAAAAAAAAawAAAAAAAAFjAAAAaTIxc3lzQ2FsbHMuT3BlbignaHR0cDovL29iZXJvbmNvcmUucnUvbWVtYmVycy8lRDAlQkElRDElODMlRDElODglRDAlQkQlRDAlQjglRDElODBfJUQwJUJGJUQwJUJDJykA*)Кушнир П. М.(*JediPlain*gvFTdGRMaW5rcy5MaW5rRGVzYwDxVmlld3MuVmlld0Rlc2MA8FN0b3Jlcy5TdG9yZURlc2MAAAAAAAAAAAAAAAAACAAAAAAAAAAAAAAA*)"
	version	= "(*JediPlain*gvFTdGRMaW5rcy5MaW5rRGVzYwDxVmlld3MuVmlld0Rlc2MA8FN0b3Jlcy5TdG9yZURlc2MAAAAAAAAAAAAAAAAASAAAAAAAAAFAAAAAU3RkQ21kcy5PcGVuVG9vbERpYWxvZygnU3lzdGVtL1JzcmMvKi9BYm91dCcsICdBYm91dCBCbGFja0JveCcpAA==*)System/Rsrc/About(*JediPlain*gvFTdGRMaW5rcy5MaW5rRGVzYwDxVmlld3MuVmlld0Rlc2MA8FN0b3Jlcy5TdG9yZURlc2MAAAAAAAAAAAAAAAAACAAAAAAAAAAAAAAA*)"
	copyright	= "(*JediPlain*gvFTdGRMaW5rcy5MaW5rRGVzYwDxVmlld3MuVmlld0Rlc2MA8FN0b3Jlcy5TdG9yZURlc2MAAAAAAAAAAAAAAAAASAAAAAAAAAFAAAAAU3RkQ21kcy5PcGVuVG9vbERpYWxvZygnU3lzdGVtL1JzcmMvKi9BYm91dCcsICdBYm91dCBCbGFja0JveCcpAA==*)System/Rsrc/About(*JediPlain*gvFTdGRMaW5rcy5MaW5rRGVzYwDxVmlld3MuVmlld0Rlc2MA8FN0b3Jlcy5TdG9yZURlc2MAAAAAAAAAAAAAAAAACAAAAAAAAAAAAAAA*)"
	license	= "(*JediPlain*gvFTdGRMaW5rcy5MaW5rRGVzYwDxVmlld3MuVmlld0Rlc2MA8FN0b3Jlcy5TdG9yZURlc2MAAAAAAAAAAAAAAAAARQAAAAAAAAE9AAAAU3RkQ21kcy5PcGVuQnJvd3NlcignRG9jdS8qL0JCLUxpY2Vuc2UnLCAnQmxhY2tCb3ggTGljZW5zZScpAA==*)Docu/BB-License(*JediPlain*gvFTdGRMaW5rcy5MaW5rRGVzYwDxVmlld3MuVmlld0Rlc2MA8FN0b3Jlcy5TdG9yZURlc2MAAAAAAAAAAAAAAAAACAAAAAAAAAAAAAAA*)"
	purpose	= ""
	changes	= ""
	issues	= ""

**)
	

	IMPORT It := ListsIterable, Log:=Out;

	TYPE
		StdItem = POINTER TO RECORD (It.IteratorItem) i: INTEGER END;

	PROCEDURE Test*;
	VAR i: INTEGER; item: StdItem; it: It.Iterator;
	BEGIN
		it := It.NewIterator();
		FOR i := 0 TO 99 DO
			NEW(item); item.i := i; it.Add(item)
		END;
		it.First;
		WHILE it.this # NIL DO
			IF ((it.this(StdItem).i + 1) MOD 2) = 0 THEN it.Remove(it.this) END;
			IF it.this#NIL THEN it.Next END;
		END;
		it.First;
		WHILE it.this # NIL DO
			Log.Int(it.this(StdItem).i);
			it.Next
		END
	END Test;

END ListsObxIterable.

(*JediPlain*gvFEZXZDb21tYW5kZXJzLlN0ZFZpZXdEZXNjAPFEZXZDb21tYW5kZXJzLlZpZXdEZXNjAPFWaWV3cy5WaWV3RGVzYwDwU3RvcmVzLlN0b3JlRGVzYwAAAAAAAAAAAAAAAAAGAAAAAAAAFQAA*)"ListsObxIterable.Test" 