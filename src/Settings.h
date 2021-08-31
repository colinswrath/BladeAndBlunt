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


	std::string FileName;
	
};
