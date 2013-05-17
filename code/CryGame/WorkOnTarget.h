/*************************************************************************
Crytek Source File.
Copyright (C), Crytek Studios, 2001-2006.
-------------------------------------------------------------------------
$Id$
$DateTime$
Description: Work Firemode Implementation

-------------------------------------------------------------------------
History:
- 05:08:2007:		Created by Marcio Martins

*************************************************************************/
#ifndef __WORKONTARGET_H__
#define __WORKONTARGET_H__

#if _MSC_VER > 1000
# pragma once
#endif

#include "Weapon.h"
#include "ItemParamReader.h"
#include "Single.h"

#define ResetValue(name, defaultValue) if (defaultInit) name=defaultValue; reader.Read(#name, name)
#define ResetValueEx(name, var, defaultValue) if (defaultInit) var=defaultValue; reader.Read(name, var)


class CWorkOnTarget;
class CWorkOnTarget : public IFireMode
{
		struct SWorkParams
		{
			SWorkParams()
			{
				Reset();
			};
			void Reset(const IItemParamsNode *params = 0, bool defaultInit = true)
			{
				CItemParamReader reader(params);
				ResetValue(range,			2.0f);
				ResetValue(delay,			0.5f);
				ResetValue(amount,		200.0f);
				ResetValue(work_type,	"");
				working_effect.Reset(params ? params->GetChild("effect") : 0, defaultInit);
			}

			void GetMemoryUsage(ICrySizer *s) const
			{
			}

			float				range;
			float				delay;
			float				amount;
			ItemString	work_type;

			CSingle::SEffectParams	working_effect;
		};

		struct SWorkActions
		{
			SWorkActions()
			{
				Reset();
			};
			void Reset(const IItemParamsNode *params = 0, bool defaultInit = true)
			{
				CItemParamReader reader(params);
				ResetValue(work,			"work");
				ResetValue(idle,			"idle");
				ResetValue(prefire,		"prefire");
				ResetValue(postfire,	"postfire");
			}

			void GetMemoryUsage(ICrySizer *s) const
			{
				s->Add(work);
				s->Add(idle);
				s->Add(prefire);
				s->Add(postfire);
			}

			ItemString	work;
			ItemString	idle;
			ItemString	prefire;
			ItemString	postfire;
		};

	public:
		CWorkOnTarget();
		virtual ~CWorkOnTarget();

		//IFireMode
		virtual void Init(IWeapon *pWeapon, const struct IItemParamsNode *params, uint32 id);
		virtual void PostInit() {}
		virtual void Update(float frameTime, uint32 frameId);
		virtual void PostUpdate(float frameTime) {};
		virtual void UpdateFPView(float frameTime) {};
		virtual void Release();
		virtual void GetMemoryUsage(ICrySizer *s) const;

		virtual void ResetParams(const struct IItemParamsNode *params);
		virtual void PatchParams(const struct IItemParamsNode *patch);
		virtual void ModifyParams(bool modify, bool modified /* = true */) {}

		virtual void Activate(bool activate);

		virtual int GetAmmoCount() const
		{
			return 0;
		};
		virtual int GetClipSize() const
		{
			return 0;
		};

		virtual bool OutOfAmmo() const
		{
			return false;
		};
		virtual bool LowAmmo(float) const
		{
			return false;
		}; // TODO: Implement properly if required. This is for Crysis2 compatibility.
		virtual bool CanReload() const
		{
			return false;
		};
		virtual void Reload(int zoomed) {};
		virtual bool IsReloading(bool includePending = true)
		{
			return false;
		};
		virtual void CancelReload() {};
		virtual bool CanCancelReload()
		{
			return false;
		};

		virtual bool AllowZoom() const
		{
			return true;
		};
		virtual void Cancel() {};

		virtual float GetRecoil() const
		{
			return 0.0f;
		};
		virtual float GetSpread() const
		{
			return 0.0f;
		};
		virtual float GetSpreadForHUD() const
		{
			return 0.0f;
		};
		virtual float GetMinSpread() const
		{
			return 0.0f;
		};
		virtual float GetMaxSpread() const
		{
			return 0.0f;
		};
		virtual const char *GetCrosshair() const
		{
			return "";
		};
		virtual float GetHeat() const
		{
			return 0.0f;
		};
		virtual bool	CanOverheat() const
		{
			return false;
		};

