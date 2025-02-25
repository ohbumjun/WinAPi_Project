#include "BossMonster.h"
#include "../UI/UIBossStateHUD.h"
#include "../UI/UICharacterStateHUD.h"
#include "../UI/UIGunStateHUD.h"
#include "../UI/UIBossEnd.h"
#include "../Collision/ColliderBox.h"
#include "../Scene/Scene.h"
#include"../Scene/SceneResource.h"
#include "EffectGrenade.h"
#include "GeneratorTower.h"
#include "KamikazeCage.h"
#include "DuckMonster.h"
#include "TurtleMonster.h"

CBossMonster::CBossMonster() :
	m_MissileAttackMaxTime(5.f),
	m_MissileAttackTime(0.f),
	m_GrenadeTime(0.f),
	m_GrenadMaxTime(5.f),
	m_IsGenerator1Alive(false),
	m_IsGenerator2Alive(false),
	m_MonsterDeath(false)
{
	m_MonsterType = EMonster_Type::Boss;
	m_AttackDistance = 2000.f;
	m_DashDistance = 3000.f;
	m_CharacterInfo.HP = -1;
}

CBossMonster::CBossMonster(const CBossMonster& obj) : CMonster(obj)
{
	m_MissileAttackMaxTime = 5.f;
	m_MissileAttackTime = 0.f;
	m_GrenadeTime = 0.f;
	m_GrenadMaxTime = 3.f;
	m_IsGenerator1Alive = false;
	m_IsGenerator2Alive = false;
	m_MonsterDeath = false;
}

CBossMonster::~CBossMonster()
{
}

void CBossMonster::CreateGenerator(int GeneratorNum)
{
	float TowerPoxX = GeneratorNum == 1 ? 600.f : 3300.f;
	CGeneratorTower* GeneratorTower = m_Scene->CreateObject<CGeneratorTower>("GeneratorTower", Vector2(TowerPoxX, 1300.f));
	// GeneratorTower->SetCharacterInfo(NORMAL_MONSTER_ATTACK, 100, 3000,
	//	5, 1, 100, 100, 600, NORMAL_MONSTER_ATTACK_DISTANCE, NORMAL_MONSTER_DASH_DISTANCE);
	GeneratorTower->SetCharacterInfo(NORMAL_MONSTER_ATTACK, 250, 5000,
		5, 1, 100, 100, 600, NORMAL_MONSTER_ATTACK_DISTANCE, NORMAL_MONSTER_DASH_DISTANCE);
	GeneratorTower->SetBossMonster(this);
	if (GeneratorNum == 1) m_IsGenerator1Alive = true;
	else m_IsGenerator2Alive = true;
	m_ShieldEnable = true;

	for (int i = 0; i < 3; i++)
	{
		Vector2 TowerPos = GeneratorTower->GetPos();
		CKamiKazeCage* KamiKaze = m_Scene->CreateObject<CKamiKazeCage>("KamiKazeCage", MONSTER_KAMIKAZECAGE_PROTO,
			TowerPos);
		KamiKaze->SetMonsterType(EMonster_Type::KamiKazeCage2);
		KamiKaze->SetMoveSpeed(0.f);
	}

	CreateSubMonsters();
}


void CBossMonster::MissileUpdate(float DeltaTime)
{
	if (m_MonsterDeath) return;
	m_MissileAttackTime += DeltaTime;
	if (m_MissileAttackTime >= m_MissileAttackMaxTime)
	{
		m_MissileAttackTime -= m_MissileAttackMaxTime;
	}
}

void CBossMonster::MissileAttack(float DeltaTime)
{
}

void CBossMonster::GrenadeUpdate(float DeltaTime)
{
	if (m_MonsterDeath) return;
	m_GrenadeTime += DeltaTime;
	if (m_GrenadeTime >= m_GrenadMaxTime)
	{
		m_GrenadeTime -= m_GrenadMaxTime;
		for (float f = 0.0f; f < 2 * M_PI; f += M_PI / 3.f )
		{
			CEffectGrenade* EffectGrenade = m_Scene->CreateObject<CEffectGrenade>(
				"GrenadeEffect",
				GRENADE_PROTO, 
				Vector2(
					(m_Pos.x - m_Offset.x) + m_Size.Length() * 1.8f * cos(f), 
					(m_Pos.y - m_Size.y - m_Offset.y) + m_Size.Length() * 1.8f * sin(f))
				);
			EffectGrenade->SetTexture("Grenade", TEXT("images/Monster/Boss/grenade.bmp"));
			EffectGrenade->SetTextureColorKey(255, 255, 255);
		}
	}
}

void CBossMonster::GrenadeAttack(float DeltaTime)
{
}

