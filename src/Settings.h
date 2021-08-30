class Settings
{
public:
	static Settings* GetSingleton();

	void LoadSettings();
	void LoadForms();

	RE::FormID IsAttackingSpellFormId;
	RE::FormID IsBlockingSpellFormId;
	RE::FormID IsSneakingSpellFormId;

	RE::SpellItem* IsAttackingSpell;
	RE::SpellItem* IsBlockingSpell;
	RE::SpellItem* IsSneakingSpell;

	std::string FileName;
	
};
