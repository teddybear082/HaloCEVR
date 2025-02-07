#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <d3d9.h>
#include "Config/Config.h"
#include "VR/IVR.h"
#include "Helpers/Renderer.h"
#include "Helpers/RenderTarget.h"
#include "Helpers/Objects.h"
#include "Maths/Vectors.h"
#include "WeaponHandler.h"
#include "InputHandler.h"
#include "InGameRenderer.h"

enum class ERenderState { UNKNOWN, LEFT_EYE, RIGHT_EYE, GAME, SCOPE};

class Game
{
public:
	static Game instance;

	void Init();
	void Shutdown();

	void OnInitDirectX();
	void PreDrawFrame(struct Renderer* renderer, float deltaTime);
	void PreDrawEye(struct Renderer* renderer, float deltaTime, int eye);
	void PostDrawEye(struct Renderer* renderer, float deltaTime, int eye);
	bool PreDrawScope(struct Renderer* renderer, float deltaTime);
	void PostDrawScope(struct Renderer* renderer, float deltaTime);
	void PreDrawMirror(struct Renderer* renderer, float deltaTime);
	void PostDrawMirror(struct Renderer* renderer, float deltaTime);
	void PostDrawFrame(struct Renderer* renderer, float deltaTime);

	bool PreDrawHUD();
	void PostDrawHUD();

	bool PreDrawMenu();
	void PostDrawMenu();

	bool PreDrawLoading(int param1, struct Renderer* renderer);
	void PostDrawLoading(int param1, struct Renderer* renderer);

	bool PreDrawCrosshair(short* anchorLocation);
	void PostDrawCrosshair();

	void PreDrawImage(void* param1, void* param2);
	void PostDrawImage(void* param1, void* param2);

	void UpdateViewModel(HaloID& id, struct Vector3* pos, struct Vector3* facing, struct Vector3* up, struct TransformQuat* BoneTransforms, struct Transform* OutBoneTransforms);
	void PreFireWeapon(HaloID& WeaponID, short param2);
	void PostFireWeapon(HaloID& WeaponID, short param2);
	void PreThrowGrenade(HaloID& playerID);
	void PostThrowGrenade(HaloID& playerID);

	void UpdateInputs();
	void UpdateCamera(float& yaw, float& pitch);
	void SetMousePosition(int& x, int& y);
	void UpdateMouseInfo(struct MouseInfo* mouseInfo);

	void SetViewportScale(struct Viewport* viewport);

	bool GetDrawMirror() const { return mirrorSource == ERenderState::GAME && c_DrawMirror->Value(); }

	ERenderState GetRenderState() const { return renderState; }

	float GetScopeSize() const { return c_ScopeScale->Value(); }

	float MetresToWorld(float m) const;
	float WorldToMetres(float w) const;

	inline IVR* GetVR() const { return vr; }

	UINT backBufferWidth = 600;
	UINT backBufferHeight = 600;

	// HACK: Some places it is hard to get the delta time (e.g. updating the camera)
	// Using the last known delta time should be good enough
	float lastDeltaTime = 0.0f;

	bool bNeedsRecentre = true;
	bool bUseTwoHandAim = true;

	InGameRenderer inGameRenderer;
	InGameRenderer scopeRenderer;

	bool bDetectedChimera = false;
	Vector3 LastLookDir;
protected:

	void CreateConsole();

	void PatchGame();

	void SetupConfigs();

	void CalcFPS(float deltaTime);

	void UpdateCrosshairAndScope();
	void SetScopeTransform(Matrix4& newTransform, bool bIsVisible);

	void StoreRenderTargets();
	void RestoreRenderTargets();

	void CreateTextureAndSurface(UINT Width, UINT Height, DWORD Usage, D3DFORMAT Format, struct IDirect3DSurface9** OutSurface, struct IDirect3DTexture9** OutTexture);

	WeaponHandler weaponHandler;
	InputHandler inputHandler;

	struct FPSTracker
	{
		float timeSinceFPSUpdate = 0.0f;
		int framesSinceFPSUpdate = 0;

		int fps = 0;
	} fpsTracker;

	FILE* consoleOut = nullptr;

	Config config;

	IVR* vr;

	RenderTarget gameRenderTargets[8];

	struct IDirect3DSurface9* uiSurface;
	struct IDirect3DSurface9* crosshairSurface;
	struct IDirect3DSurface9* uiRealSurface;
	struct IDirect3DSurface9* crosshairRealSurface;

	struct IDirect3DSurface9* scopeSurfaces[3];
	struct IDirect3DTexture9* scopeTextures[3];

	ERenderState renderState = ERenderState::UNKNOWN;

	CameraFrustum frustum1;
	CameraFrustum frustum2;

	short realZoom = -1;
	sRect realRect;
	sRect realLoadRect;
	UINT realUIWidth;
	UINT realUIHeight;

	DWORD realAlphaFunc;
	DWORD realAlphaSrc;
	DWORD realAlphaDest;

	bool bShowViewModel = false;

	bool bInVehicle = false;
	bool bHasWeapon = true;

	ERenderState mirrorSource;

	bool bHasShutdown = true;

	bool bWasLoading = false;

	//======Configs======//
public:

	BoolProperty* c_ShowConsole = nullptr;
	BoolProperty* c_DrawMirror = nullptr;
	IntProperty* c_MirrorEye = nullptr;
	FloatProperty* c_UIOverlayDistance = nullptr;
	FloatProperty* c_UIOverlayScale = nullptr;
	FloatProperty* c_UIOverlayCurvature = nullptr;
	IntProperty* c_UIOverlayWidth = nullptr;
	IntProperty* c_UIOverlayHeight = nullptr;
	BoolProperty* c_ShowCrosshair = nullptr;
	BoolProperty* c_SnapTurn = nullptr;
	FloatProperty* c_SnapTurnAmount = nullptr;
	FloatProperty* c_SmoothTurnAmount = nullptr;
	IntProperty* c_HandRelativeMovement = nullptr;
	FloatProperty* c_HandRelativeOffsetRotation = nullptr;
	FloatProperty* c_HorizontalVehicleTurnAmount = nullptr;
	FloatProperty* c_VerticalVehicleTurnAmount = nullptr;
	FloatProperty* c_LeftHandFlashlightDistance = nullptr;
	FloatProperty* c_RightHandFlashlightDistance = nullptr;
	Vector3Property* c_ControllerOffset = nullptr;
	Vector3Property* c_ControllerRotation = nullptr;
	FloatProperty* c_ScopeRenderScale = nullptr;
	FloatProperty* c_ScopeScale = nullptr;
	Vector3Property* c_ScopeOffsetPistol = nullptr;
	Vector3Property* c_ScopeOffsetSniper = nullptr;
	Vector3Property* c_ScopeOffsetRocket = nullptr;
	FloatProperty* c_LeftHandMeleeSwingSpeed = nullptr;
	FloatProperty* c_RightHandMeleeSwingSpeed = nullptr;
	FloatProperty* c_CrouchHeight = nullptr;
	BoolProperty* c_ShowRoomCentre = nullptr;
	BoolProperty* c_ToggleGrip = nullptr;
	BoolProperty* c_LeftHanded = nullptr;
	StringProperty* c_d3d9Path = nullptr;
};

