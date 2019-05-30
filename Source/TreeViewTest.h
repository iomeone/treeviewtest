#pragma once


#include "../JuceLibraryCode/JuceHeader.h"
#include "jucer_CommonHeaders.h"

class JucerTreeViewBase : public TreeViewItem,
	public TooltipClient
{
public:
	JucerTreeViewBase();
	~JucerTreeViewBase() override;

	int getItemWidth() const override { return -1; }
	int getItemHeight() const override { return 25; }

	void paintOpenCloseButton(Graphics&, const Rectangle<float>& area, Colour backgroundColour, bool isMouseOver) override;
	void paintItem(Graphics& g, int width, int height) override;
	void itemClicked(const MouseEvent& e) override;
	void itemSelectionChanged(bool isNowSelected) override;
	void itemDoubleClicked(const MouseEvent&) override;
	Component* createItemComponent() override;
	String getTooltip() override { return {}; }

	void cancelDelayedSelectionTimer();

	//==============================================================================
	virtual bool isRoot() const { return false; }
	virtual Font getFont() const;
	virtual String getRenamingName() const = 0;
	virtual String getDisplayName() const = 0;
	virtual void setName(const String& newName) = 0;
	virtual bool isMissing() const = 0;
	virtual bool hasWarnings() const { return false; }
	virtual Icon getIcon() const = 0;
	virtual bool isIconCrossedOut() const { return false; }
	virtual void paintIcon(Graphics& g, Rectangle<float> area);
	virtual void paintContent(Graphics& g, Rectangle<int> area);
	virtual int getRightHandButtonSpace() { return 0; }
	virtual Colour getContentColour(bool isIcon) const;
	virtual int getMillisecsAllowedForDragGesture() { return 120; }
	virtual File getDraggableFile() const { return {}; }

	void refreshSubItems();
	virtual void deleteItem();
	virtual void deleteAllSelectedItems();
	virtual void showDocument();
	virtual void showMultiSelectionPopupMenu();
	virtual void showRenameBox();

	void launchPopupMenu(PopupMenu&); // runs asynchronously, and produces a callback to handlePopupMenuResult().
	virtual void showPopupMenu();
	virtual void showAddMenu();
	virtual void handlePopupMenuResult(int resultCode);
	virtual void setSearchFilter(const String&) {}

	//==============================================================================
	// To handle situations where an item gets deleted before openness is
	// restored for it, this OpennessRestorer keeps only a pointer to the
	// topmost tree item.
	struct WholeTreeOpennessRestorer : public OpennessRestorer
	{
		WholeTreeOpennessRestorer(TreeViewItem& item) : OpennessRestorer(getTopLevelItem(item))
		{}

	private:
		static TreeViewItem& getTopLevelItem(TreeViewItem& item)
		{
			if (TreeViewItem* const p = item.getParentItem())
				return getTopLevelItem(*p);

			return item;
		}
	};

	int textX;

protected:
	//ProjectContentComponent* getProjectContentComponent() const;
	virtual void addSubItems() {}

private:
	class ItemSelectionTimer;
	friend class ItemSelectionTimer;
	std::unique_ptr<Timer> delayedSelectionTimer;

	void invokeShowDocument();

	JUCE_DECLARE_WEAK_REFERENCEABLE(JucerTreeViewBase)
};



class TreeItemComponent : public Component
{
public:
	TreeItemComponent(JucerTreeViewBase& i) : item(i)
	{
		setInterceptsMouseClicks(false, true);
		item.textX = iconWidth;
	}

	void paint(Graphics& g) override
	{
		auto bounds = getLocalBounds().toFloat();
		auto iconBounds = bounds.removeFromLeft((float)iconWidth).reduced(7, 5);

		bounds.removeFromRight(buttons.size() * bounds.getHeight());

		item.paintIcon(g, iconBounds);
		item.paintContent(g, bounds.toNearestInt());
	}

	void resized() override
	{
		auto r = getLocalBounds();

		for (int i = buttons.size(); --i >= 0;)
			buttons.getUnchecked(i)->setBounds(r.removeFromRight(r.getHeight()));
	}

	void addRightHandButton(Component* button)
	{
		buttons.add(button);
		addAndMakeVisible(button);
	}

	JucerTreeViewBase& item;
	OwnedArray<Component> buttons;

	const int iconWidth = 25;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TreeItemComponent)
};


namespace Ids
{
#define DECLARE_ID(name)      const Identifier name (#name)