		virtual bool CanFire(bool considerAmmo = true) const;
		virtual void StartFire();
		virtual void StopFire();
		virtual bool IsFiring() const
		{
			return m_working || m_firing;
		};
		virtual bool IsSilenced() const
		{
			return false;    // compile fix, no guarantee of functionality
		}
		virtual void NetShoot(const Vec3 &hit, int ph);
		virtual void NetShootEx(const Vec3 &pos, const Vec3 &dir, const Vec3 &vel, const Vec3 &hit, float extra, int ph);
		virtual void NetEndReload() {};

		virtual void NetStartFire();
		virtual void NetStopFire();

		virtual void Shoot(const Vec3 &pos, const Vec3 &dir, const Vec3 &vel);

		virtual EntityId GetProjectileId() const
		{
			return 0;
		};
		virtual void SetProjectileId(EntityId id) {};
		virtual EntityId RemoveProjectileId()
		{
			return 0;
		};

		virtual const char *GetType() const;
		virtual IEntityClass *GetAmmoType() const
		{
			return 0;
		};
		virtual int GetDamage() const
		{
			return 0;
		};

		virtual float GetSpinUpTime() const
		{
			return 0.0f;
		};
		virtual float GetSpinDownTime() const
		{
			return 0.0f;
		};
		virtual float GetNextShotTime() const
		{
			return 0.0f;
		};
		virtual void SetNextShotTime(float time) {};
		virtual float GetFireRate() const
		{
			return 0.0f;
		};

		virtual void Enable(bool enable)
		{
			m_enabled = enable;
		};
		virtual bool IsEnabled() const
		{
			return m_enabled;
		};

		virtual void SetSecondary(bool secondary)
		{
			m_secondary = secondary;
		}
		virtual bool IsSecondary() const
		{
			return m_secondary;
		}

		virtual Vec3 GetFiringPos(const Vec3 &probableHit) const
		{
			return ZERO;
		}
		virtual Vec3 GetFiringDir(const Vec3 &probableHit, const Vec3 &firingPos) const
		{
			return ZERO;
		}
		virtual void SetName(const char *name)
		{
			m_name = name;
		};
		virtual const char *GetName()const
		{
			return m_name.empty() ? 0 : m_name.c_str();
		};

		virtual bool HasFireHelper() const
		{
			return false;
		}
		virtual Vec3 GetFireHelperPos() const
		{
			return Vec3(ZERO);
		}
		virtual Vec3 GetFireHelperDir() const
		{
			return FORWARD_DIRECTION;
		}

		virtual int GetCurrentBarrel() const
		{
			return 0;
		}
		virtual void Serialize(TSerialize ser) {};
		virtual void PostSerialize() {};

		virtual void SetRecoilMultiplier(float recoilMult) { }
		virtual float GetRecoilMultiplier() const
		{
			return 1.0f;
		}

		virtual void PatchSpreadMod(const SSpreadModParams &sSMP) {};
		virtual void ResetSpreadMod() {};

		virtual void PatchRecoilMod(const SRecoilModParams &sRMP) {};
		virtual void ResetRecoilMod() {};

		virtual void ResetLock() {};
		virtual void StartLocking(EntityId targetId, int partId) {};
		virtual void Lock(EntityId targetId, int partId) {};
		virtual void Unlock() {};
		virtual void OnZoomStateChanged() {} // compile fix, no guarantee of functionality
		//~IFireMode

	protected:
		IEntity			*CanWork();
		void				StartWork(IEntity *pEntity);
		void				StopWork();
		bool				WorkOnTarget(IEntity *pEntity, float frameTime);

		CWeapon			*m_pWeapon;

		bool				m_enabled;
		bool				m_secondary;
		bool				m_working;
		bool				m_firing;
		float				m_delayTimer;
		EntityId		m_lastTargetId;

		int					m_effectId;
		tSoundID		m_soundId;

		ItemString			m_name;

		SWorkParams		m_workparams;
		SWorkActions	m_workactions;
};


#endif //__WORKONTARGET_H__