void CBossMonster::GeneratorUpdate(float DeltaTime)
{
	if (m_MonsterDeath) return;
	if (m_CharacterInfo.HP > m_CharacterInfo.HPMax * 0.45f
		&& m_CharacterInfo.HP <= m_CharacterInfo.HPMax * 0.7f)
	{
		// Make Generator 
		if (!m_IsGenerator1Alive)
			CreateGenerator(1);
		// Make Kage 

		// Make Red Line To Generator From Monster
	}
	else if (m_CharacterInfo.HP > m_CharacterInfo.HPMax * 0.25f && 
		m_CharacterInfo.HP <= m_CharacterInfo.HPMax * 0.45f)
	{
		if (!m_IsGenerator2Alive)
			CreateGenerator(2);
	}
}

void CBossMonster::GeneratorAttack(float DeltaTime)
{
}

void CBossMonster::CreateSubMonsters()
{
	CShotGun* ShotGun = m_Scene->CreateObject<CShotGun>(GUN_SHOTGUN_LIGHT, GUN_SHOTGUN_LIGHT_PROTO);

	CDuckMonster* DuckMonster = m_Scene->CreateObject<CDuckMonster>("DuckMonster",MONSTER_DUCK1_PROTO);
	DuckMonster->SetPos(Vector2(rand() % 1000 + 1000.f , rand() % 1000 + 1000.f));
	DuckMonster->Equip((CGun*)ShotGun);
	DuckMonster = m_Scene->CreateObject<CDuckMonster>("DuckMonster",MONSTER_DUCK2_PROTO);
	DuckMonster->SetPos(Vector2(rand() % 1000 + 1000.f, rand() % 1000 + 1000.f));
	ShotGun = m_Scene->CreateObject<CShotGun>(GUN_SHOTGUN_LIGHT, GUN_SHOTGUN_LIGHT_PROTO);
	DuckMonster->Equip((CGun*)ShotGun);
	DuckMonster = m_Scene->CreateObject<CDuckMonster>("DuckMonster",MONSTER_DUCK3_PROTO);
	DuckMonster->SetPos(Vector2(rand() % 1000 + 1000.f, rand() % 1000 + 1000.f));
	ShotGun = m_Scene->CreateObject<CShotGun>(GUN_SHOTGUN_LIGHT, GUN_SHOTGUN_LIGHT_PROTO);
	DuckMonster->Equip((CGun*)ShotGun);

	// Turtle Monster
	CTurtleMonster* TurtleMonster = m_Scene->CreateObject<CTurtleMonster>("TurtleMonster",MONSTER_TURTLE1_PROTO);
	TurtleMonster->SetPos(Vector2(rand() % 1000 + 1000.f, rand() % 1000 + 1000.f));
	ShotGun = m_Scene->CreateObject<CShotGun>(GUN_SHOTGUN_LIGHT, GUN_SHOTGUN_LIGHT_PROTO);
	TurtleMonster->Equip((CGun*)ShotGun);
	TurtleMonster = m_Scene->CreateObject<CTurtleMonster>("TurtleMonster",MONSTER_TURTLE2_PROTO);
	TurtleMonster->SetPos(Vector2(rand() % 1000 + 1000.f, rand() % 1000 + 1000.f));
	ShotGun = m_Scene->CreateObject<CShotGun>(GUN_SHOTGUN_LIGHT, GUN_SHOTGUN_LIGHT_PROTO);
	TurtleMonster->Equip((CGun*)ShotGun);
	TurtleMonster = m_Scene->CreateObject<CTurtleMonster>("TurtleMonster",MONSTER_TURTLE3_PROTO);
	TurtleMonster->SetPos(Vector2(rand() % 1000 + 1000.f, rand() % 1000 + 1000.f));
	ShotGun = m_Scene->CreateObject<CShotGun>(GUN_SHOTGUN_LIGHT, GUN_SHOTGUN_LIGHT_PROTO);
	TurtleMonster->Equip((CGun*)ShotGun);


}

void CBossMonster::UIUpdate(float DeltaTime)
{
	if (m_MonsterDeath) return;
	CUIBossStateHUD* State = m_Scene->FindUIWindow<CUIBossStateHUD>("BossStateHUD");
	State->SetHPPercent(m_CharacterInfo.HP / (float)m_CharacterInfo.HPMax);
}

void CBossMonster::ChangeIdleAnimation()
{
	CMonster::ChangeIdleAnimation();

	if (m_Dir.x < 0)
		ChangeAnimation(MONSTER_BOSS_LEFT_IDLE);
	else
		ChangeAnimation(MONSTER_BOSS_RIGHT_IDLE);
}

void CBossMonster::ChangeMoveAnimation()
{
	CMonster::ChangeMoveAnimation();
	if (m_Dir.x < 0)
		ChangeAnimation(MONSTER_BOSS_LEFT_WALK);
	else
		ChangeAnimation(MONSTER_BOSS_RIGHT_WALK);
}