	DECLARE_ID(name);
	DECLARE_ID(file);
	DECLARE_ID(path);
	DECLARE_ID(text);
	DECLARE_ID(vendor);
	DECLARE_ID(version);
	DECLARE_ID(license);
	DECLARE_ID(minimumCppStandard);
	DECLARE_ID(include);
	DECLARE_ID(info);
	DECLARE_ID(description);
	DECLARE_ID(companyName);
	DECLARE_ID(companyCopyright);
	DECLARE_ID(companyWebsite);
	DECLARE_ID(companyEmail);
	DECLARE_ID(displaySplashScreen);
	DECLARE_ID(reportAppUsage);
	DECLARE_ID(splashScreenColour);
	DECLARE_ID(position);
	DECLARE_ID(source);
	DECLARE_ID(width);
	DECLARE_ID(height);
	DECLARE_ID(bounds);
	DECLARE_ID(background);
	DECLARE_ID(initialState);
	DECLARE_ID(targetFolder);
	DECLARE_ID(intermediatesPath);
	DECLARE_ID(modulePaths);
	DECLARE_ID(searchpaths);
	DECLARE_ID(osxFallback);
	DECLARE_ID(windowsFallback);
	DECLARE_ID(linuxFallback);
	DECLARE_ID(jucePath);
	DECLARE_ID(defaultJuceModulePath);
	DECLARE_ID(defaultUserModulePath);
	DECLARE_ID(vstLegacyFolder);
	DECLARE_ID(vst3Folder);
	DECLARE_ID(rtasFolder);
	DECLARE_ID(auFolder);
	DECLARE_ID(vstLegacyPath);
	DECLARE_ID(vst3Path);
	DECLARE_ID(rtasPath);
	DECLARE_ID(aaxPath);
	DECLARE_ID(flags);
	DECLARE_ID(line);
	DECLARE_ID(index);
	DECLARE_ID(type);
	DECLARE_ID(time);
	DECLARE_ID(extraCompilerFlags);
	DECLARE_ID(extraLinkerFlags);
	DECLARE_ID(externalLibraries);
	DECLARE_ID(extraDefs);
	DECLARE_ID(projectType);
	DECLARE_ID(isDebug);
	DECLARE_ID(alwaysGenerateDebugSymbols);
	DECLARE_ID(targetName);
	DECLARE_ID(binaryPath);
	DECLARE_ID(optimisation);
	DECLARE_ID(defines);
	DECLARE_ID(headerPath);
	DECLARE_ID(systemHeaderPath);
	DECLARE_ID(liveWindowsTargetPlatformVersion);
	DECLARE_ID(libraryPath);
	DECLARE_ID(customXcodeFlags);
	DECLARE_ID(customXcassetsFolder);
	DECLARE_ID(customLaunchStoryboard);
	DECLARE_ID(customXcodeResourceFolders);
	DECLARE_ID(plistPreprocessorDefinitions);
	DECLARE_ID(customPList);
	DECLARE_ID(pListPrefixHeader);
	DECLARE_ID(pListPreprocess);
	DECLARE_ID(UIFileSharingEnabled);
	DECLARE_ID(UISupportsDocumentBrowser);
	DECLARE_ID(UIStatusBarHidden);
	DECLARE_ID(documentExtensions);
	DECLARE_ID(keepCustomXcodeSchemes);
	DECLARE_ID(useHeaderMap);
	DECLARE_ID(cppLanguageStandard);
	DECLARE_ID(enableGNUExtensions);
	DECLARE_ID(cppLibType);
	DECLARE_ID(codeSigningIdentity);
	DECLARE_ID(fastMath);
	DECLARE_ID(linkTimeOptimisation);
	DECLARE_ID(vstBinaryLocation);
	DECLARE_ID(vst3BinaryLocation);
	DECLARE_ID(auBinaryLocation);
	DECLARE_ID(rtasBinaryLocation);
	DECLARE_ID(aaxBinaryLocation);
	DECLARE_ID(unityPluginBinaryLocation);
	DECLARE_ID(enablePluginBinaryCopyStep);
	DECLARE_ID(stripLocalSymbols);
	DECLARE_ID(osxSDK);
	DECLARE_ID(osxCompatibility);
	DECLARE_ID(osxArchitecture);
	DECLARE_ID(iosCompatibility);
	DECLARE_ID(xcodeSubprojects);
	DECLARE_ID(extraFrameworks);
	DECLARE_ID(frameworkSearchPaths);
	DECLARE_ID(extraCustomFrameworks);
	DECLARE_ID(embeddedFrameworks);
	DECLARE_ID(extraDLLs);
	DECLARE_ID(winArchitecture);
	DECLARE_ID(winWarningLevel);
	DECLARE_ID(msvcManifestFile);
	DECLARE_ID(warningsAreErrors);
	DECLARE_ID(linuxArchitecture);
	DECLARE_ID(linuxCodeBlocksArchitecture);
	DECLARE_ID(windowsCodeBlocksArchitecture);
	DECLARE_ID(codeBlocksWindowsTarget);
	DECLARE_ID(toolset);
	DECLARE_ID(windowsTargetPlatformVersion);
	DECLARE_ID(debugInformationFormat);
	DECLARE_ID(IPPLibrary);
	DECLARE_ID(msvcModuleDefinitionFile);
	DECLARE_ID(bigIcon);
	DECLARE_ID(smallIcon);
	DECLARE_ID(jucerVersion);
	DECLARE_ID(prebuildCommand);
	DECLARE_ID(postbuildCommand);
	DECLARE_ID(generateManifest);
	DECLARE_ID(useRuntimeLibDLL);
	DECLARE_ID(multiProcessorCompilation);
	DECLARE_ID(enableIncrementalLinking);
	DECLARE_ID(bundleIdentifier);
	DECLARE_ID(aaxIdentifier);
	DECLARE_ID(aaxFolder);
	DECLARE_ID(compile);
	DECLARE_ID(noWarnings);
	DECLARE_ID(resource);
	DECLARE_ID(xcodeResource);
	DECLARE_ID(className);
	DECLARE_ID(classDesc);
	DECLARE_ID(controlPoint);
	DECLARE_ID(createCallback);
	DECLARE_ID(parentClasses);
	DECLARE_ID(constructorParams);
	DECLARE_ID(objectConstructionArgs);
	DECLARE_ID(memberInitialisers);
	DECLARE_ID(canBeAggregated);
	DECLARE_ID(rootItemVisible);
	DECLARE_ID(openByDefault);
	DECLARE_ID(locked);
	DECLARE_ID(tooltip);
	DECLARE_ID(memberName);
	DECLARE_ID(markerName);
	DECLARE_ID(focusOrder);
	DECLARE_ID(hidden);
	DECLARE_ID(useStdCall);
	DECLARE_ID(useGlobalPath);
	DECLARE_ID(showAllCode);
	DECLARE_ID(useLocalCopy);
	DECLARE_ID(overwriteOnSave);
	DECLARE_ID(hardenedRuntime);
	DECLARE_ID(hardenedRuntimeOptions);
	DECLARE_ID(microphonePermissionNeeded);
	DECLARE_ID(microphonePermissionsText);
	DECLARE_ID(cameraPermissionNeeded);
	DECLARE_ID(cameraPermissionText);
	DECLARE_ID(androidJavaLibs);
	DECLARE_ID(androidAdditionalJavaFolders);
	DECLARE_ID(androidAdditionalResourceFolders);
	DECLARE_ID(androidRepositories);
	DECLARE_ID(androidDependencies);
	DECLARE_ID(androidBuildConfigRemoteNotifsConfigFile);
	DECLARE_ID(androidAdditionalXmlValueResources);
	DECLARE_ID(androidAdditionalDrawableResources);
	DECLARE_ID(androidAdditionalRawValueResources);
	//    DECLARE_ID (androidActivityClass);  // DEPRECATED!
	const Identifier androidCustomActivityClass("androidActivitySubClassName"); // old name is very confusing, but we need to remain backward compatible
//    DECLARE_ID (androidActivityBaseClassName); // DEPRECATED!
	DECLARE_ID(androidCustomApplicationClass);
	DECLARE_ID(androidVersionCode);
	DECLARE_ID(androidSDKPath);
	DECLARE_ID(androidNDKPath);
	DECLARE_ID(androidOboeRepositoryPath);
	DECLARE_ID(androidInternetNeeded);
	DECLARE_ID(androidArchitectures);
	DECLARE_ID(androidManifestCustomXmlElements);
	DECLARE_ID(androidCustomStringXmlElements);
	DECLARE_ID(androidBluetoothNeeded);
	DECLARE_ID(androidExternalReadNeeded);
	DECLARE_ID(androidExternalWriteNeeded);
	DECLARE_ID(androidInAppBilling);
	DECLARE_ID(androidVibratePermissionNeeded);
	DECLARE_ID(androidEnableRemoteNotifications);
	DECLARE_ID(androidRemoteNotificationsConfigFile);
	DECLARE_ID(androidEnableContentSharing);
	DECLARE_ID(androidMinimumSDK);
	DECLARE_ID(androidTargetSDK);
	DECLARE_ID(androidOtherPermissions);
	DECLARE_ID(androidKeyStore);
	DECLARE_ID(androidKeyStorePass);
	DECLARE_ID(androidKeyAlias);
	DECLARE_ID(androidKeyAliasPass);
	DECLARE_ID(androidTheme);
	DECLARE_ID(androidStaticLibraries);
	DECLARE_ID(androidSharedLibraries);
	DECLARE_ID(androidScreenOrientation);
	DECLARE_ID(androidExtraAssetsFolder);
	DECLARE_ID(androidStudioExePath);
	DECLARE_ID(iosDeviceFamily);
	const Identifier iPhoneScreenOrientation("iosScreenOrientation"); // old name is confusing
	DECLARE_ID(iPadScreenOrientation);
	DECLARE_ID(iosScreenOrientation);
	DECLARE_ID(iosInAppPurchases);
	DECLARE_ID(iosBackgroundAudio);
	DECLARE_ID(iosBackgroundBle);
	DECLARE_ID(iosPushNotifications);
	DECLARE_ID(iosAppGroups);
	DECLARE_ID(iCloudPermissions);
	DECLARE_ID(iosDevelopmentTeamID);
	DECLARE_ID(iosAppGroupsId);
	DECLARE_ID(duplicateAppExResourcesFolder);
	DECLARE_ID(buildToolsVersion);
	DECLARE_ID(gradleVersion);
	const Identifier androidPluginVersion("gradleWrapperVersion"); // old name is very confusing, but we need to remain backward compatible
	DECLARE_ID(gradleToolchain);
	DECLARE_ID(gradleToolchainVersion);
	DECLARE_ID(linuxExtraPkgConfig);
	DECLARE_ID(clionMakefileEnabled);
	DECLARE_ID(clionXcodeEnabled);
	DECLARE_ID(clionCodeBlocksEnabled);
	DECLARE_ID(clionExePath);
	DECLARE_ID(font);
	DECLARE_ID(colour);
	DECLARE_ID(userNotes);
	DECLARE_ID(maxBinaryFileSize);
	DECLARE_ID(includeBinaryInJuceHeader);
	DECLARE_ID(binaryDataNamespace);
	DECLARE_ID(characterSet);
	DECLARE_ID(JUCERPROJECT);
	DECLARE_ID(MAINGROUP);
	DECLARE_ID(EXPORTFORMATS);
	DECLARE_ID(GROUP);
	DECLARE_ID(FILE);
	DECLARE_ID(MODULES);
	DECLARE_ID(MODULE);
	DECLARE_ID(JUCEOPTIONS);
	DECLARE_ID(CONFIGURATIONS);
	DECLARE_ID(CONFIGURATION);
	DECLARE_ID(MODULEPATHS);
	DECLARE_ID(MODULEPATH);
	DECLARE_ID(PATH);
	DECLARE_ID(userpath);
	DECLARE_ID(systempath);
	DECLARE_ID(utilsCppInclude);
	DECLARE_ID(juceModulesFolder);
	DECLARE_ID(parentActive);
	DECLARE_ID(message);
	DECLARE_ID(start);
	DECLARE_ID(end);
	DECLARE_ID(range);
	DECLARE_ID(location);
	DECLARE_ID(key);
	DECLARE_ID(list);
	DECLARE_ID(METADATA);
	DECLARE_ID(DEPENDENCIES);
	DECLARE_ID(CLASSLIST);
	DECLARE_ID(CLASS);
	DECLARE_ID(MEMBER);
	DECLARE_ID(METHOD);
	DECLARE_ID(LITERALS);
	DECLARE_ID(LITERAL);
	DECLARE_ID(abstract);
	DECLARE_ID(anonymous);
	DECLARE_ID(noDefConstructor);
	DECLARE_ID(returnType);
	DECLARE_ID(numArgs);
	DECLARE_ID(declaration);
	DECLARE_ID(definition);
	DECLARE_ID(classDecl);
	DECLARE_ID(initialisers);
	DECLARE_ID(destructors);
	DECLARE_ID(pluginFormats);
	DECLARE_ID(buildVST);
	DECLARE_ID(buildVST3);
	DECLARE_ID(buildAU);
	DECLARE_ID(buildAUv3);
	DECLARE_ID(buildRTAS);
	DECLARE_ID(buildAAX);
	DECLARE_ID(buildStandalone);
	DECLARE_ID(buildUnity);
	DECLARE_ID(enableIAA);
	DECLARE_ID(pluginName);
	DECLARE_ID(pluginDesc);
	DECLARE_ID(pluginManufacturer);
	DECLARE_ID(pluginManufacturerCode);
	DECLARE_ID(pluginCode);
	DECLARE_ID(pluginChannelConfigs);
	DECLARE_ID(pluginCharacteristicsValue);
	DECLARE_ID(pluginIsSynth);
	DECLARE_ID(pluginWantsMidiIn);
	DECLARE_ID(pluginProducesMidiOut);
	DECLARE_ID(pluginIsMidiEffectPlugin);
	DECLARE_ID(pluginEditorRequiresKeys);
	DECLARE_ID(pluginVSTCategory);
	DECLARE_ID(pluginVST3Category);
	DECLARE_ID(pluginAUExportPrefix);
	DECLARE_ID(pluginAUMainType);
	DECLARE_ID(pluginAUIsSandboxSafe);
	DECLARE_ID(pluginRTASCategory);
	DECLARE_ID(pluginRTASDisableBypass);
	DECLARE_ID(pluginRTASDisableMultiMono);
	DECLARE_ID(pluginAAXCategory);
	DECLARE_ID(pluginAAXDisableBypass);
	DECLARE_ID(pluginAAXDisableMultiMono);
	DECLARE_ID(pluginVSTNumMidiInputs);
	DECLARE_ID(pluginVSTNumMidiOutputs);
	DECLARE_ID(exporters);
	DECLARE_ID(website);
	DECLARE_ID(mainClass);
	DECLARE_ID(moduleFlags);
	DECLARE_ID(buildEnabled);
	DECLARE_ID(continuousRebuildEnabled);
	DECLARE_ID(warningsEnabled);
	DECLARE_ID(projectLineFeed);
	DECLARE_ID(compilerFlagSchemes);
	DECLARE_ID(compilerFlagScheme);

