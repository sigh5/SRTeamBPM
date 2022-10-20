#ifndef Engine_Macro_h__
#define Engine_Macro_h__

namespace Engine
{
#define		WINCMAX		480000

#define		WINCX		1280
#define		WINCY		1024


#define		VTXCNTX		129
#define		VTXCNTZ		129
#define		VTXITV		1


#define		OBJ_DEAD     99
#define		RETURN_ERR  -1
#define		SCENE_CHANGE_RETRURN	33


#define		MAX_CROSSROAD 15.f	// »ç°Å¸®

#define		MSG_GRAPHICDEV 27000

#define		CUSOR_INVENTORY  L"../Bin/Resource/Texture/MouseImage/cross_0.png"
#define		CUSOR_SHOT		 L"../Bin/Resource/Texture/MouseImage/cross_12.png"



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



#define		ID_SHOT_GUN		1
#define		ID_MAGNUM		2
#define		ID_HELMET1		3
#define		ID_HELMET2		4
// ........ 



#define		LOADING_IMAGE0			 L"LoadingImage0"
#define		LOADING_IMAGE1			 L"LoadingImagefsaijfhsa"
#define		LOADING_IMAGE2			 L"LoadingImage2"
#define		LOADING_IMAGE3			 L"LoadingImage3"
#define		LOADING_IMAGE4			 L"LoadingImage4"
#define		LOADING_IMAGE5			 L"LoadingImage5"
#define		LOADING_IMAGE6			 L"LoadingImage6"
#define		LOADING_IMAGE7			 L"LoadingImage7"
#define		LOADING_IMAGE8			 L"LoadingImage8"


#define		LOADING_FONT_BACKGROUND0  L"LoadingFontBackGround0"
#define		LOADING_FONT_BACKGROUND1  L"LoadingFontBackGroundfsajhdsk"
#define		LOADING_FONT_BACKGROUND2  L"LoadingFontBackGround2"
#define		LOADING_FONT_BACKGROUND3  L"LoadingFontBackGround3"
#define		LOADING_FONT_BACKGROUND4  L"LoadingFontBackGround4"
#define		LOADING_FONT_BACKGROUND5  L"LoadingFontBackGround5"
#define		LOADING_FONT_BACKGROUND6  L"LoadingFontBackGround6"
#define		LOADING_FONT_BACKGROUND7  L"LoadingFontBackGround7"
#define		LOADING_FONT_BACKGROUND8  L"LoadingFontBackGround8"



}

#endif // Engine_Macro_h__



