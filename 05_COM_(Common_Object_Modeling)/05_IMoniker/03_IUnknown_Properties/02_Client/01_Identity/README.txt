Identity

----Global changes----
1. take two pointr variables of IUnknown

	IUnknown* gpIUnknownFromIOddNumber = NULL;
	IUnknown* gpIUnknownFromIEvenNumber = NULL;
	
-----In WndProc()------

1. get gpIOddNumber and gpIEvenNumber as usual way

2. Now get gpIUnknownFromIEvenNumber interface from gpIEvenNumber interface

3. Now get gpIUnknownFromIOddNumber interface from gpIOddNumber interface

4. now compare gpIUnknownFromIEvenNumber and gpIUnknownFromIOddNumber 
	if they are equal -> from same component
	else -> not from same component
	
5. Check and release all interfaces