	const Identifier ID("id");
	const Identifier ID_uppercase("ID");
	const Identifier class_("class");
	const Identifier dependencies_("dependencies");

#undef DECLARE_ID
}

const char* const projectItemDragType = "Project Items";
const char* const drawableItemDragType = "Drawable Items";
const char* const componentItemDragType = "Components";

const char* const sourceFileExtensions = "cpp;mm;m;c;cc;cxx;swift;s;asm;r";
const char* const headerFileExtensions = "h;hpp;hxx;hh;inl";
const char* const cOrCppFileExtensions = "cpp;cc;cxx;c";
const char* const cppFileExtensions = "cpp;cc;cxx";
const char* const objCFileExtensions = "mm;m";
const char* const asmFileExtensions = "s;S;asm";
const char* const sourceOrHeaderFileExtensions = "cpp;mm;m;c;cc;cxx;swift;s;S;asm;h;hpp;hxx;hh;inl";
const char* const browseableFileExtensions = "cpp;mm;m;c;cc;cxx;swift;s;S;asm;h;hpp;hxx;hh;inl;txt;md;rtf";
const char* const fileTypesToCompileByDefault = "cpp;mm;c;m;cc;cxx;swift;s;S;asm;r";

class Project : public FileBasedDocument,
	public ValueTree::Listener
{
public:
	
	//==============================================================================
	Project(const File& f) : FileBasedDocument(projectFileExtension,
		String("*") + projectFileExtension,
		"Choose a Jucer project to load",
		"Save Jucer project")
	{
		modificationTime = getFile().getLastModificationTime();
		loadDocument(f);


		//juce::String valueTrue = getMainGroup().state.toXmlString();// x->item.state.toXmlString();
		//AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon, "Invalid Global Path", valueTrue);
	}
	~Project() override
	{
		projectRoot.removeListener(this);
	}

	//==============================================================================
	// FileBasedDocument stuff..
	String getDocumentTitle() override
	{
		return getProjectNameString();
	}
	Result loadDocument(const File& file) override
	{

		auto xml = parseXML(file);

		if (xml == nullptr || !xml->hasTagName(Ids::JUCERPROJECT.toString()))
			return Result::fail("Not a valid Jucer project!");

		auto newTree = ValueTree::fromXml(*xml);

		if (!newTree.hasType(Ids::JUCERPROJECT))
			return Result::fail("The document contains errors and couldn't be parsed!");

	
		projectRoot = newTree;

		return Result::ok();
	}
	Result saveDocument(const File& file) override
	{
		return Result::ok();
	}
	Result saveProject(const File& file, bool isCommandLineApp)
	{
		return Result::ok();
	}
	Result saveResourcesOnly(const File& file)
	{
		return Result::ok();
	}
	File getLastDocumentOpened() override;
 
	void setLastDocumentOpened(const File& file) override;

	void setTitle(const String& newTitle);

	//==============================================================================
	File getProjectFolder() const { return getFile().getParentDirectory(); }
	File getGeneratedCodeFolder() const { return getFile().getSiblingFile("JuceLibraryCode"); }
	File getSourceFilesFolder() const { return getProjectFolder().getChildFile("Source"); }
	File getLocalModulesFolder() const { return getGeneratedCodeFolder().getChildFile("modules"); }
	File getLocalModuleFolder(const String& moduleID) const { return getLocalModulesFolder().getChildFile(moduleID); }
	File getAppIncludeFile() const { return getGeneratedCodeFolder().getChildFile(getJuceSourceHFilename()); }

	File getBinaryDataCppFile(int index) const;
	File getBinaryDataHeaderFile() const { return getBinaryDataCppFile(0).withFileExtension(".h"); }

	String getAppConfigFilename() const { return "AppConfig.h"; }
	String getJuceSourceFilenameRoot() const { return "JuceLibraryCode"; }
	String getJuceSourceHFilename() const { return "JuceHeader.h"; }

	//==============================================================================
	template <class FileType>
	bool shouldBeAddedToBinaryResourcesByDefault(const FileType& file)
	{
		return !file.hasFileExtension(sourceOrHeaderFileExtensions);
	}

	File resolveFilename(String filename) const;
	String getRelativePathForFile(const File& file) const;

	//==============================================================================
	// Creates editors for the project settings
	//void createPropertyEditors(PropertyListBuilder&);

	//==============================================================================
	ValueTree getProjectRoot() const { return projectRoot; }
	Value getProjectValue(const Identifier& name) { return projectRoot.getPropertyAsValue(name, getUndoManagerFor(projectRoot)); }
	var   getProjectVar(const Identifier& name) const { return projectRoot.getProperty(name); }

	//const ProjectType& getProjectType() const;
	String getProjectTypeString() const { return projectTypeValue.get(); }
	void setProjectType(const String& newProjectType) { projectTypeValue = newProjectType; }

	String getProjectNameString() const { return projectNameValue.get(); }
	String getProjectFilenameRootString() { return File::createLegalFileName(getDocumentTitle()); }
	String getProjectUIDString() const { return projectUIDValue.get(); }

	String getProjectLineFeed() const { return projectLineFeedValue.get(); }

	String getVersionString() const { return versionValue.get(); }
	String getVersionAsHex() const;
	int getVersionAsHexInteger() const;
	void setProjectVersion(const String& newVersion) { versionValue = newVersion; }

	String getBundleIdentifierString() const { return bundleIdentifierValue.get(); }
	String getDefaultBundleIdentifierString() const;
	String getDefaultAAXIdentifierString() const { return getDefaultBundleIdentifierString(); }
	String getDefaultPluginManufacturerString() const;

	String getCompanyNameString() const { return companyNameValue.get(); }
	String getCompanyCopyrightString() const { return companyCopyrightValue.get(); }
	String getCompanyWebsiteString() const { return companyWebsiteValue.get(); }
	String getCompanyEmailString() const { return companyEmailValue.get(); }

	String getHeaderSearchPathsString() const { return headerSearchPathsValue.get(); }

	StringPairArray getPreprocessorDefs() const { return parsedPreprocessorDefs; }

	int getMaxBinaryFileSize() const { return maxBinaryFileSizeValue.get(); }
	bool shouldIncludeBinaryInJuceHeader() const { return includeBinaryDataInJuceHeaderValue.get(); }
	String getBinaryDataNamespaceString() const { return binaryDataNamespaceValue.get(); }

	bool shouldDisplaySplashScreen() const { return displaySplashScreenValue.get(); }
	bool shouldReportAppUsage() const { return reportAppUsageValue.get(); }
	String getSplashScreenColourString() const { return splashScreenColourValue.get(); }

	String getCppStandardString() const { return cppStandardValue.get(); }

	StringArray getCompilerFlagSchemes() const;
	void addCompilerFlagScheme(const String&);
	void removeCompilerFlagScheme(const String&);

	//==============================================================================
	String getPluginNameString() const { return pluginNameValue.get(); }
	String getPluginDescriptionString() const { return pluginDescriptionValue.get(); }
	String getPluginManufacturerString() const { return pluginManufacturerValue.get(); }
	String getPluginManufacturerCodeString() const { return pluginManufacturerCodeValue.get(); }
	String getPluginCodeString() const { return pluginCodeValue.get(); }
	String getPluginChannelConfigsString() const { return pluginChannelConfigsValue.get(); }
	String getAAXIdentifierString() const { return pluginAAXIdentifierValue.get(); }
	String getPluginAUExportPrefixString() const { return pluginAUExportPrefixValue.get(); }
	String getVSTNumMIDIInputsString() const { return pluginVSTNumMidiInputsValue.get(); }
	String getVSTNumMIDIOutputsString() const { return pluginVSTNumMidiOutputsValue.get(); }

	//==============================================================================
	static bool checkMultiChoiceVar(const ValueWithDefault& valueToCheck, Identifier idToCheck) noexcept
	{
		if (!valueToCheck.get().isArray())
			return false;

		auto v = valueToCheck.get();

		if (auto* varArray = v.getArray())
			return varArray->contains(idToCheck.toString());

		return false;
	}

	//==============================================================================
	//bool shouldBuildVST() const { return checkMultiChoiceVar(pluginFormatsValue, Ids::buildVST); }
	//bool shouldBuildVST3() const { return checkMultiChoiceVar(pluginFormatsValue, Ids::buildVST3); }
	//bool shouldBuildAU() const { return checkMultiChoiceVar(pluginFormatsValue, Ids::buildAU); }
	//bool shouldBuildAUv3() const { return checkMultiChoiceVar(pluginFormatsValue, Ids::buildAUv3); }
	//bool shouldBuildRTAS() const { return checkMultiChoiceVar(pluginFormatsValue, Ids::buildRTAS); }
	//bool shouldBuildAAX() const { return checkMultiChoiceVar(pluginFormatsValue, Ids::buildAAX); }
	//bool shouldBuildStandalonePlugin() const { return checkMultiChoiceVar(pluginFormatsValue, Ids::buildStandalone); }
	//bool shouldBuildUnityPlugin() const { return checkMultiChoiceVar(pluginFormatsValue, Ids::buildUnity); }
	//bool shouldEnableIAA() const { return checkMultiChoiceVar(pluginFormatsValue, Ids::enableIAA); }

	////==============================================================================
	//bool isPluginSynth() const { return checkMultiChoiceVar(pluginCharacteristicsValue, Ids::pluginIsSynth); }
	//bool pluginWantsMidiInput() const { return checkMultiChoiceVar(pluginCharacteristicsValue, Ids::pluginWantsMidiIn); }
	//bool pluginProducesMidiOutput() const { return checkMultiChoiceVar(pluginCharacteristicsValue, Ids::pluginProducesMidiOut); }
	//bool isPluginMidiEffect() const { return checkMultiChoiceVar(pluginCharacteristicsValue, Ids::pluginIsMidiEffectPlugin); }
	//bool pluginEditorNeedsKeyFocus() const { return checkMultiChoiceVar(pluginCharacteristicsValue, Ids::pluginEditorRequiresKeys); }
	//bool isPluginRTASBypassDisabled() const { return checkMultiChoiceVar(pluginCharacteristicsValue, Ids::pluginRTASDisableBypass); }
	//bool isPluginRTASMultiMonoDisabled() const { return checkMultiChoiceVar(pluginCharacteristicsValue, Ids::pluginRTASDisableMultiMono); }
	//bool isPluginAAXBypassDisabled() const { return checkMultiChoiceVar(pluginCharacteristicsValue, Ids::pluginAAXDisableBypass); }
	//bool isPluginAAXMultiMonoDisabled() const { return checkMultiChoiceVar(pluginCharacteristicsValue, Ids::pluginAAXDisableMultiMono); }

	//==============================================================================
	static StringArray getAllAUMainTypeStrings() noexcept;
	static Array<var> getAllAUMainTypeVars() noexcept;
	Array<var> getDefaultAUMainTypes() const noexcept;

	static StringArray getAllVSTCategoryStrings() noexcept;
	Array<var> getDefaultVSTCategories() const noexcept;

	static StringArray getAllVST3CategoryStrings() noexcept;
	Array<var> getDefaultVST3Categories() const noexcept;

	static StringArray getAllAAXCategoryStrings() noexcept;
	static Array<var> getAllAAXCategoryVars() noexcept;
	Array<var> getDefaultAAXCategories() const noexcept;

	static StringArray getAllRTASCategoryStrings() noexcept;
	static Array<var> getAllRTASCategoryVars() noexcept;
	Array<var> getDefaultRTASCategories() const noexcept;

	String getAUMainTypeString() const noexcept;
	bool isAUSandBoxSafe() const noexcept;
	String getVSTCategoryString() const noexcept;
	String getVST3CategoryString() const noexcept;
	int getAAXCategory() const noexcept;
	int getRTASCategory() const noexcept;

	String getIAATypeCode();
	String getIAAPluginName();

	String getUnityScriptName() const { return addUnityPluginPrefixIfNecessary(getProjectNameString()) + "_UnityScript.cs"; }
	static String addUnityPluginPrefixIfNecessary(const String& name)
	{
		if (!name.startsWithIgnoreCase("audioplugin"))
			return "audioplugin_" + name;

		return name;
	}

	//==============================================================================
	bool isAUPluginHost();
	bool isVSTPluginHost();
	bool isVST3PluginHost();

	//==============================================================================
	//bool shouldBuildTargetType(ProjectType::Target::Type targetType) const noexcept;
	//static ProjectType::Target::Type getTargetTypeFromFilePath(const File& file, bool returnSharedTargetIfNoValidSuffix);

	//==============================================================================
	void updateDeprecatedProjectSettingsInteractively();

	//==============================================================================
	class Item
	{
	public:
		//==============================================================================
		Item(Project& project, const ValueTree& itemNode, bool isModuleCode);
		Item(const Item& other);

		static Item createGroup(Project& project, const String& name, const String& uid, bool isModuleCode);
		void initialiseMissingProperties();

		//==============================================================================
		bool isValid() const { return state.isValid(); }
		bool operator== (const Item& other) const { return state == other.state && &project == &other.project; }
		bool operator!= (const Item& other) const { return !operator== (other); }

		//==============================================================================
		bool isFile() const;
		bool isGroup() const;
		bool isMainGroup() const;
		bool isImageFile() const;

		String getID() const;
		void setID(const String& newID);
		Item findItemWithID(const String& targetId) const; // (recursive search)

		String getImageFileID() const;
		Drawable* loadAsImageFile() const;

		//==============================================================================
		Value getNameValue();
		String getName() const;
		File getFile() const;
		String getFilePath() const;
		void setFile(const File& file);
		//void setFile(const RelativePath& file);
		File determineGroupFolder() const;
		bool renameFile(const File& newFile);

		bool shouldBeAddedToTargetProject() const;
		bool shouldBeCompiled() const;
		Value getShouldCompileValue();

		bool shouldBeAddedToBinaryResources() const;
		Value getShouldAddToBinaryResourcesValue();

		bool shouldBeAddedToXcodeResources() const;
		Value getShouldAddToXcodeResourcesValue();

		Value getShouldInhibitWarningsValue();
		bool shouldInhibitWarnings() const;

		bool isModuleCode() const;

		Value getCompilerFlagSchemeValue();
		String getCompilerFlagSchemeString() const;

		void setCompilerFlagScheme(const String&);
		void clearCurrentCompilerFlagScheme();

		//==============================================================================
		bool canContain(const Item& child) const;
		int getNumChildren() const { return state.getNumChildren(); }
		Item getChild(int index) const { return Item(project, state.getChild(index), belongsToModule); }

		Item addNewSubGroup(const String& name, int insertIndex);
		Item getOrCreateSubGroup(const String& name);
		void addChild(const Item& newChild, int insertIndex);
		bool addFileAtIndex(const File& file, int insertIndex, bool shouldCompile);
		bool addFileRetainingSortOrder(const File& file, bool shouldCompile);
		void addFileUnchecked(const File& file, int insertIndex, bool shouldCompile);
		//bool addRelativeFile(const RelativePath& file, int insertIndex, bool shouldCompile);
		void removeItemFromProject();
		void sortAlphabetically(bool keepGroupsAtStart, bool recursive);
		Item findItemForFile(const File& file) const;
		//bool containsChildForFile(const RelativePath& file) const;

		Item getParent() const;
		Item createCopy();

		UndoManager* getUndoManager() const { return project.getUndoManagerFor(state); }

		Icon getIcon(bool isOpen = false) const;
		bool isIconCrossedOut() const;

		bool needsSaving() const noexcept;

		Project& project;
		ValueTree state;

	private:
		Item& operator= (const Item&);
		bool belongsToModule;
	};

	Item getMainGroup();

	void findAllImageItems(OwnedArray<Item>& items);

	//==============================================================================
	ValueTree getExporters();
	int getNumExporters();
	//ProjectExporter* createExporter(int index);
	void addNewExporter(const String& exporterName);
	void createExporterForCurrentPlatform();

	struct ExporterIterator
	{
		ExporterIterator(Project& project);
		~ExporterIterator();

		bool next();

	/*	ProjectExporter& operator*() const { return *exporter; }
		ProjectExporter* operator->() const { return exporter.get(); }*/

		//std::unique_ptr<ProjectExporter> exporter;
		int index;

	private:
		Project& project;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ExporterIterator)
	};

	//==============================================================================
	struct ConfigFlag
	{
		String symbol, description, sourceModuleID;
		ValueWithDefault value;
	};

	ValueWithDefault getConfigFlag(const String& name);
	bool isConfigFlagEnabled(const String& name, bool defaultIsEnabled = false) const;

	//==============================================================================
	/*EnabledModuleList& getEnabledModules();

	AvailableModuleList& getExporterPathsModuleList();*/
	void rescanExporterPathModules(bool async = false);

	std::pair<String, File> getModuleWithID(const String&);

	//==============================================================================
	String getFileTemplate(const String& templateName);

	//==============================================================================
	PropertiesFile& getStoredProperties() const;

	//==============================================================================
	void valueTreePropertyChanged(ValueTree&, const Identifier&) override
	{

	}
	void valueTreeChildAdded(ValueTree&, ValueTree&) override
	{

	}
	void valueTreeChildRemoved(ValueTree&, ValueTree&, int) override
	{

	}
	void valueTreeChildOrderChanged(ValueTree&, int, int) override
	{

	}
	void valueTreeParentChanged(ValueTree&) override
	{

	}

	//==============================================================================
	UndoManager* getUndoManagerFor(const ValueTree&) const { return nullptr; }
	UndoManager* getUndoManager() const { return nullptr; }

	//==============================================================================
	static const char* projectFileExtension;

	//==============================================================================
	bool hasProjectBeenModified();
	void updateModificationTime() { modificationTime = getFile().getLastModificationTime(); }

	//==============================================================================
	String getUniqueTargetFolderSuffixForExporter(const String& exporterName, const String& baseTargetFolder);

	//==============================================================================
	bool isCurrentlySaving() const noexcept { return isSaving; }
	bool shouldWaitAfterSaving = false;
	String specifiedExporterToSave = {};

	//==============================================================================
	bool isTemporaryProject() const noexcept { return tempDirectory != File(); }
	File getTemporaryDirectory() const noexcept { return tempDirectory; }
	void setTemporaryDirectory(const File&) noexcept;

	void setOpenInIDEAfterSaving(bool open) noexcept { openInIDEAfterSaving = open; }
	bool shouldOpenInIDEAfterSaving() const noexcept { return openInIDEAfterSaving; }

	//==============================================================================
	bool shouldSendGUIBuilderAnalyticsEvent() noexcept;

	//==============================================================================
	//CompileEngineSettings& getCompileEngineSettings() { return *compileEngineSettings; }

