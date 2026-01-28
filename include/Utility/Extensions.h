#pragma once
#include "Cache.h"
#include "Settings.h"

//Extensions file. Gradually move utility functions here and organize by type

namespace Actor
{
    inline static bool IsDrawingBow(RE::Actor* actor, bool crossbow = false)
    {
        auto attackState = actor->AsActorState()->GetAttackState();

        auto equippedWeapon = skyrim_cast<RE::TESObjectWEAP*>(actor->GetEquippedObject(false));
        if (!equippedWeapon) {
            return false;
        }

        RE::WEAPON_TYPE type = RE::WEAPON_TYPE::kBow;

        if (crossbow) {
            type = RE::WEAPON_TYPE::kCrossbow;
        }

        switch (attackState) {
        case RE::ATTACK_STATE_ENUM::kBowDrawn: {
            if (equippedWeapon->GetWeaponType() == type) {
                return true;
            }
            break;
        }
        case RE::ATTACK_STATE_ENUM::kBowAttached: {
            if (!crossbow && equippedWeapon->GetWeaponType() == RE::WEAPON_TYPE::kBow) {
                return true;
            }
            break;
        }
        default:
            break;
        }
        return false;
    }

    //Below are address cached versions of actor functions. This avoids address library lookup overhead. Not all actor functions need this

    static bool IsAttacking(RE::Actor* actor)
    {
        using func_t = decltype(&Actor::IsAttacking);
        REL::Relocation<func_t> func{ Cache::IsAttackingAddress };
        return func(actor);
    }
    inline static REL::Relocation<decltype(IsAttacking)> _IsAttacking;

    static bool IsBlocking(RE::Actor* actor)
    {
        using func_t = decltype(&Actor::IsBlocking);
        REL::Relocation<func_t> func{ Cache::IsBlockingAddress };
        return func(actor);
    }
    inline static REL::Relocation<decltype(IsBlocking)> _IsBlocking;

    static bool HasSpell(RE::Actor* actor, RE::SpellItem* spell)
    {
        using func_t = decltype(&Actor::HasSpell);

        REL::Relocation<func_t> func{ Cache::HasSpellAddress };

        return func(actor, spell);
    }
    inline static REL::Relocation<decltype(HasSpell)> _HasSpell;

    static bool RemoveSpell(RE::Actor* actor, RE::SpellItem* a_spell)
    {
        using func_t = decltype(&Actor::RemoveSpell);
        REL::Relocation<func_t> func{ Cache::RemoveSpellAddress };
        return func(actor, a_spell);
    }

    static bool IsCasting(RE::Actor* actor, RE::MagicItem* a_spell)
    {
        using func_t = decltype(&Actor::IsCasting);
        REL::Relocation<func_t> func{ Cache::IsCastingAddress };
        return func(actor, a_spell);
    }
}
