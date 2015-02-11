MODULE ListsObxMap;
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

	

	IMPORT Map := ListsMap, Log:=Out;

	TYPE
		TestType = POINTER TO RECORD x: INTEGER END;
		TestId = POINTER TO RECORD(Map.Id) x: INTEGER END;


	PROCEDURE (i: TestId)Equal (with: Map.Id): BOOLEAN;
	BEGIN
		Log.String('compare ');
		RETURN i.x = with(TestId).x

	END Equal;

	PROCEDURE Test*;
	VAR x: ANYPTR; x1, x2, x3: TestType; id: TestId; i: Map.IntMap; s: Map.StrMap; p: Map.PtrMap;
	BEGIN
		NEW(x1); x1.x := 1111;
		NEW(x2); x2.x := 2222;
		NEW(x3); x3.x := 3333;

		i := Map.NewIntMap();
		i.Add(1, x1);
		i.Add(2, x2);
		i.Add(3, x3);
		i.Remove(2);
		x := i.Item(2);
		IF x # NIL THEN Log.Int(x(TestType).x) END;
		Log.Ln;
		x := i.Item(2);
		IF x # NIL THEN Log.Int(x(TestType).x) ELSE Log.String('removed') END;
		Log.Ln;
		i.Add(2, x2);
		x := i.Item(2);
		IF x # NIL THEN Log.Int(x(TestType).x) END;
		Log.Ln;
		i.Remove(2);
		x := i.Item(2);
		IF x # NIL THEN Log.Int(x(TestType).x) ELSE Log.String('removed') END;
		Log.Ln;

		s := Map.NewStrMap();
		s.Add('1', x1);
		s.Add('2', x2);
		s.Add('3', x3);
		s.Remove('2');
		x := s.Item('2');
		IF x # NIL THEN Log.Int(x(TestType).x) END;
		Log.Ln;
		x := s.Item('2');
		IF x # NIL THEN Log.Int(x(TestType).x) ELSE Log.String('removed') END;
		Log.Ln;
		s.Add('2', x2);
		x := s.Item('2');
		IF x # NIL THEN Log.Int(x(TestType).x) END;
		Log.Ln;
		s.Remove('2');
		x := s.Item('2');
		IF x # NIL THEN Log.Int(x(TestType).x) ELSE Log.String('removed') END;
		Log.Ln;

		p := Map.NewPtrMap();
		NEW(id); id.x := 1; p.Add(id, x1);
		NEW(id); id.x := 2; p.Add(id, x2);
		NEW(id); id.x := 3; p.Add(id, x3);
		NEW(id); id.x := 2;
		p.Remove(id);
		x := p.Item(id);
		IF x # NIL THEN Log.Int(x(TestType).x) END;
		Log.Ln;
		x := p.Item(id);
		IF x # NIL THEN Log.Int(x(TestType).x) ELSE Log.String('removed') END;
		Log.Ln;
		p.Add(id, x2);
		x := p.Item(id);
		IF x # NIL THEN Log.Int(x(TestType).x) END;
		Log.Ln;
		p.Remove(id);
		x := p.Item(id);
		IF x # NIL THEN Log.Int(x(TestType).x) ELSE Log.String('removed') END;
		Log.Ln
	END Test;

END ListsObxMap.

(*JediPlain*gvFEZXZDb21tYW5kZXJzLlN0ZFZpZXdEZXNjAPFEZXZDb21tYW5kZXJzLlZpZXdEZXNjAPFWaWV3cy5WaWV3RGVzYwDwU3RvcmVzLlN0b3JlRGVzYwAAAAAAAAAAAAAAAAAGAAAAAAAAFQAA*)ListsObxMap.Test