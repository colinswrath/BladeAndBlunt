#pragma once

//Credit: PAPER by Dennis Soemers used as reference
struct RecentHitEventData
{
	RecentHitEventData(RE::TESObjectREFR* target, RE::TESObjectREFR* cause, std::uint32_t applicationRuntime) :
		target(target), cause(cause), applicationRuntime(applicationRuntime) {}

	RE::TESObjectREFR* target;
	RE::TESObjectREFR* cause;
	std::uint32_t applicationRuntime;
};