private:
	ValueTree projectRoot{ Ids::JUCERPROJECT };

	ValueWithDefault projectNameValue, projectUIDValue, projectLineFeedValue, projectTypeValue, versionValue, bundleIdentifierValue, companyNameValue,
		companyCopyrightValue, companyWebsiteValue, companyEmailValue, displaySplashScreenValue, reportAppUsageValue, splashScreenColourValue, cppStandardValue,
		headerSearchPathsValue, preprocessorDefsValue, userNotesValue, maxBinaryFileSizeValue, includeBinaryDataInJuceHeaderValue, binaryDataNamespaceValue,
		compilerFlagSchemesValue;

	ValueWithDefault pluginFormatsValue, pluginNameValue, pluginDescriptionValue, pluginManufacturerValue, pluginManufacturerCodeValue,
		pluginCodeValue, pluginChannelConfigsValue, pluginCharacteristicsValue, pluginAUExportPrefixValue, pluginAAXIdentifierValue,
		pluginAUMainTypeValue, pluginAUSandboxSafeValue, pluginRTASCategoryValue, pluginVSTCategoryValue, pluginVST3CategoryValue, pluginAAXCategoryValue,
		pluginVSTNumMidiInputsValue, pluginVSTNumMidiOutputsValue;

	//==============================================================================
	//std::unique_ptr<CompileEngineSettings> compileEngineSettings;
	//std::unique_ptr<EnabledModuleList> enabledModuleList;
	//std::unique_ptr<AvailableModuleList> exporterPathsModuleList;

	//==============================================================================
	bool shouldWriteLegacyPluginFormatSettings = false;
	bool shouldWriteLegacyPluginCharacteristicsSettings = false;

	static Array<Identifier> getLegacyPluginFormatIdentifiers() noexcept;
	static Array<Identifier> getLegacyPluginCharacteristicsIdentifiers() noexcept;

	void writeLegacyPluginFormatSettings();
	void writeLegacyPluginCharacteristicsSettings();

	void coalescePluginFormatValues();
	void coalescePluginCharacteristicsValues();
	void updatePluginCategories();

	//==============================================================================
	File tempDirectory = {};
	bool openInIDEAfterSaving = false;

	void askUserWhereToSaveProject();
	void moveTemporaryDirectory(const File&);
	bool saveProjectRootToFile();

	//==============================================================================
	bool hasSentGUIBuilderAnalyticsEvent = false;

	//==============================================================================
	friend class Item;
	bool isSaving = false;
	Time modificationTime;
	StringPairArray parsedPreprocessorDefs;

	//==============================================================================
	void initialiseProjectValues();
	void initialiseMainGroup();
	void initialiseAudioPluginValues();

	bool setCppVersionFromOldExporterSettings();

	//void createAudioPluginPropertyEditors(PropertyListBuilder& props);

	//==============================================================================
	void updateTitleDependencies();
	void updateCompanyNameDependencies();
	void updateProjectSettings();
	ValueTree getConfigurations() const;
	ValueTree getConfigNode();

	void updateOldStyleConfigList();
	void moveOldPropertyFromProjectToAllExporters(Identifier name);
	void removeDefunctExporters();
	void updateOldModulePaths();
	void warnAboutOldProjucerVersion();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Project)
};




