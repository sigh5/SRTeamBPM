#ifndef Engine_Macro_h__
#define Engine_Macro_h__

namespace Engine
{
#define		WINCMAX		480000

#define		WINCX		800
#define		WINCY		600


#define		VTXCNTX		129
#define		VTXCNTZ		129
#define		VTXITV		1


#define		OBJ_DEAD     99
#define		RETURN_ERR  -1

#define		MAX_CROSSROAD 15.f	// »ç°Å¸®



#ifndef			MSG_BOX
#define			MSG_BOX(_message)			MessageBox(NULL, TEXT(_message), L"System Message", MB_OK)
#endif

#define			BEGIN(NAMESPACE)		namespace NAMESPACE {
#define			END						}

#define			USING(NAMESPACE)	using namespace NAMESPACE;
	//

#ifdef	ENGINE_EXPORTS
#define ENGINE_DLL		_declspec(dllexport)
#else
#define ENGINE_DLL		_declspec(dllimport)
#endif

#define NULL_CHECK( _ptr)	\
	{if( _ptr == 0){__asm { int 3 };return;}}

#define NULL_CHECK_RETURN( _ptr, _return)	\
	{if( _ptr == 0){__asm { int 3 };return _return;}}

#define NULL_CHECK_MSG( _ptr, _message )		\
	{if( _ptr == 0){MessageBox(NULL, _message, L"System Message",MB_OK);__asm { int 3 };}}

#define NULL_CHECK_RETURN_MSG( _ptr, _return, _message )	\
	{if( _ptr == 0){MessageBox(NULL, _message, L"System Message",MB_OK);__asm { int 3 };return _return;}}


#define FAILED_CHECK(_hr)	if( ((HRESULT)(_hr)) < 0 )	\
	{ MessageBoxW(NULL, L"Failed", L"System Error",MB_OK); __asm { int 3 }; return E_FAIL;}

#define FAILED_CHECK_RETURN(_hr, _return)	if( ((HRESULT)(_hr)) < 0 )		\
	{ MessageBoxW(NULL, L"Failed", L"System Error",MB_OK); __asm { int 3 }; return _return;}

#define FAILED_CHECK_MSG( _hr, _message)	if( ((HRESULT)(_hr)) < 0 )	\
	{ MessageBoxW(NULL, _message, L"System Message",MB_OK); __asm { int 3 };return E_FAIL;}

#define FAILED_CHECK_RETURN_MSG( _hr, _return, _message)	if( ((HRESULT)(_hr)) < 0 )	\
	{ MessageBoxW(NULL, _message, L"System Message",MB_OK); __asm { int 3 };return _return;}



#define NO_COPY(CLASSNAME)							\
		private:											\
		CLASSNAME(const CLASSNAME&) = delete;					\
		CLASSNAME& operator = (const CLASSNAME&) = delete;		

#define DECLARE_SINGLETON(CLASSNAME)				\
		NO_COPY(CLASSNAME)								\
		private:										\
		static CLASSNAME*	m_pInstance;				\
		public:											\
		static CLASSNAME*	GetInstance( void );		\
		static void DestroyInstance( void );			

#define IMPLEMENT_SINGLETON(CLASSNAME)				\
		CLASSNAME*	CLASSNAME::m_pInstance = NULL;		\
		CLASSNAME*	CLASSNAME::GetInstance( void )	{	\
			if(NULL == m_pInstance) {					\
				m_pInstance = new CLASSNAME;			\
			}											\
			return m_pInstance;							\
		}												\
		void CLASSNAME::DestroyInstance( void ) {		\
			if(NULL != m_pInstance)	{					\
				delete m_pInstance;						\
				m_pInstance = NULL;						\
			}											\
		}


#define READY_LAYER(PGAMEOBJECT,CLASSNAME, PLAYER,GRAPHICDEV,OBJNAME)	\
			PGAMEOBJECT = CLASSNAME::Create(GRAPHICDEV);		 \
			NULL_CHECK_RETURN(PGAMEOBJECT, E_FAIL);			\
			FAILED_CHECK_RETURN(PLAYER->Add_GameObject(OBJNAME,PGAMEOBJECT),E_FAIL); 


#define READY_LAYER_POS(PGAMEOBJECT,CLASSNAME, PLAYER,GRAPHICDEV,OBJNAME, POSX, POSZ)	\
			PGAMEOBJECT = CLASSNAME::Create(GRAPHICDEV,POSX,POSZ);		 \
			NULL_CHECK_RETURN(PGAMEOBJECT, E_FAIL);			\
			FAILED_CHECK_RETURN(PLAYER->Add_GameObject(OBJNAME,PGAMEOBJECT),E_FAIL); 


// NO_NULLCHECK
#define READY_CREATE_EFFECT_VECTOR(PGAMEOBJECT,CLASSNAME, PLAYER,GRAPHICDEV, VECTOR3)	\
			PGAMEOBJECT = CLASSNAME::Create(GRAPHICDEV,VECTOR3);		 \
			if(PGAMEOBJECT==nullptr) return;		\
			PLAYER->Add_EffectList(PGAMEOBJECT); 



#define ADD_CLONE_PROTO(CLASSNAME,PCOMPONENT,MAP_CONTAINER,MAP_CONTAINER_TYPE,PROTO_NAME) \
			PCOMPONENT =dynamic_cast<CLASSNAME*>(Clone_Proto(PROTO_NAME));				  \
			NULL_CHECK_RETURN(PCOMPONENT,E_FAIL);										  \
			MAP_CONTAINER[MAP_CONTAINER_TYPE].insert({PROTO_NAME,PCOMPONENT});				


}

#endif // Engine_Macro_h__



