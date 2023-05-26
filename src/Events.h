#pragma once
#include <Conditions.h>

//Credit: PAPER by Dennis Soemers used as reference
struct RecentHitEventData
{
	RecentHitEventData(RE::TESObjectREFR* target, RE::TESObjectREFR* cause, float applicationRuntime) :
		target(target), cause(cause), applicationRuntime(applicationRuntime) {}

	RE::TESObjectREFR* target;
	RE::TESObjectREFR* cause;
	float applicationRuntime;
};

class OnHitEventHandler : public RE::BSTEventSink<RE::TESHitEvent>
{
public:
	std::vector<RecentHitEventData> recentHits;

	static OnHitEventHandler* GetSingleton()
	{
		static OnHitEventHandler singleton;
		return &singleton;
	}

	RE::BSEventNotifyControl ProcessEvent(const RE::TESHitEvent* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESHitEvent>* a_eventSource) override
	{
		if (!a_event || !a_event->target || !a_event->cause) {
			return RE::BSEventNotifyControl::kContinue;
		}
		
		auto causeActor = a_event->cause->As<RE::Actor>();
		auto targetActor = a_event->target->As<RE::Actor>();

		if (causeActor && targetActor && targetActor->IsPlayerRef()) {
			const auto applicationRuntime = RE::GetDurationOfApplicationRunTime();

			bool skipEvent = false;
			size_t numToRemove = 0;
			for (const auto& recentHit : recentHits) {
				if (recentHit.applicationRuntime == applicationRuntime) {
					if (recentHit.cause == a_event->cause.get() && recentHit.target == targetActor) {
						skipEvent = true;
						break;
					}
				} else {
					++numToRemove;
				}
			}

			if (numToRemove > 0) {
				recentHits.erase(recentHits.begin(), recentHits.begin() + numToRemove);
			}

			if (!skipEvent) {			
				auto attackingWeapon = RE::TESForm::LookupByID<RE::TESObjectWEAP>(a_event->source);

				//Something is effed with power attacks. The source isnt coming through and casting as a weapon and the hit flags are empty
				//We can work around it like this
				bool powerAttackMelee = false;
				if (a_event->flags.any(RE::TESHitEvent::Flag::kPowerAttack) || Conditions::IsPowerAttacking(causeActor)) {
					bool rightIsMeleeWeapon = false;
					if (auto rightHandForm = causeActor->GetEquippedObject(false)) {
						if (rightHandForm->IsWeapon() && rightHandForm->As<RE::TESObjectWEAP>()->IsMelee()) {
							rightIsMeleeWeapon = true;
						}
					}

					bool LeftIsMeleeWeaponOrNone = false;
					auto leftHandForm = causeActor->GetEquippedObject(true);
					if (!leftHandForm || (leftHandForm->IsWeapon() && leftHandForm->As<RE::TESObjectWEAP>()->IsMelee()) || leftHandForm->IsArmor()) {
						LeftIsMeleeWeaponOrNone = true;
					}

					if (rightIsMeleeWeapon && LeftIsMeleeWeaponOrNone) {
						powerAttackMelee = true;
					}
				}

				if (Settings::GetSingleton()->enableInjuries) {
					RollForInjuryEvent();
				}

				auto leftHand = RE::PlayerCharacter::GetSingleton()->GetEquippedObject(true);

				bool blockedMeleeHit = false;
				if (!a_event->projectile && 
					((attackingWeapon && attackingWeapon->IsMelee()) || powerAttackMelee) &&
					(a_event->flags.any(RE::TESHitEvent::Flag::kHitBlocked) || targetActor->IsBlocking())) {
					blockedMeleeHit = true;
				}
				
				//Shield Stagger
				if (leftHand && leftHand->IsArmor() && blockedMeleeHit)
				{
					ProcessHitEventForBlockStagger(targetActor, causeActor);
				} else if (blockedMeleeHit) {
					//Parry
					ProcessHitEventForParry(targetActor,causeActor);
				}

				recentHits.emplace_back(targetActor, causeActor, applicationRuntime);
			}

		}

		return RE::BSEventNotifyControl::kContinue;
	}

