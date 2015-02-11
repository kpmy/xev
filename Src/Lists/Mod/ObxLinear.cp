MODULE ListsObxLinear;
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

	IMPORT Linear := ListsLinear, Log:=Out;
	
	TYPE
		StdItem = POINTER TO RECORD(Linear.ListItem) i: INTEGER END;
		
	PROCEDURE Test*;
	VAR i : INTEGER; item : StdItem; list : Linear.List;
	BEGIN
		list:=Linear.NewList(); list.SetLength(1000);
		FOR i:=0 TO 999 DO
			NEW(item); item.i:=1000-i; list.SetItem(i, item);
		END;
		FOR i:=0 TO 999 DO
			item:=list.GetItem(i)(StdItem); Log.Int(item.i);
		END;	
	END Test;
END ListsObxLinear.

(*JediPlain*gvFEZXZDb21tYW5kZXJzLlN0ZFZpZXdEZXNjAPFEZXZDb21tYW5kZXJzLlZpZXdEZXNjAPFWaWV3cy5WaWV3RGVzYwDwU3RvcmVzLlN0b3JlRGVzYwAAAAAAAAAAAAAAAAAGAAAAAAAAFQAA*)ListsObxLinear.Test
	