class FileTreeItemBase : public JucerTreeViewBase,
	public ValueTree::Listener
{
public:
	FileTreeItemBase(const Project::Item& projectItem)
		: item(projectItem), isFileMissing(false)
	{
		item.state.addListener(this);
	}

	~FileTreeItemBase() override
	{
		item.state.removeListener(this);
	}

	//==============================================================================
	virtual bool acceptsFileDrop(const StringArray& files) const = 0;
	virtual bool acceptsDragItems(const OwnedArray<Project::Item>& selectedNodes) = 0;

	//==============================================================================
	String getDisplayName() const override { return item.getName(); }
	String getRenamingName() const override { return getDisplayName(); }
	void setName(const String& newName) override { item.getNameValue() = newName; }
	bool isMissing() const override { return isFileMissing; }
	virtual File getFile() const { return item.getFile(); }

	void deleteItem() override { item.removeItemFromProject(); }

	void deleteAllSelectedItems() override
	{
		auto* tree = getOwnerView();
		Array<File> filesToTrash;
		Array<Project::Item> itemsToRemove;

		for (int i = 0; i < tree->getNumSelectedItems(); ++i)
		{
			if (auto* p = dynamic_cast<FileTreeItemBase*> (tree->getSelectedItem(i)))
			{
				itemsToRemove.add(p->item);

				if (p->item.isGroup())
				{
					for (int j = 0; j < p->item.getNumChildren(); ++j)
					{
						auto associatedFile = p->item.getChild(j).getFile();

						if (associatedFile.existsAsFile())
							filesToTrash.addIfNotAlreadyThere(associatedFile);
					}
				}
				else if (p->getFile().existsAsFile())
				{
					filesToTrash.addIfNotAlreadyThere(p->getFile());
				}
			}
		}

		if (filesToTrash.size() > 0)
		{
			String fileList;
			auto maxFilesToList = 10;
			for (auto i = jmin(maxFilesToList, filesToTrash.size()); --i >= 0;)
				fileList << filesToTrash.getUnchecked(i).getFullPathName() << "\n";

			if (filesToTrash.size() > maxFilesToList)
				fileList << "\n...plus " << (filesToTrash.size() - maxFilesToList) << " more files...";

			auto r = AlertWindow::showYesNoCancelBox(AlertWindow::NoIcon, "Delete Project Items",
				"As well as removing the selected item(s) from the project, do you also want to move their files to the trash:\n\n"
				+ fileList,
				"Just remove references",
				"Also move files to Trash",
				"Cancel",
				tree->getTopLevelComponent());

			if (r == 0)
				return;

			if (r != 2)
				filesToTrash.clear();
		}

		//if (auto* treeRootItem = dynamic_cast<FileTreeItemBase*> (tree->getRootItem()))
		//{
		//	//auto& om = ProjucerApplication::getApp().openDocumentManager;

		//	for (auto i = filesToTrash.size(); --i >= 0;)
		//	{
		//		auto f = filesToTrash.getUnchecked(i);

		//		om.closeFile(f, false);

		//		if (!f.moveToTrash())
		//		{
		//			// xxx
		//		}
		//	}

		//	for (auto i = itemsToRemove.size(); --i >= 0;)
		//	{
		//		if (auto itemToRemove = treeRootItem->findTreeViewItem(itemsToRemove.getUnchecked(i)))
		//		{
		//			if (auto* pcc = treeRootItem->getProjectContentComponent())
		//			{
		//				if (auto* fileInfoComp = dynamic_cast<FileGroupInformationComponent*> (pcc->getEditorComponentContent()))
		//					if (fileInfoComp->getGroupPath() == itemToRemove->getFile().getFullPathName())
		//						pcc->hideEditor();
		//			}

		//			om.closeFile(itemToRemove->getFile(), false);
		//			itemToRemove->deleteItem();
		//		}
		//	}
		//}
		//else
		//{
		//	jassertfalse;
		//}
	}

	virtual void revealInFinder() const
	{
		getFile().revealToUser();
	}

	virtual void browseToAddExistingFiles()
	{
		auto location = item.isGroup() ? item.determineGroupFolder() : getFile();
		FileChooser fc("Add Files to Jucer Project", location, {});

		if (fc.browseForMultipleFilesOrDirectories())
		{
			StringArray files;

			for (int i = 0; i < fc.getResults().size(); ++i)
				files.add(fc.getResults().getReference(i).getFullPathName());

			addFilesRetainingSortOrder(files);
		}
	}

	virtual void checkFileStatus()  // (recursive)
	{
		auto file = getFile();
		auto nowMissing = (file != File() && !file.exists());

		if (nowMissing != isFileMissing)
		{
			isFileMissing = nowMissing;
			repaintItem();
		}
	}

	virtual void addFilesAtIndex(const StringArray& files, int insertIndex)
	{
		if (auto* p = getParentProjectItem())
			p->addFilesAtIndex(files, insertIndex);
	}

	virtual void addFilesRetainingSortOrder(const StringArray& files)
	{
		if (auto* p = getParentProjectItem())
			p->addFilesRetainingSortOrder(files);
	}

	virtual void moveSelectedItemsTo(OwnedArray <Project::Item>&, int /*insertIndex*/)
	{
		jassertfalse;
	}

	void showMultiSelectionPopupMenu() override
	{
		PopupMenu m;
		m.addItem(1, "Delete");

		m.showMenuAsync(PopupMenu::Options(),
			ModalCallbackFunction::create(treeViewMultiSelectItemChosen, this));
	}

	static void treeViewMultiSelectItemChosen(int resultCode, FileTreeItemBase* item)
	{
		switch (resultCode)
		{
		case 1:     item->deleteAllSelectedItems(); break;
		default:    break;
		}
	}

	virtual FileTreeItemBase* findTreeViewItem(const Project::Item& itemToFind)
	{
		if (item == itemToFind)
			return this;

		auto wasOpen = isOpen();
		setOpen(true);

		for (auto i = getNumSubItems(); --i >= 0;)
		{
			if (auto* pg = dynamic_cast<FileTreeItemBase*> (getSubItem(i)))
				if (auto* found = pg->findTreeViewItem(itemToFind))
					return found;
		}

		setOpen(wasOpen);
		return nullptr;
	}

	//==============================================================================
	void valueTreePropertyChanged(ValueTree& tree, const Identifier&) override
	{
		if (tree == item.state)
			repaintItem();
	}

	void valueTreeChildAdded(ValueTree& parentTree, ValueTree&) override { treeChildrenChanged(parentTree); }
	void valueTreeChildRemoved(ValueTree& parentTree, ValueTree&, int) override { treeChildrenChanged(parentTree); }
	void valueTreeChildOrderChanged(ValueTree& parentTree, int, int) override { treeChildrenChanged(parentTree); }
	void valueTreeParentChanged(ValueTree&) override {}

	//==============================================================================
	bool mightContainSubItems() override { return item.getNumChildren() > 0; }
	String getUniqueName() const override { jassert(item.getID().isNotEmpty()); return item.getID(); }
	bool canBeSelected() const override { return true; }
	String getTooltip() override { return {}; }
	File getDraggableFile() const override { return getFile(); }

	//var getDragSourceDescription() override
	//{
	//	cancelDelayedSelectionTimer();
	//	return projectItemDragType;
	//}

	void addSubItems() override
	{
		for (int i = 0; i < item.getNumChildren(); ++i)
			if (auto* p = createSubItem(item.getChild(i)))
				addSubItem(p);
	}

	void itemOpennessChanged(bool isNowOpen) override
	{
		if (isNowOpen)
			refreshSubItems();
	}

	//==============================================================================
	bool isInterestedInFileDrag(const StringArray& files) override
	{
		return acceptsFileDrop(files);
	}

	void filesDropped(const StringArray& files, int insertIndex) override
	{
	/*	if (files.size() == 1 && File(files[0]).hasFileExtension(Project::projectFileExtension))
			ProjucerApplication::getApp().openFile(files[0]);
		else
			addFilesAtIndex(files, insertIndex);*/
	}

	bool isInterestedInDragSource(const DragAndDropTarget::SourceDetails& dragSourceDetails) override
	{
		OwnedArray<Project::Item> selectedNodes;
		getSelectedProjectItemsBeingDragged(dragSourceDetails, selectedNodes);

		return selectedNodes.size() > 0 && acceptsDragItems(selectedNodes);
	}

	void itemDropped(const DragAndDropTarget::SourceDetails& dragSourceDetails, int insertIndex) override
	{
		OwnedArray<Project::Item> selectedNodes;
		getSelectedProjectItemsBeingDragged(dragSourceDetails, selectedNodes);

		if (selectedNodes.size() > 0)
		{
			auto* tree = getOwnerView();
			std::unique_ptr<XmlElement> oldOpenness(tree->getOpennessState(false));

			moveSelectedItemsTo(selectedNodes, insertIndex);

			if (oldOpenness != nullptr)
				tree->restoreOpennessState(*oldOpenness, false);
		}
	}

	int getMillisecsAllowedForDragGesture() override
	{
		// for images, give the user longer to start dragging before assuming they're
		// clicking to select it for previewing..
		return item.isImageFile() ? 250 : JucerTreeViewBase::getMillisecsAllowedForDragGesture();
	}

	static void getSelectedProjectItemsBeingDragged(const DragAndDropTarget::SourceDetails& dragSourceDetails,
		OwnedArray<Project::Item>& selectedNodes)
	{
		if (dragSourceDetails.description == projectItemDragType)
		{
			auto* tree = dynamic_cast<TreeView*> (dragSourceDetails.sourceComponent.get());

			if (tree == nullptr)
				tree = dragSourceDetails.sourceComponent->findParentComponentOfClass<TreeView>();

			if (tree != nullptr)
			{
				auto numSelected = tree->getNumSelectedItems();

				for (int i = 0; i < numSelected; ++i)
					if (auto* p = dynamic_cast<FileTreeItemBase*> (tree->getSelectedItem(i)))
						selectedNodes.add(new Project::Item(p->item));
			}
		}
	}

	FileTreeItemBase* getParentProjectItem() const
	{
		return dynamic_cast<FileTreeItemBase*> (getParentItem());
	}

	//==============================================================================
	Project::Item item;

protected:
	bool isFileMissing;

	virtual FileTreeItemBase* createSubItem(const Project::Item& node) = 0;

	Icon getIcon() const override
	{
		auto colour = getOwnerView()->findColour(isSelected() ? defaultHighlightedTextColourId
			: treeIconColourId);

		return item.getIcon(isOpen()).withColour(colour);
	}

	bool isIconCrossedOut() const override { return item.isIconCrossedOut(); }

	void treeChildrenChanged(const ValueTree& parentTree)
	{
		if (parentTree == item.state)
		{
			refreshSubItems();
			treeHasChanged();
			setOpen(true);
		}
	}

	void triggerAsyncRename(const Project::Item& itemToRename)
	{
		struct RenameMessage : public CallbackMessage
		{
			RenameMessage(TreeView* const t, const Project::Item& i)
				: tree(t), itemToRename(i) {}

			void messageCallback() override
			{
				if (tree != nullptr)
					if (auto* root = dynamic_cast<FileTreeItemBase*> (tree->getRootItem()))
						if (auto* found = root->findTreeViewItem(itemToRename))
							found->showRenameBox();
			}

		private:
			Component::SafePointer<TreeView> tree;
			Project::Item itemToRename;
		};

		(new RenameMessage(getOwnerView(), itemToRename))->post();
	}

	static void moveItems(OwnedArray<Project::Item>& selectedNodes, Project::Item destNode, int insertIndex)
	{
		for (auto i = selectedNodes.size(); --i >= 0;)
		{
			auto* n = selectedNodes.getUnchecked(i);

			if (destNode == *n || destNode.state.isAChildOf(n->state)) // Check for recursion.
				return;

			if (!destNode.canContain(*n))
				selectedNodes.remove(i);
		}

		// Don't include any nodes that are children of other selected nodes..
		for (auto i = selectedNodes.size(); --i >= 0;)
		{
			auto* n = selectedNodes.getUnchecked(i);

			for (auto j = selectedNodes.size(); --j >= 0;)
			{
				if (j != i && n->state.isAChildOf(selectedNodes.getUnchecked(j)->state))
				{
					selectedNodes.remove(i);
					break;
				}
			}
		}

		// Remove and re-insert them one at a time..
		for (int i = 0; i < selectedNodes.size(); ++i)
		{
			auto* selectedNode = selectedNodes.getUnchecked(i);

			if (selectedNode->state.getParent() == destNode.state
				&& indexOfNode(destNode.state, selectedNode->state) < insertIndex)
				--insertIndex;

			selectedNode->removeItemFromProject();
			destNode.addChild(*selectedNode, insertIndex++);
		}
	}

	static int indexOfNode(const ValueTree& parent, const ValueTree& child)
	{
		for (auto i = parent.getNumChildren(); --i >= 0;)
			if (parent.getChild(i) == child)
				return i;

		return -1;
	}
};





