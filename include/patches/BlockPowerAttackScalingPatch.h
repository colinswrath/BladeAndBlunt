#pragma once

namespace BlockPowerAttackScalingPatch
{
    //26425
    float GetAttackDamageReduction(float f1, float f2, float f3, bool blockedWithWeapon1, bool blockedWithWeapon2, bool isPowerAttack, [[maybe_unused]] RE::Actor* actor)
    {
        //Still trying to figure out exactly what these are
        //f1 -> somehow related to attack. Maybe its attack damage?
        //f2 -> armor rating % reduction maybe?
        //f3 -> additional reduction mult?

        //We actually do not need the actor right now, but in the future if we want to change this more based on actor attributes we can

        //Havent figured out what weaponBlock2 is exactly, but blockedWithWeapon1 is set when you block with a weapon...suprise

        float percentBlocked = 0.0f;

		auto gameSettings = RE::GameSettingCollection::GetSingleton();
        static auto* SettingArMax                   = gameSettings->GetSetting("fArmorRatingMax");
        static auto* SettingArBase                  = gameSettings->GetSetting("fArmorRatingBase");
        static auto* SettingBlockWeapScaling        = gameSettings->GetSetting("fBlockWeaponScaling");
        static auto* SettingBlockWeapBase           = gameSettings->GetSetting("fBlockWeaponBase");
        static auto* SettingShieldScaleFactor       = gameSettings->GetSetting("fShieldScalingFactor");
        static auto* SettingShieldBaseFactor        = gameSettings->GetSetting("fShieldBaseFactor");
        static auto* SettingBlockPowerAttackMult    = gameSettings->GetSetting("fBlockPowerAttackMult");

        float arMax             = SettingArMax->GetFloat();
        float arBase            = SettingArBase->GetFloat();
        float weapScale         = SettingBlockWeapScaling->GetFloat();
        float weapBase          = SettingBlockWeapBase->GetFloat();
        float shieldScale       = SettingShieldScaleFactor->GetFloat();
        float shieldBase        = SettingShieldBaseFactor->GetFloat();
        float powerAttckMult    = SettingBlockPowerAttackMult->GetFloat();

        float armorFactor = ((arMax - arBase) * f2 * 0.01f) + arBase;

        if (blockedWithWeapon1 || blockedWithWeapon2) {
            percentBlocked = ((armorFactor * f1 * weapScale * 0.01f) + weapBase);
        }
        else
        {
            percentBlocked = (((armorFactor + f3) * f1 * shieldScale * 0.01f) + shieldBase);
            powerAttckMult += 0.25f; // If blocking with a shield increase power attack mult by 25%
        }

        if (isPowerAttack) {
            percentBlocked *= powerAttckMult;   //<- this is the same as percentBlocked = percentBlocked X powerAttackMult
        }
        
        return percentBlocked;
    }

    bool InstallBlockPowerAttackPatch()
    {
        struct patchOne : Xbyak::CodeGenerator
        {
            patchOne(std::uintptr_t returnAddress)
            {
                Xbyak::Label funcLabel;
                Xbyak::Label returnLabel;

                mov(r11, rsp);

                //New space on stack
                sub(rsp, 0x40);

                //Rebuild stack frame with new parameter
                mov(al, byte[r11 + 0x28]);
                mov(byte[rsp + 0x28], al);
                mov(al, byte[r11 + 0x30]);
                mov(byte[rsp + 0x30], al);
                mov(ptr[rsp + 0x38], rbx);
                call(ptr[rip + funcLabel]);

                // Restore stack
                add(rsp, 0x40);

                movss(ptr[r12], xmm0);

                jmp(ptr[rip + returnLabel]);
                L(funcLabel);
                dq(reinterpret_cast<std::uintptr_t>(GetAttackDamageReduction));

                L(returnLabel);
                dq(returnAddress);
            }
        };

        patchOne code1{ Hooks::hitDataPop.address() + REL::Relocate(0x442,0x428) };

        auto& trampoline = SKSE::GetTrampoline();
        trampoline.write_branch<6>(Hooks::hitDataPop.address() + REL::Relocate(0x437,0x41D), trampoline.allocate(code1));
        logger::info("Block power attack hook installed");
        return true;
    }
}
