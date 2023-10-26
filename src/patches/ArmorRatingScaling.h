#pragma once

namespace ArmorRatingScaling
{
	float AdjustArmorRating(float a_vanilla)
	{
		auto factor = RE::GameSettingCollection::GetSingleton()->GetSetting("fArmorScalingFactor")->GetFloat();
		auto armorRating = (a_vanilla / factor) * 100;

		if (armorRating <= 500) {
			return a_vanilla;
		} else if (armorRating < 1000) {
			auto remainderRating = armorRating - 500;
			return 0.75f + (remainderRating / 100 * 0.03f);
		} else {
			return 0.90f;
		}
	}

	bool InstallArmorRatingHookAE()
	{
		struct ratingPatch : Xbyak::CodeGenerator
		{
			ratingPatch(std::uintptr_t returnAddress)
			{
				Xbyak::Label funcLabel;
				Xbyak::Label returnLabel;

				movss(xmm0, ptr[rbp + 0x77]);
				sub(rsp, 0x20);
				call(ptr[rip + funcLabel]);
				add(rsp, 0x20);
				movss(ptr[rbp + 0x77], xmm0);

				jmp(ptr[rip + returnLabel]);

				L(funcLabel);
				dq(reinterpret_cast<std::uintptr_t>(AdjustArmorRating));

				L(returnLabel);
				dq(returnAddress);
			}
		};

		struct ratingPatch2 : Xbyak::CodeGenerator
		{
			ratingPatch2(std::uintptr_t returnAddress)
			{
				Xbyak::Label funcLabel;
				Xbyak::Label returnLabel;

				movss(xmm0, xmm7);
				sub(rsp, 0x20);
				call(ptr[rip + funcLabel]);
				add(rsp, 0x20);
				movss(xmm7, xmm0);

				jmp(ptr[rip + returnLabel]);

				L(funcLabel);
				dq(reinterpret_cast<std::uintptr_t>(AdjustArmorRating));

				L(returnLabel);
				dq(returnAddress);
			}
		};

		ratingPatch code1{ Hooks::armorRating1.address() + 0x10D };
		ratingPatch2 code2{ Hooks::armorRating2.address() + 0x94 };

		auto& trampoline = SKSE::GetTrampoline();
		trampoline.write_branch<6>(Hooks::armorRating1.address() + 0xF9, trampoline.allocate(code1));
		trampoline.write_branch<6>(Hooks::armorRating2.address() + 0x7A, trampoline.allocate(code2));
		logger::info("armor rating AE hook installed");
		return true;
	}

	bool InstallArmorRatingHookSE()
	{
		struct ratingPatch : Xbyak::CodeGenerator
		{
			ratingPatch(std::uintptr_t returnAddress)
			{
				Xbyak::Label funcLabel;
				Xbyak::Label returnLabel;

				movss(xmm0, ptr[rbp + 0x77]);
				sub(rsp, 0x20);
				call(ptr[rip + funcLabel]);
				add(rsp, 0x20);
				movss(ptr[rbp + 0x77], xmm0);

				jmp(ptr[rip + returnLabel]);

				L(funcLabel);
				dq(reinterpret_cast<std::uintptr_t>(AdjustArmorRating));

				L(returnLabel);
				dq(returnAddress);
			}
		};

		struct ratingPatch2 : Xbyak::CodeGenerator
		{
			ratingPatch2(std::uintptr_t returnAddress)
			{
				Xbyak::Label funcLabel;
				Xbyak::Label returnLabel;

				movss(xmm0, xmm8);
				sub(rsp, 0x20);
				call(ptr[rip + funcLabel]);
				add(rsp, 0x20);
				movss(xmm8, xmm0);

				jmp(ptr[rip + returnLabel]);

				L(funcLabel);
				dq(reinterpret_cast<std::uintptr_t>(AdjustArmorRating));

				L(returnLabel);
				dq(returnAddress);
			}
		};

		ratingPatch code1{ Hooks::armorRating1.address() + 0x115 };
		ratingPatch2 code2{ Hooks::armorRating2.address() + 0x7C };

		auto& trampoline = SKSE::GetTrampoline();
		trampoline.write_branch<6>(Hooks::armorRating1.address() + 0x101, trampoline.allocate(code1));
		trampoline.write_branch<6>(Hooks::armorRating2.address() + 0x99, trampoline.allocate(code2));
		logger::info("armor rating SE hook installed");
		return true;
	}
}
