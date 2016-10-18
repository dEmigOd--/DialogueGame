#pragma once

#include <vector>
#include <memory>
#include <functional>
#include "GameState.h"
#include "Utils.h"

class Menu
{
public:
	typedef std::pointer_to_binary_function<Utils&, GameState&, bool> OnCallCallback;
	typedef std::pointer_to_binary_function<Utils&, GameState&, bool> BeforeCallCallback;
	typedef std::pointer_to_binary_function<Utils&, GameState&, bool> AfterCallCallback;

private:
	std::string text;
	BeforeCallCallback BeforeMenuItem;
	OnCallCallback OnMenuItem;
	AfterCallCallback AfterMenuItem;
	std::vector<std::shared_ptr<Menu>> subMenus;

#define SUB_MENUS_PRESENT !subMenus.empty()

	bool PrintMenu(Utils& utils, GameState& state) const
	{
		utils.ClearScreen();

		if (NeedToPrintHeader()) 
		{
			std::cout << Text() << std::endl;
			utils.PrintEmptyLine();
		}

		if (SUB_MENUS_PRESENT)
		{
			int numSubMenus = PrintSubMenus();
			int userChoice;

			std::cin >> userChoice;

			// handle exitable menus
			if (userChoice > subMenus.size())
				return false;

			// on wrong input exit current menu
			if (userChoice < 1 || userChoice > numSubMenus)
				return false;

			bool runForever = true;

			while (runForever)
			{
				// process pre-requisites to action
				subMenus[userChoice - 1]->BeforeCall(utils, state);
				// act
				runForever = subMenus[userChoice - 1]->OnCall(utils, state);
				// process results
				subMenus[userChoice - 1]->AfterCall(utils, state);
			}

			return true;
		}

		// returning from the leaf
		return false;
	}

#undef SUB_MENUS_PRESENT

	class FunctionConverter
	{
	private:
		std::pointer_to_binary_function<Utils&, GameState&, void> callback;
	public:
		FunctionConverter(std::pointer_to_binary_function<Utils&, GameState&, void> simpleCallback)
			: callback(simpleCallback)
		{
		}

		bool operator()(Utils& utils, GameState& state)
		{
			callback(utils, state);
			return true;
		}
	};
protected:
	virtual int PrintSubMenus() const
	{
		int index = 0;
		for each (auto subMenu in subMenus)
		{
			std::cout << ++index << ". " << subMenu->Text() << std::endl;
		}

		return index;
	}

	virtual bool NeedToPrintHeader() const
	{
		return true;
	}

public:
	static bool DoNothing(Utils& utils, GameState& state)
	{
		return false;
	}

	Menu(const std::string& text, OnCallCallback onCall, 
		BeforeCallCallback beforeCall = Menu::BeforeCallCallback(Menu::DoNothing), 
		AfterCallCallback afterCall = Menu::AfterCallCallback(Menu::DoNothing))
		: text(text), OnMenuItem(onCall), BeforeMenuItem(beforeCall), AfterMenuItem(afterCall)
	{
	}

	std::string Text() const
	{
		return text;
	}

#define SHOULD_REPEAT_CALL(x) (x)
#define SHOULD_WAIT_FOR_USER(x) (x)

	bool BeforeCall(Utils& utils, GameState& state) const
	{
		utils.ClearScreen();

		if (SHOULD_WAIT_FOR_USER(BeforeMenuItem(utils, state)))
			utils.Pause();

		return false;
	}

	bool OnCall(Utils& utils, GameState& state) const
	{
		utils.ClearScreen();

		if(SHOULD_WAIT_FOR_USER(OnMenuItem(utils, state)))
			utils.Pause();

		return SHOULD_REPEAT_CALL(PrintMenu(utils, state));
	}

	bool AfterCall(Utils& utils, GameState& state) const
	{
		utils.ClearScreen();

		if (SHOULD_WAIT_FOR_USER(AfterMenuItem(utils, state)))
			utils.Pause();

		return false;
	}

	virtual void AddSubMenu(Menu* subMenu)
	{
		subMenus.push_back(std::shared_ptr<Menu>(subMenu));
	}
};

class DeepestMenu : public Menu
{
public:
	virtual bool NeedToPrintHeader() const
	{
		return false;
	}

	virtual void AddSubMenu(Menu* subMenu)
	{
		// checkout subMenu will be destroyed as not needed
		auto ensureSubMenuDestroyed = std::shared_ptr<Menu>(subMenu);
	}

public:
	DeepestMenu(const std::string& text, OnCallCallback onCall,
		BeforeCallCallback beforeCall = Menu::BeforeCallCallback(Menu::DoNothing),
		AfterCallCallback afterCall = Menu::AfterCallCallback(Menu::DoNothing))
		: Menu(text, onCall, beforeCall, afterCall)
	{}
};

class ExitableMenu : public Menu
{
protected:
	virtual int PrintSubMenus() const
	{
		int index = Menu::PrintSubMenus();

		std::cout << ++index << ". Exit" << std::endl;

		return index;
	}

public:
	ExitableMenu(const std::string& text, OnCallCallback onCall,
		BeforeCallCallback beforeCall = Menu::BeforeCallCallback(Menu::DoNothing),
		AfterCallCallback afterCall = Menu::AfterCallCallback(Menu::DoNothing))
		: Menu(text, onCall, beforeCall, afterCall)
	{}
};