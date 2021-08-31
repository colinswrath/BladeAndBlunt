class Settings
{
public:
	static Settings* GetSingleton();

	void LoadSettings();
	void LoadForms();

	RE::FormID IsAttackingSpellFormId;
	RE::FormID IsBlockingSpellFormId;
	RE::FormID IsSneakingSpellFormId;
	RE::FormID BowDrainStaminaFormId;

	RE::SpellItem* IsAttackingSpell;
	RE::SpellItem* IsBlockingSpell;
	RE::SpellItem* IsSneakingSpell;
	RE::SpellItem* BowDrainStaminaSpell;

	float BowDrainStamMagnitude;

	static std::uint32_t ParseUInt32(const std::string& str);

	std::string FileName;
	
};