	static void ProcessHitEventForParry(RE::Actor* target, RE::Actor* aggressor)
	{
		auto settings = Settings::GetSingleton();
		if (Conditions::PlayerHasActiveMagicEffect(settings->MAG_ParryWindowEffect)) {
			Conditions::ApplySpell(target, aggressor, settings->MAGParryStaggerSpell);
		}
	}

	static void ProcessHitEventForBlockStagger(RE::Actor* target, RE::Actor* aggressor)
	{
		auto settings = Settings::GetSingleton();
		if (target->HasPerk(settings->BlockStaggerPerk)) {
			Conditions::ApplySpell(target, aggressor, settings->MAGBlockStaggerSpell2);
		}
		else {
			Conditions::ApplySpell(target, aggressor, settings->MAGBlockStaggerSpell);
		}
	}

	static void RollForInjuryEvent() 
	{

		auto player = RE::PlayerCharacter::GetSingleton();
		auto health = Conditions::GetMaxHealth();
		auto settings = Settings::GetSingleton();

		//If health below 25% roll for injury
		if (player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kHealth) < health * 0.25f) {
			auto random = std::rand() % 100;

			if (random < settings->InjuryChance25Health->value) {
				ApplyInjury();
			}
		} else if (player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kHealth) < health * 0.5f) {
			//Roll rand for injury
			auto random = std::rand() % 100;

			if (random < settings->InjuryChance50Health->value) {
				ApplyInjury();
			}
		}
	}

	static void ApplyInjury()
	{
		auto player = RE::PlayerCharacter::GetSingleton();
		auto settings = Settings::GetSingleton();

		if (player->HasSpell(settings->InjurySpell1)) {
			if (!Conditions::PlayerHasActiveMagicEffect(settings->MAG_InjuryCooldown1)) {
				player->RemoveSpell(settings->InjurySpell1);
				player->AddSpell(settings->InjurySpell2);
			}
		} else if (player->HasSpell(settings->InjurySpell2)) {
			if (!Conditions::PlayerHasActiveMagicEffect(settings->MAG_InjuryCooldown2)) {
				player->RemoveSpell(settings->InjurySpell2);
				player->AddSpell(settings->InjurySpell3);
			}
		} else if (!player->HasSpell(settings->InjurySpell3)) {
			player->AddSpell(settings->InjurySpell1);
		}
	}

	static void Register()
	{
		RE::ScriptEventSourceHolder* eventHolder = RE::ScriptEventSourceHolder::GetSingleton();
		eventHolder->AddEventSink(OnHitEventHandler::GetSingleton());
	}
};

class WeaponFireHandler
{
public:
	static void InstallArrowReleaseHook() {
		logger::info("Writing arrow release handler hook");

		auto& trampoline = SKSE::GetTrampoline();
		_Weapon_Fire = trampoline.write_call<5>(Hooks::arrow_release_handler.address(), WeaponFire);
		
		logger::info("Release arrow hooked");
	}

	static void WeaponFire(RE::TESObjectWEAP* a_weapon, RE::TESObjectREFR* a_source, RE::TESAmmo* a_ammo, RE::EnchantmentItem* a_ammoEnchantment, RE::AlchemyItem* a_poison)
	{
		_Weapon_Fire(a_weapon, a_source, a_ammo, a_ammoEnchantment, a_poison);

		if (!a_source) {
			return;
		}

		auto source = a_source->As<RE::Actor>();

		if (source->IsPlayerRef() && a_weapon->IsCrossbow()) {
			Conditions::ApplySpell(source, source,Settings::GetSingleton()->MAGCrossbowStaminaDrainSpell);
		}
	}
	inline static REL::Relocation<decltype(WeaponFire)> _Weapon_Fire;
};

