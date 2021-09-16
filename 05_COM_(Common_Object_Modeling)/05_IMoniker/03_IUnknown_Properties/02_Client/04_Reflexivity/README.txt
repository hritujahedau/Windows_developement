Identity

----Global changes----
1. Remove gpIEvenNumber interface pointer and code

2. take  pointer variables of IUnknown

	IOddNumber* gpIOddNumberAgain = NULL;

	
-----In WndProc()------

1. get gpIOddNumber and gpIEvenNumber as usual way

2. now call QueryInterface again from gpIOddNumber to get gpIOddNumberAgain

	gpIOddNumber->QueryInterface(IID_IOddNumber, (void**)&gpIOddNumberAgain);
	
3. Now check gpIOddNumber and gpIOddNumberAgain
	if equal - Reflexivity true
	if not -> Reflexivity false
	
4. Check and release all interfaces
