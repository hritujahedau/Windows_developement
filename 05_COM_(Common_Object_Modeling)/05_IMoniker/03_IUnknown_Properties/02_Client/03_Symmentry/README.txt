Identity

----Global changes----
1. take  pointer variables of IUnknown

	IOddNumber* gpIOddNumberAgain = NULL;
	
-----In WndProc()------

1. get gpIOddNumber and gpIEvenNumber as usual way
2. now call QueryInterface again from gpIEvenNumber to get gpIOddNumberAgain

	gpIEvenNumber->QueryInterface(IID_IOddNumber, (void**)&gpIOddNumberAgain);
	
3. Now check gpIOddNumber and gpIOddNumberAgain
	if equal - Symmentric
	if not -> not symmetric
4. Check and release all interfaces

