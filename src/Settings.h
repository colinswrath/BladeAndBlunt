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

	float BowStaminaRatePerSec;

	static RE::FormID ParseFormID(const std::string& str);

	std::string FileName;
	
};
