MODULE Core;
	VAR
		go_process*: PROCEDURE (IN s: ARRAY OF SHORTCHAR);
	
	PROCEDURE LoadMod*(name: ARRAY OF CHAR);
		VAR x: ARRAY 1024 OF CHAR;
	BEGIN
		x:='{"type": "core", "command": "load", "data": "'+name$+'"}';
		go_process(SHORT(x));		
	END LoadMod;
	
END Core.