class GroupItem : public FileTreeItemBase
{
public:
	GroupItem(const Project::Item& projectItem, const String& filter = {})
		: FileTreeItemBase(projectItem),
		searchFilter(filter)
	{
	}

	bool isRoot() const override { return item.isMainGroup(); }
	bool acceptsFileDrop(const StringArray&) const override { return true; }

	void addNewGroup()
	{
		auto newGroup = item.addNewSubGroup("New Group", 0);
		triggerAsyncRename(newGroup);
	}

	bool acceptsDragItems(const OwnedArray<Project::Item>& selectedNodes) override
	{
		for (auto i = selectedNodes.size(); --i >= 0;)
			if (item.canContain(*selectedNodes.getUnchecked(i)))
				return true;

		return false;
	}

	void addFilesAtIndex(const StringArray& files, int insertIndex) override
	{
		for (auto f : files)
		{
			if (item.addFileAtIndex(f, insertIndex, true))
				++insertIndex;
		}
	}

	void addFilesRetainingSortOrder(const StringArray& files) override
	{
		for (auto i = files.size(); --i >= 0;)
			item.addFileRetainingSortOrder(files[i], true);
	}

	void moveSelectedItemsTo(OwnedArray<Project::Item>& selectedNodes, int insertIndex) override
	{
		moveItems(selectedNodes, item, insertIndex);
	}

