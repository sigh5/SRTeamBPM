#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum WINMODE	 { MODE_FULL, MODE_WIN };
	
// ID_DYNAMIC : 매 프레임마다 갱신을 해야하는 컴포넌트 집단
// ID_STATIC : 한 번 만들면 그대로 사용할 컴포넌트 집단
	enum COMPONENTID { ID_DYNAMIC, ID_STATIC, ID_END };

	enum RENDERID { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END };

	enum INFOID { INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };

	enum ROTATIONID { ROT_X, ROT_Y, ROT_Z, ROT_END};

	enum TEXTUREID { TEX_NORMAL, TEX_CUBE, TEX_END };
	// 0929 Pjw's add enum (LOADING_PLAYER_UI)
	enum LOADINGID { LOADING_STAGE,LOADING_COLLIDER, LOADING_BOSS, LOADING_TEST, LOADING_PLAYER_UI, LOADING_END };

	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };

	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y,DIMS_Z,  DIMS_END }; 
	
	enum CREATECUBE { CREATECUBE_NONE ,CREATECUBE_LEFT, CREATECUBE_RIGHT, CREATECUBE_UP, CREATECUBE_DOWN, CREATECUBE_Z, CREATECUBE_END};

	enum DIR {DIR_UP,DIR_DOWN,DIR_LEFT,DIR_RIGHT, DIR_END};
	
	enum OBJ_TYPE { OBJ_CUBE, OBJ_PLAYER, OBJ_MONSTER, OBJ_TYPE_END};

	enum SCENE_TYPE { SCENE_TOOLTEST, SCENE_STAGE, SCENE_END };

	// 최대 32개 채널까지 동시 재생이 가능
	enum CHANNELID { SOUND_EFFECT, SOUND_BGM, SOUND_PLAYER, SOUND_MONSTER, MAXCHANNEL };


	// ※
	enum TYPING_TYPE { TYPING_W, TYPING_A, TYPING_S, TYPING_D, TYPING_END };
}

#endif // Engine_Enum_h__




