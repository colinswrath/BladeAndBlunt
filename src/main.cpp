#include "Hooks.h"
#include "Settings.h"
#include "Cache.h"

void InitLogger()
{
#ifndef NDEBUG
	auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
	auto path = logger::log_directory();
	if (!path) {
		return;
	}

	*path /= fmt::format("{}.log"sv, Version::PROJECT);
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif

	auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

#ifndef NDEBUG
	log->set_level(spdlog::level::trace);
#else
	log->set_level(spdlog::level::info);
	log->flush_on(spdlog::level::warn);
#endif

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("%s(%#): [%^%l%$] %v"s);

	logger::info(FMT_STRING("{} v{}"), Version::PROJECT, Version::NAME);
}

extern "C" DLLEXPORT constexpr auto SKSEPlugin_Version =
[]() {
	SKSE::PluginVersionData v{};
	v.PluginVersion(Version::MAJOR);
	v.PluginName(Version::PROJECT);
	v.AuthorName("colinswrath and Kernalsegg"sv);
	v.UsesAddressLibrary(true);
	return v;
}();


extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface * a_skse, SKSE::PluginInfo * a_info)
{
	InitLogger();

	a_info->infoVersion = SKSE::PluginInfo::kVersion;
	a_info->name = Version::PROJECT.data();
	a_info->version = Version::MAJOR;

	if (a_skse->IsEditor()) {
		logger::critical("Loaded in editor, marking as incompatible"sv);
		return false;
	}

	const auto ver = a_skse->RuntimeVersion();
	if (ver < SKSE::RUNTIME_VR_1_4_15) {
		logger::critical(FMT_STRING("Unsupported runtime version {}"), ver.string());
		return false;
	}

	return true;
}

void InitListener(SKSE::MessagingInterface::Message* a_msg)
{
	switch (a_msg->type)
	{
	case SKSE::MessagingInterface::kNewGame:
		Settings::GetSingleton()->LoadForms();
		break;
	case SKSE::MessagingInterface::kDataLoaded:
		Settings::GetSingleton()->LoadForms();
		break;
	}
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	InitLogger();	//1.6

	SKSE::Init(a_skse);
	logger::info("Loading Blade and Blunt.");
	SKSE::AllocTrampoline(42);
	Cache::CacheAddLibAddresses();
	if (!Hooks::Install())
	{
		logger::error("Hook installation failed.");
		return false;
	}

	Settings::GetSingleton()->LoadSettings();

	auto messaging = SKSE::GetMessagingInterface();
	if (!messaging->RegisterListener(InitListener))
	{
		return false;
	}

	logger::info("Blade and Blunt loaded.");
	spdlog::default_logger()->flush();
	return true;
}