	void checkFileStatus() override
	{
		for (int i = 0; i < getNumSubItems(); ++i)
			if (auto* p = dynamic_cast<FileTreeItemBase*> (getSubItem(i)))
				p->checkFileStatus();
	}

	bool isGroupEmpty(const Project::Item& group) // recursive
	{
		for (int i = 0; i < group.getNumChildren(); ++i)
		{
			auto child = group.getChild(i);

			if ((child.isGroup() && !isGroupEmpty(child))
				|| (child.isFile() && child.getName().containsIgnoreCase(searchFilter)))
				return false;
		}

		return true;
	}

	FileTreeItemBase* createSubItem(const Project::Item& child) override;

	void showDocument() override
	{
		/*if (auto* pcc = getProjectContentComponent())
			pcc->setEditorComponent(new FileGroupInformationComponent(item), nullptr);*/
	}

	static void openAllGroups(TreeViewItem* root)
	{
		for (int i = 0; i < root->getNumSubItems(); ++i)
			if (auto* sub = root->getSubItem(i))
				openOrCloseAllSubGroups(*sub, true);
	}

	static void closeAllGroups(TreeViewItem* root)
	{
		for (int i = 0; i < root->getNumSubItems(); ++i)
			if (auto* sub = root->getSubItem(i))
				openOrCloseAllSubGroups(*sub, false);
	}

	static void openOrCloseAllSubGroups(TreeViewItem& item, bool shouldOpen)
	{
		item.setOpen(shouldOpen);

		for (auto i = item.getNumSubItems(); --i >= 0;)
			if (auto* sub = item.getSubItem(i))
				openOrCloseAllSubGroups(*sub, shouldOpen);
	}

