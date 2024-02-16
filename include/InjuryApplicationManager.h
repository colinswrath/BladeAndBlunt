#pragma once

#include <RecentHitEventData.h>
#include <Conditions.h>
#include <Settings.h>

class InjuryApplicationManager
{
public:
	std::multimap<std::uint32_t, RecentHitEventData> recentInjuryRolls;
	
	static InjuryApplicationManager* GetSingleton()
	{
		static InjuryApplicationManager appHandler;
		return &appHandler;
	}

	void ProcessHitInjuryApplication(RE::Actor* cause, RE::Actor* target, uint32_t runtime, bool targetBlocking)
	{
		auto settings = Settings::GetSingleton();
		if (((settings->enableInjuries && !settings->SMOnlyEnableInjuries) ||
			(settings->enableInjuries && settings->SMOnlyEnableInjuries && Conditions::IsSurvivalEnabled()))) {

			uint32_t roundedRunTime = RoundRunTime(runtime);
			if (!ShouldSkipInjuryRoll(cause, target, roundedRunTime)) {
				RollForInjuryEvent(targetBlocking ? 0.50f : 1.0f);
				recentInjuryRolls.insert(std::make_pair(roundedRunTime, RecentHitEventData(target, cause, roundedRunTime)));
			}
		}
	}

private:

	//this is separate from the "ShouldSkipHitEvent" because this is meant to track injury roll hits within the last second.
	//"ShouldSkipHitEvent" tracks duplicate events that happen at the same time (ie. hits from enchanted weapons trigger multiple hits)
	bool ShouldSkipInjuryRoll(RE::Actor* causeActor, RE::Actor* targetActor, std::uint32_t runTime)
	{
		bool skipEvent = false;

		auto secondRuntime = recentInjuryRolls.lower_bound(runTime - 1000);
		//Loop over all hits within the last 1 second of runtime
		for (auto it = secondRuntime; it != recentInjuryRolls.end(); ++it) {
			if (it->second.cause == causeActor && it->second.target == targetActor) {
				skipEvent = true;
				break;
			}
		}

		//Remove all events older than 1 second from runtime
		auto it = recentInjuryRolls.begin();
		while (it != secondRuntime) {
			it = recentInjuryRolls.erase(it);
		}

		return skipEvent;
	}

	static void RollForInjuryEvent(float chanceMult = 1.0f)
	{
		auto player = RE::PlayerCharacter::GetSingleton();
		auto health = Conditions::GetMaxHealth();
		auto settings = Settings::GetSingleton();

		//If health below 25% roll for injury
		if (player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kHealth) < health * 0.25f) {
			auto random = std::rand() % 100;

			if (random < settings->InjuryChance25Health->value * chanceMult) {
				ApplyInjury();
			}
		} else if (player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kHealth) < health * 0.5f) {
			//Roll rand for injury
			auto random = std::rand() % 100;

			if (random < settings->InjuryChance50Health->value * chanceMult) {
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

	uint32_t RoundRunTime(uint32_t runTime)
	{
		auto flooredTime = static_cast<uint32_t>((runTime * 0.01f) + 0.5f);
		return flooredTime * 100;
	}
};