void CBossMonster::ChangeRunAnimation()
{
	CMonster::ChangeRunAnimation();
	if (m_Dir.x < 0)
		ChangeAnimation(MONSTER_BOSS_LEFT_RUN);
	else
		ChangeAnimation(MONSTER_BOSS_RIGHT_RUN);
}

void CBossMonster::ChangeDeathAnimation()
{
	CMonster::ChangeDeathAnimation();
	if (m_Dir.x < 0)
		ChangeAnimation(MONSTER_BOSS_LEFT_DEATH);
	else
		ChangeAnimation(MONSTER_BOSS_RIGHT_DEATH);
}

void CBossMonster::ChangeHitAnimation()
{
	CMonster::ChangeHitAnimation();
	if (m_Dir.x < 0)
		ChangeAnimation(MONSTER_BOSS_LEFT_HIT);
	else
		ChangeAnimation(MONSTER_BOSS_RIGHT_HIT);
}

void CBossMonster::AIDeath(float DeltaTime)
{
	CMonster::AIDeath(DeltaTime);
	// 화면내 Player 제외 모든 몬스터 제외 
	m_MonsterDeath = true;
}

void CBossMonster::SetAnimation()
{
	// Right
	AddAnimation(MONSTER_BOSS_RIGHT_IDLE, true, 2.f);
	AddAnimation(MONSTER_BOSS_RIGHT_WALK, true, 1.f);
	AddAnimation(MONSTER_BOSS_RIGHT_ATTACK, false, 0.1f);
	AddAnimation(MONSTER_BOSS_RIGHT_RUN, true, 0.6f);
	AddAnimation(MONSTER_BOSS_RIGHT_HIT, false, 1.f);
	AddAnimation(MONSTER_BOSS_RIGHT_DEATH, false, 1.0f);

	// Left
	AddAnimation(MONSTER_BOSS_LEFT_IDLE, true, 2.f);
	AddAnimation(MONSTER_BOSS_LEFT_WALK, true, 1.f);
	AddAnimation(MONSTER_BOSS_LEFT_ATTACK, false, 0.1f);
	AddAnimation(MONSTER_BOSS_LEFT_RUN, true, 0.6f);
	AddAnimation(MONSTER_BOSS_LEFT_HIT, false, 1.f);
	AddAnimation(MONSTER_BOSS_LEFT_DEATH, false, 1.0f);

}

void CBossMonster::CharacterDestroy()
{
	CCharacter::CharacterDestroy();
	// Create End Widget
	CUIBossEnd* SelectWindow = m_Scene->CreateUIWindow<CUIBossEnd>("UIEnd");

	// Clear Monsters
	m_Scene->ClearAllMonsters();

	// Equip Boss Gun To Player
	CGun* BossGun = m_Scene->CreateObject<CGun>("BossGun", BOSS_GUN_PROTO, m_Pos);
	CPlayer* Player = (CPlayer*)m_Scene->GetPlayer();
	Player->Equip(BossGun);

	// Clear UI 
	CUICharacterStateHUD* StateWindow = m_Scene->FindUIWindow<CUICharacterStateHUD>("CharacterStateHUD");
	if (StateWindow) StateWindow->SetVisibility(false);

	CUIGunStateHUD* GunStateWindow = m_Scene->FindUIWindow<CUIGunStateHUD>("GunStateHUD");
	if (GunStateWindow) GunStateWindow->SetVisibility(false);

	CUIBossStateHUD* BossStateWindow = m_Scene->FindUIWindow<CUIBossStateHUD>("BossStateHUD");
	if (BossStateWindow) BossStateWindow->SetVisibility(false);
}

void CBossMonster::Start()
{
	CMonster::Start();

	SetAnimationEndNotify<CBossMonster>(MONSTER_BOSS_LEFT_DEATH , this, &CBossMonster::CharacterDestroy);
	SetAnimationEndNotify<CBossMonster>(MONSTER_BOSS_RIGHT_DEATH, this, &CBossMonster::CharacterDestroy);
}

bool CBossMonster::Init()
{
	if (!CMonster::Init()) return false;
	SetAnimation();

	m_HPBarWidget->SetPos(-25.f, -155.f);
	m_MPBarWidget->SetPos(-25.f, -145.f);

	CColliderBox* Body = (CColliderBox*)FindCollider("Body");
	Body->SetOffset(0.f, -60.5f);

	return true;
}

void CBossMonster::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);

	MissileUpdate(DeltaTime);
	GrenadeUpdate(DeltaTime);
	GeneratorUpdate(DeltaTime);
	UIUpdate(DeltaTime);
}

void CBossMonster::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

void CBossMonster::Collision(float DeltaTime)
{
	CMonster::Collision(DeltaTime);
}

void CBossMonster::Render(HDC hDC)
{
	CMonster::Render(hDC);
}

CBossMonster* CBossMonster::Clone()
{
	return new CBossMonster(*this);
}


// CUISelect* SelectWindow = CreateUIWindow<CUISelect>("MenuWindow");