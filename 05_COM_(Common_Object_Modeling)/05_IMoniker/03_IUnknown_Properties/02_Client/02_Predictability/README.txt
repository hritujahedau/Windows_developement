Identity

----Global changes----
1. take  pointer variables of IUnknown

	IOddNumber* gpIOddNumberAgain = NULL;
	
-----In WndProc()------

1. get gpIOddNumber and gpIEvenNumber as usual way
2. now call Sleep(10000); 
3. now call QueryInterface again from gpIOddNumber to get gpIOddNumberAgain

	gpIOddNumber->QueryInterface(IID_IOddNumber, (void**)&gpIOddNumberAgain);
	
4. Now check gpIOddNumber and gpIOddNumberAgain
	if equal - we have go same interface again
	if not -> we didn't get same interface again
	
5. Check and release all interfaces

