Readme

members: Zoltán, Gergely

|	No.	 |	Projet steps:		|	Technologies:	|	Future updates:	|
-------------------------------------------------------------------------
|	1	 |	Generator class		|	PugiXml			|					|
|	2	 |	Unit test			|	Google test		|					|
|	3	 |	Registration system	|	Sqlite			|	MySQL			|
|	4	 |	Server				|	ZeroMQ			|	Socets			|
|	5	 | 	Frontend			|	Htmx			|					|
|	6	 |	Ai chat				|	Gpt API			|					|
|	7  	 |	Security			|	OpenSSL			|					|

---------- No. 1: ---------- 
	
---------- No. 2: ---------- 

---------- No. 3: ---------- 
Vision:
"I would like to create a chat application with end to end encryption on an Htmx frontend."

Requirement breakdown:
		Account					-- login,registration
		2 person communication	-- 
		2+ person communication	--
		self communication
		end to end encryption	-- for messaging
		Htmx frontend			-- reg, login, chat windows, profil, settings
	
Data modell:
		Users 			-- username, password, birthdate, profil pic, address, dark/light mode
		Message		 	-- Sender, Receiver/s, text message, 
		Encryption		-- Pub key, private key, [hol tarolod a ket kulcsot, milyen formaban es hogyan tovaabitod]
			milyen adat kell az encpreitionhoz 
			hol tarolom el az adatokat a titkositashoz
		Frontend	
			Kelle e valamit eltarolni?





---------- No. 4: ---------- 

---------- No. 5: ---------- 
	
Kérdések, amikre választ keresek:
	1. Az sql db msot a build mappában belül, helyezkedik el, kvázi minden buildelésnél felülírodnak a tábla tartalmai(ha nem az egész tábla).
		Valahogy kikéne tenni onnan --> cmake magic ON
	2.