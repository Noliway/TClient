/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_CLIENT_COMPONENTS_PLAYERS_H
#define GAME_CLIENT_COMPONENTS_PLAYERS_H
#include <generated/protocol.h>

#include <game/client/component.h>
#include <game/client/render.h>

class CPlayers : public CComponent
{
	friend class CGhost;

	void RenderHand6(const CTeeRenderInfo *pInfo, vec2 HandPos, float HandAngle, float Alpha);
	void RenderHand7(const CTeeRenderInfo *pInfo, vec2 HandPos, float HandAngle, float Alpha);

	void RenderHand(const CTeeRenderInfo *pInfo, vec2 CenterPos, vec2 Dir, float AngleOffset, vec2 PostRotOffset, float Alpha);
	void RenderPlayer(
		const CScreenRect &ScreenRect,
		const CNetObj_Character *pPrevChar,
		const CNetObj_Character *pPlayerChar,
		const CTeeRenderInfo *pRenderInfo,
		int ClientId,
		float Intra = 0.f,
		bool RenderGhost = false);

	void RenderHook(
		const CScreenRect &ScreenRect,
		const CNetObj_Character *pPrevChar,
		const CNetObj_Character *pPlayerChar,
		const CTeeRenderInfo *pRenderInfo,
		int ClientId,
		float Intra = 0.f);
	void RenderHookCollLine(
		const CScreenRect &ScreenRect,
		const CNetObj_Character *pPrevChar,
		const CNetObj_Character *pPlayerChar,
		int ClientId);
	bool IsPlayerInfoAvailable(int ClientId) const;

	// Kagune: purely cosmetic tentacles, computed and drawn locally only
	enum
	{
		KAGUNE_MAX_TENTACLES = 8,
	};
	struct CKaguneState
	{
		bool m_Valid = false;
		int m_LastAttackTick = 0;
		int m_LastHookState = 0;
		int m_LastJumped = 0;
		int m_NextTentacle = 0;
		int m_HookMask = 0; // tentacles currently stretched out as the hook (set by RenderKaguneHook)
		float m_NextIdleLash = 0.0f;
		float m_aLashStart[KAGUNE_MAX_TENTACLES];
		float m_aLashAngle[KAGUNE_MAX_TENTACLES];
		float m_aLashReach[KAGUNE_MAX_TENTACLES];
	};
	CKaguneState m_aKagune[MAX_CLIENTS];
	std::vector<IGraphics::CFreeformItem> m_vKaguneQuads;

	void KaguneTrigger(CKaguneState &State, int Count, float Time, float Angle, float Reach, bool InOrder);
	void KaguneUpdate(CKaguneState &State, int Count, float Time, const CNetObj_Character &Player, vec2 Position, vec2 Aim);
	void KaguneAppendStrand(const vec2 *pPoints, const float *pHalfWidths, int NumPoints, float WidthMul);
	void KaguneDrawPass(float R, float G, float B, float A, bool Additive);
	void RenderKagune(int ClientId, vec2 Position, const CNetObj_Character &Player, vec2 Aim, vec2 Vel, float Alpha, bool Paused);
	void RenderKaguneHook(int ClientId, vec2 Pos, vec2 HookPos, float Alpha);

	int m_WeaponEmoteQuadContainerIndex;
	int m_aWeaponSpriteMuzzleQuadContainerIndex[NUM_WEAPONS];

	void CreateNinjaTeeRenderInfo();
	void CreateSpectatorTeeRenderInfo();

	std::shared_ptr<CManagedTeeRenderInfo> m_pNinjaTeeRenderInfo;
	std::shared_ptr<CManagedTeeRenderInfo> m_pSpectatorTeeRenderInfo;

public:
	float GetPlayerTargetAngle(
		const CNetObj_Character *pPrevChar,
		const CNetObj_Character *pPlayerChar,
		int ClientId,
		float Intra = 0.0f);

	int Sizeof() const override { return sizeof(*this); }
	void OnInit() override;
	void OnRender() override;

	const std::shared_ptr<CManagedTeeRenderInfo> &NinjaTeeRenderInfo() const { return m_pNinjaTeeRenderInfo; }
	const std::shared_ptr<CManagedTeeRenderInfo> &SpectatorTeeRenderInfo() const { return m_pSpectatorTeeRenderInfo; }
};

#endif