	static void setFilesToCompile(Project::Item item, const bool shouldCompile)
	{
		if (item.isFile() && (item.getFile().hasFileExtension(fileTypesToCompileByDefault)))
			item.getShouldCompileValue() = shouldCompile;

		for (auto i = item.getNumChildren(); --i >= 0;)
			setFilesToCompile(item.getChild(i), shouldCompile);
	}

	void showPopupMenu() override
	{
		PopupMenu m;
		addCreateFileMenuItems(m);

		m.addSeparator();

		m.addItem(1, "Collapse all Groups");
		m.addItem(2, "Expand all Groups");

		if (!isRoot())
		{
			if (isOpen())
				m.addItem(3, "Collapse all Sub-groups");
			else
				m.addItem(4, "Expand all Sub-groups");
		}

		m.addSeparator();
		m.addItem(5, "Enable compiling of all enclosed files");
		m.addItem(6, "Disable compiling of all enclosed files");

		m.addSeparator();
		m.addItem(7, "Sort Items Alphabetically");
		m.addItem(8, "Sort Items Alphabetically (Groups first)");
		m.addSeparator();

		if (!isRoot())
		{
			m.addItem(9, "Rename...");
			m.addItem(10, "Delete");
		}

		launchPopupMenu(m);
	}

	void showAddMenu() override
	{
		PopupMenu m;
		addCreateFileMenuItems(m);

		launchPopupMenu(m);
	}

	void handlePopupMenuResult(int resultCode) override
	{
		switch (resultCode)
		{
		case 1:     closeAllGroups(getOwnerView()->getRootItem()); break;
		case 2:     openAllGroups(getOwnerView()->getRootItem()); break;
		case 3:     openOrCloseAllSubGroups(*this, false); break;
		case 4:     openOrCloseAllSubGroups(*this, true); break;
		case 5:     setFilesToCompile(item, true); break;
		case 6:     setFilesToCompile(item, false); break;
		case 7:     item.sortAlphabetically(false, false); break;
		case 8:     item.sortAlphabetically(true, false); break;
		case 9:     triggerAsyncRename(item); break;
		case 10:    deleteAllSelectedItems(); break;
		default:    processCreateFileMenuItem(resultCode); break;
		}
	}

	void addCreateFileMenuItems(PopupMenu& m)
	{
		m.addItem(1001, "Add New Group");
		m.addItem(1002, "Add Existing Files...");

		m.addSeparator();
		//NewFileWizard().addWizardsToMenu(m);
	}

	void processCreateFileMenuItem(int menuID)
	{
		switch (menuID)
		{
		case 1001:  addNewGroup(); break;
		case 1002:  browseToAddExistingFiles(); break;

		default:
			jassert(getProject() != nullptr);
			//NewFileWizard().runWizardFromMenu(menuID, *getProject(), item);
			break;
		}
	}

	Project* getProject()
	{
		//if (auto* tv = getOwnerView())
		//	if (auto* pcc = tv->findParentComponentOfClass<ProjectContentComponent>())
		//		return pcc->getProject();

		return nullptr;
	}

	void setSearchFilter(const String& filter) override
	{
		searchFilter = filter;
		refreshSubItems();
	}

	String searchFilter;
};




class SourceFileItem : public FileTreeItemBase
{
public:
	SourceFileItem(const Project::Item& projectItem)
		: FileTreeItemBase(projectItem)
	{
	}

	bool acceptsFileDrop(const StringArray&) const override { return false; }
	bool acceptsDragItems(const OwnedArray <Project::Item>&) override { return false; }

	String getDisplayName() const override
	{
		return getFile().getFileName();
	}

	void paintItem(Graphics& g, int width, int height) override
	{
		JucerTreeViewBase::paintItem(g, width, height);

		if (item.needsSaving())
		{
			auto bounds = g.getClipBounds().withY(0).withHeight(height);

			g.setFont(getFont());
			g.setColour(getContentColour(false));

			g.drawFittedText("*", bounds.removeFromLeft(height), Justification::centred, 1);
		}
	}

	static File findCorrespondingHeaderOrCpp(const File& f)
	{
		if (f.hasFileExtension(sourceFileExtensions))  return f.withFileExtension(".h");
		if (f.hasFileExtension(headerFileExtensions))  return f.withFileExtension(".cpp");

		return {};
	}

	void setName(const String& newName) override
	{
		if (newName != File::createLegalFileName(newName))
		{
			AlertWindow::showMessageBox(AlertWindow::WarningIcon, "File Rename",
				"That filename contained some illegal characters!");
			triggerAsyncRename(item);
			return;
		}

		auto oldFile = getFile();
		auto newFile = oldFile.getSiblingFile(newName);
		auto correspondingFile = findCorrespondingHeaderOrCpp(oldFile);

		if (correspondingFile.exists() && newFile.hasFileExtension(oldFile.getFileExtension()))
		{
			auto correspondingItem = item.project.getMainGroup().findItemForFile(correspondingFile);

			if (correspondingItem.isValid())
			{
				if (AlertWindow::showOkCancelBox(AlertWindow::NoIcon, "File Rename",
					"Do you also want to rename the corresponding file \"" + correspondingFile.getFileName()
					+ "\" to match?"))
				{
					if (!item.renameFile(newFile))
					{
						AlertWindow::showMessageBox(AlertWindow::WarningIcon, "File Rename",
							"Failed to rename \"" + oldFile.getFullPathName() + "\"!\n\nCheck your file permissions!");
						return;
					}

					if (!correspondingItem.renameFile(newFile.withFileExtension(correspondingFile.getFileExtension())))
					{
						AlertWindow::showMessageBox(AlertWindow::WarningIcon, "File Rename",
							"Failed to rename \"" + correspondingFile.getFullPathName() + "\"!\n\nCheck your file permissions!");
					}
				}
			}
		}

		if (!item.renameFile(newFile))
		{
			AlertWindow::showMessageBox(AlertWindow::WarningIcon, "File Rename",
				"Failed to rename the file!\n\nCheck your file permissions!");
		}
	}

	FileTreeItemBase* createSubItem(const Project::Item&) override
	{
		jassertfalse;
		return nullptr;
	}

	void showDocument() override
	{
		//auto f = getFile();

		//if (f.exists())
		//	if (auto* pcc = getProjectContentComponent())
		//		pcc->showEditorForFile(f, false);
	}

	void showPopupMenu() override
	{
		PopupMenu m;

		m.addItem(1, "Open in external editor");
		m.addItem(2,
#if JUCE_MAC
			"Reveal in Finder");
#else
			"Reveal in Explorer");
#endif

		m.addItem(4, "Rename File...");
		m.addSeparator();

		if (auto* group = dynamic_cast<GroupItem*> (getParentItem()))
		{
			if (group->isRoot())
			{
				m.addItem(5, "Binary Resource", true, item.shouldBeAddedToBinaryResources());
				m.addItem(6, "Xcode Resource", true, item.shouldBeAddedToXcodeResources());
				m.addItem(7, "Compile", true, item.shouldBeCompiled());
				m.addSeparator();
			}
		}

		m.addItem(3, "Delete");

		launchPopupMenu(m);
	}

	void showAddMenu() override
	{
		if (auto* group = dynamic_cast<GroupItem*> (getParentItem()))
			group->showAddMenu();
	}

	void handlePopupMenuResult(int resultCode) override
	{
		switch (resultCode)
		{
		case 1:     getFile().startAsProcess(); break;
		case 2:     revealInFinder(); break;
		case 3:     deleteAllSelectedItems(); break;
		case 4:     triggerAsyncRename(item); break;
		case 5:     item.getShouldAddToBinaryResourcesValue().setValue(!item.shouldBeAddedToBinaryResources()); break;
		case 6:     item.getShouldAddToXcodeResourcesValue().setValue(!item.shouldBeAddedToXcodeResources()); break;
		case 7:     item.getShouldCompileValue().setValue(!item.shouldBeCompiled()); break;

		default:
			if (auto* parentGroup = dynamic_cast<GroupItem*> (getParentProjectItem()))
				parentGroup->processCreateFileMenuItem(resultCode);

			break;
		}
	}
};
