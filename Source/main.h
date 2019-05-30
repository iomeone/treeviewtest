#pragma once


#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"
#include "TestLookAndFeel.h"
#include "jucer_Icons.h"

 

//==============================================================================
class TreeViewTestApplication : public JUCEApplication
{
public:
	//==============================================================================
	TreeViewTestApplication() {}

	const String getApplicationName() override { return ProjectInfo::projectName; }
	const String getApplicationVersion() override { return ProjectInfo::versionString; }
	bool moreThanOneInstanceAllowed() override { return true; }


	static TreeViewTestApplication& getApp()
	{
		TreeViewTestApplication* const app = dynamic_cast<TreeViewTestApplication*> (JUCEApplication::getInstance());
		jassert(app != nullptr);
		return *app;
	}


	//==============================================================================
	void initialise(const String& commandLine) override
	{
		icons.reset(new Icons());

		// This method is where you should put your application's initialisation code..

		mainWindow.reset(new MainWindow(getApplicationName()));
	}

	void shutdown() override
	{
		// Add your application's shutdown code here..

		mainWindow = nullptr; // (deletes our window)
	}

	//==============================================================================
	void systemRequestedQuit() override
	{
		// This is called when the app is being asked to quit: you can ignore this
		// request and let the app carry on running, or call quit() to allow the app to close.
		quit();
	}

	void anotherInstanceStarted(const String& commandLine) override
	{
		// When another instance of the app is launched while this one is running,
		// this method is invoked, and the commandLine parameter tells you what
		// the other instance's command-line arguments were.
	}
	std::unique_ptr<Icons> icons;

	class MainWindow : public DocumentWindow
	{
	public:
		MainWindow(String name) : DocumentWindow(name,
			Desktop::getInstance().getDefaultLookAndFeel()
			.findColour(ResizableWindow::backgroundColourId),
			DocumentWindow::allButtons),

			_v4lookAndFeel(new ProjucerLookAndFeel())

		{
			setUsingNativeTitleBar(true);
			setContentOwned(new MainComponent(), true);

#if JUCE_IOS || JUCE_ANDROID
			setFullScreen(true);
#else
			setResizable(true, true);
			centreWithSize(getWidth(), getHeight());
#endif


			Desktop::getInstance().setDefaultLookAndFeel(_v4lookAndFeel.get());


			setVisible(true);
		}

		void closeButtonPressed() override
		{
			// This is called when the user tries to close this window. Here, we'll just
			// ask the app to quit when this happens, but you can change this to do
			// whatever you need.
			JUCEApplication::getInstance()->systemRequestedQuit();
		}

		/* Note: Be careful if you override any DocumentWindow methods - the base
		   class uses a lot of them, so by overriding you might break its functionality.
		   It's best to do all your work in your content component instead, but if
		   you really have to override any DocumentWindow methods, make sure your
		   subclass also calls the superclass's method.
		*/

	private:

		std::unique_ptr<LookAndFeel_V4> _v4lookAndFeel;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
	};
private:
	std::unique_ptr<MainWindow> mainWindow;
};