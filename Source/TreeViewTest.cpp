

#include "TreeViewTest.h"
#include "jucer_FileHelpers.h"
#include "jucer_CommonHeaders.h"
#include "jucer_Icons.h"

JucerTreeViewBase::JucerTreeViewBase() : textX(0)
{
	setLinesDrawnForSubItems(false);
	setDrawsInLeftMargin(true);
}

JucerTreeViewBase::~JucerTreeViewBase()
{
}

void JucerTreeViewBase::refreshSubItems()
{
	//WholeTreeOpennessRestorer wtor(*this);
	clearSubItems();
	addSubItems();
}

Font JucerTreeViewBase::getFont() const
{
	return Font(getItemHeight() * 0.6f);
}

void JucerTreeViewBase::paintOpenCloseButton(Graphics& g, const Rectangle<float>& area, Colour /*backgroundColour*/, bool isMouseOver)
{
	g.setColour(getOwnerView()->findColour(isSelected() ? defaultHighlightedTextColourId : treeIconColourId));
	TreeViewItem::paintOpenCloseButton(g, area, getOwnerView()->findColour(defaultIconColourId), isMouseOver);
}

void JucerTreeViewBase::paintIcon(Graphics& g, Rectangle<float> area)
{
	g.setColour(getContentColour(true));
	getIcon().draw(g, area, isIconCrossedOut());
	textX = roundToInt(area.getRight()) + 7;
}

void JucerTreeViewBase::paintItem(Graphics& g, int width, int height)
{

	ignoreUnused(width, height);

	auto bounds = g.getClipBounds().withY(0).withHeight(height).toFloat();

	//g.setColour(juce::Colour(0, 255, 0));
	g.setColour(getOwnerView()->findColour(treeIconColourId).withMultipliedAlpha(0.4f));
	g.fillRect(bounds.removeFromBottom(0.5f).reduced(5, 0));
}

Colour JucerTreeViewBase::getContentColour(bool isIcon) const
{
	////  text color
	//return Colours::red;
	if (isMissing())      return Colours::red;
	if (isSelected())     return getOwnerView()->findColour(defaultHighlightedTextColourId);
	if (hasWarnings())    return getOwnerView()->findColour(defaultHighlightColourId);

	return getOwnerView()->findColour(isIcon ? treeIconColourId : defaultTextColourId);


}

void JucerTreeViewBase::paintContent(Graphics& g, Rectangle<int> area)
{
	g.setFont(getFont());
	g.setColour(getContentColour(false));

	g.drawFittedText(getDisplayName(), area, Justification::centredLeft, 1, 1.0f);
}

Component* JucerTreeViewBase::createItemComponent()
{
	return new TreeItemComponent(*this);
}



void JucerTreeViewBase::showRenameBox()
{
	Rectangle<int> r(getItemPosition(true));
	r.setLeft(r.getX() + textX);
	r.setHeight(getItemHeight());

	//new RenameTreeItemCallback(*this, *getOwnerView(), r);
}

void JucerTreeViewBase::itemClicked(const MouseEvent& e)
{
	if (e.mods.isPopupMenu())
	{
		if (getOwnerView()->getNumSelectedItems() > 1)
			showMultiSelectionPopupMenu();
		else
			showPopupMenu();
	}
	else if (isSelected())
	{
		itemSelectionChanged(true);
	}
}

void JucerTreeViewBase::deleteItem() {}
void JucerTreeViewBase::deleteAllSelectedItems() {}
void JucerTreeViewBase::showDocument() {}
void JucerTreeViewBase::showPopupMenu() {}
void JucerTreeViewBase::showAddMenu() {}
void JucerTreeViewBase::showMultiSelectionPopupMenu() {}


class JucerTreeViewBase::ItemSelectionTimer : public Timer
{
public:
	ItemSelectionTimer(JucerTreeViewBase& tvb) : owner(tvb) {}

	void timerCallback() override { owner.invokeShowDocument(); }

private:
	JucerTreeViewBase& owner;
	JUCE_DECLARE_NON_COPYABLE(ItemSelectionTimer)
};

void JucerTreeViewBase::itemSelectionChanged(bool isNowSelected)
{
	if (isNowSelected)
	{
		delayedSelectionTimer.reset(new ItemSelectionTimer(*this));
		delayedSelectionTimer->startTimer(getMillisecsAllowedForDragGesture());
	}
	else
	{
		cancelDelayedSelectionTimer();
	}
}

void JucerTreeViewBase::invokeShowDocument()
{
	cancelDelayedSelectionTimer();
	showDocument();
}

void JucerTreeViewBase::itemDoubleClicked(const MouseEvent&)
{
	invokeShowDocument();
}

void JucerTreeViewBase::cancelDelayedSelectionTimer()
{
	delayedSelectionTimer.reset();
}

void JucerTreeViewBase::handlePopupMenuResult(int)
{
}



static void treeViewMenuItemChosen(int resultCode, WeakReference<JucerTreeViewBase> item)
{
	if (item != nullptr)
		item->handlePopupMenuResult(resultCode);
}

void JucerTreeViewBase::launchPopupMenu(PopupMenu& m)
{
	m.showMenuAsync(PopupMenu::Options(),
		ModalCallbackFunction::create(treeViewMenuItemChosen, WeakReference<JucerTreeViewBase>(this)));
}


//ProjectContentComponent* JucerTreeViewBase::getProjectContentComponent() const
//{
//	for (Component* c = getOwnerView(); c != nullptr; c = c->getParentComponent())
//		if (ProjectContentComponent* pcc = dynamic_cast<ProjectContentComponent*> (c))
//			return pcc;
//
//	return nullptr;
//}






FileTreeItemBase* GroupItem::createSubItem(const Project::Item& child) 
{
	if (child.isGroup())
	{
		if (searchFilter.isNotEmpty() && isGroupEmpty(child))
			return nullptr;

		return new GroupItem(child, searchFilter);
	}

	if (child.isFile())
	{
		if (child.getName().containsIgnoreCase(searchFilter))
			return new SourceFileItem(child);

		return nullptr;
	}

	jassertfalse;
	return nullptr;
}










Project::Item::Item(Project& p, const ValueTree& s, bool isModuleCode)
	: project(p), state(s), belongsToModule(isModuleCode)
{
}

Project::Item::Item(const Item& other)
	: project(other.project), state(other.state), belongsToModule(other.belongsToModule)
{
}

Project::Item Project::Item::createCopy() { Item i(*this); i.state = i.state.createCopy(); return i; }

String Project::Item::getID() const { return state[Ids::ID]; }
void Project::Item::setID(const String& newID) { state.setProperty(Ids::ID, newID, nullptr); }

Drawable* Project::Item::loadAsImageFile() const
{
	const MessageManagerLock mml(ThreadPoolJob::getCurrentThreadPoolJob());

	if (!mml.lockWasGained())
		return nullptr;

	return isValid() ? Drawable::createFromImageFile(getFile())
		: nullptr;
}

Project::Item Project::Item::createGroup(Project& project, const String& name, const String& uid, bool isModuleCode)
{
	Item group(project, ValueTree(Ids::GROUP), isModuleCode);
	group.setID(uid);
	group.initialiseMissingProperties();
	group.getNameValue() = name;
	return group;
}

bool Project::Item::isFile() const { return state.hasType(Ids::FILE); }
bool Project::Item::isGroup() const { return state.hasType(Ids::GROUP) || isMainGroup(); }
bool Project::Item::isMainGroup() const { return state.hasType(Ids::MAINGROUP); }

bool Project::Item::isImageFile() const
{
	return isFile() && (ImageFileFormat::findImageFormatForFileExtension(getFile()) != nullptr
		|| getFile().hasFileExtension("svg"));
}

Project::Item Project::Item::findItemWithID(const String& targetId) const
{
	if (state[Ids::ID] == targetId)
		return *this;

	if (isGroup())
	{
		for (auto i = getNumChildren(); --i >= 0;)
		{
			auto found = getChild(i).findItemWithID(targetId);

			if (found.isValid())
				return found;
		}
	}

	return Item(project, ValueTree(), false);
}

bool Project::Item::canContain(const Item& child) const
{
	if (isFile())
		return false;

	if (isGroup())
		return child.isFile() || child.isGroup();

	jassertfalse;
	return false;
}

bool Project::Item::shouldBeAddedToTargetProject() const { return isFile(); }

Value Project::Item::getShouldCompileValue() { return state.getPropertyAsValue(Ids::compile, getUndoManager()); }
bool Project::Item::shouldBeCompiled() const { return state[Ids::compile]; }

Value Project::Item::getShouldAddToBinaryResourcesValue() { return state.getPropertyAsValue(Ids::resource, getUndoManager()); }
bool Project::Item::shouldBeAddedToBinaryResources() const { return state[Ids::resource]; }

Value Project::Item::getShouldAddToXcodeResourcesValue() { return state.getPropertyAsValue(Ids::xcodeResource, getUndoManager()); }
bool Project::Item::shouldBeAddedToXcodeResources() const { return state[Ids::xcodeResource]; }

Value Project::Item::getShouldInhibitWarningsValue() { return state.getPropertyAsValue(Ids::noWarnings, getUndoManager()); }
bool Project::Item::shouldInhibitWarnings() const { return state[Ids::noWarnings]; }

bool Project::Item::isModuleCode() const { return belongsToModule; }

Value Project::Item::getCompilerFlagSchemeValue() { return state.getPropertyAsValue(Ids::compilerFlagScheme, getUndoManager()); }
String Project::Item::getCompilerFlagSchemeString() const { return state[Ids::compilerFlagScheme]; }

void Project::Item::setCompilerFlagScheme(const String& scheme)
{
	//state.getPropertyAsValue(Ids::compilerFlagScheme, getUndoManager()).setValue(scheme);
}

void Project::Item::clearCurrentCompilerFlagScheme()
{
	//state.removeProperty(Ids::compilerFlagScheme, getUndoManager());
}

String Project::Item::getFilePath() const
{
	//if (isFile())
	//	return state[Ids::file].toString();

	return {};
}

File Project::Item::getFile() const
{
	if (isFile())
		return project.resolveFilename(state[Ids::file].toString());

	return {};
}

void Project::Item::setFile(const File& file)
{
	//setFile(RelativePath(project.getRelativePathForFile(file), RelativePath::projectFolder));
	//jassert(getFile() == file);
}
//
//void Project::Item::setFile(const RelativePath& file)
//{
//	jassert(isFile());
//	state.setProperty(Ids::file, file.toUnixStyle(), getUndoManager());
//	state.setProperty(Ids::name, file.getFileName(), getUndoManager());
//}

bool Project::Item::renameFile(const File& newFile)
{
	auto oldFile = getFile();

	if (oldFile.moveFileTo(newFile)
		|| (newFile.exists() && !oldFile.exists()))
	{
		setFile(newFile);
		//ProjucerApplication::getApp().openDocumentManager.fileHasBeenRenamed(oldFile, newFile);
		return true;
	}

	return false;
}
//
//bool Project::Item::containsChildForFile(const RelativePath& file) const
//{
//	return state.getChildWithProperty(Ids::file, file.toUnixStyle()).isValid();
//}

Project::Item Project::Item::findItemForFile(const File& file) const
{
	if (getFile() == file)
		return *this;

	if (isGroup())
	{
		for (auto i = getNumChildren(); --i >= 0;)
		{
			auto found = getChild(i).findItemForFile(file);

			if (found.isValid())
				return found;
		}
	}

	return Item(project, ValueTree(), false);
}

File Project::Item::determineGroupFolder() const
{
	jassert(isGroup());
	File f;

	for (int i = 0; i < getNumChildren(); ++i)
	{
		f = getChild(i).getFile();

		if (f.exists())
			return f.getParentDirectory();
	}

	auto parent = getParent();
	if (parent != *this)
	{
		f = parent.determineGroupFolder();

		if (f.getChildFile(getName()).isDirectory())
			f = f.getChildFile(getName());
	}
	else
	{
		f = project.getProjectFolder();

		if (f.getChildFile("Source").isDirectory())
			f = f.getChildFile("Source");
	}

	return f;
}

void Project::Item::initialiseMissingProperties()
{
	/*if (!state.hasProperty(Ids::ID))
		setID(createAlphaNumericUID());*/

	if (isFile())
	{
		state.setProperty(Ids::name, getFile().getFileName(), nullptr);
	}
	else if (isGroup())
	{
		for (auto i = getNumChildren(); --i >= 0;)
			getChild(i).initialiseMissingProperties();
	}
}

Value Project::Item::getNameValue()
{
	return state.getPropertyAsValue(Ids::name, getUndoManager());
}

String Project::Item::getName() const
{
	return state[Ids::name];
}

void Project::Item::addChild(const Item& newChild, int insertIndex)
{
	state.addChild(newChild.state, insertIndex, getUndoManager());
}

void Project::Item::removeItemFromProject()
{
	state.getParent().removeChild(state, getUndoManager());
}

Project::Item Project::Item::getParent() const
{
	if (isMainGroup() || !isGroup())
		return *this;

	return { project, state.getParent(), belongsToModule };
}




void Project::Item::sortAlphabetically(bool keepGroupsAtStart, bool recursive)
{
	/*sortGroup(state, keepGroupsAtStart, getUndoManager());

	if (recursive)
		for (auto i = getNumChildren(); --i >= 0;)
			getChild(i).sortAlphabetically(keepGroupsAtStart, true);*/
}

Project::Item Project::Item::getOrCreateSubGroup(const String& name)
{
	for (auto i = state.getNumChildren(); --i >= 0;)
	{
		auto child = state.getChild(i);
		if (child.getProperty(Ids::name) == name && child.hasType(Ids::GROUP))
			return { project, child, belongsToModule };
	}

	return addNewSubGroup(name, -1);
}
String createGUID(const String& seed)
{
	auto hex = MD5((seed + "_guidsalt").toUTF8()).toHexString().toUpperCase();

	return "{" + hex.substring(0, 8)
		+ "-" + hex.substring(8, 12)
		+ "-" + hex.substring(12, 16)
		+ "-" + hex.substring(16, 20)
		+ "-" + hex.substring(20, 32)
		+ "}";
}
Project::Item Project::Item::addNewSubGroup(const String& name, int insertIndex)
{
	auto newID = createGUID(getID() + name + String(getNumChildren()));

	//int n = 0;
	//while (project.getMainGroup().findItemWithID(newID).isValid())
	//	newID = createGUID(newID + String(++n));

	auto group = createGroup(project, name, newID, belongsToModule);

	jassert(canContain(group));
	addChild(group, insertIndex);
	return group;
}

bool Project::Item::addFileAtIndex(const File& file, int insertIndex, const bool shouldCompile)
{
	if (file == File() || file.isHidden() || file.getFileName().startsWithChar('.'))
		return false;

	if (file.isDirectory())
	{
		auto group = addNewSubGroup(file.getFileName(), insertIndex);

		for (DirectoryIterator iter(file, false, "*", File::findFilesAndDirectories); iter.next();)
			if (!project.getMainGroup().findItemForFile(iter.getFile()).isValid())
				group.addFileRetainingSortOrder(iter.getFile(), shouldCompile);
	}
	else if (file.existsAsFile())
	{
		if (!project.getMainGroup().findItemForFile(file).isValid())
			addFileUnchecked(file, insertIndex, shouldCompile);
	}
	else
	{
		jassertfalse;
	}

	return true;
}
Project::Item Project::getMainGroup()
{
	return { *this, projectRoot.getChildWithName(Ids::MAINGROUP), false };
}

static File lastDocumentOpened;
const char* Project::projectFileExtension = ".jucer";

File Project::getLastDocumentOpened() 
{
	return lastDocumentOpened;
}
void Project::setLastDocumentOpened(const File& file) 
{
	lastDocumentOpened = file;
}


bool Project::Item::addFileRetainingSortOrder(const File& file, bool shouldCompile)
{
	/*auto wasSortedGroupsNotFirst = isGroupSorted(state, false);
	auto wasSortedGroupsFirst = isGroupSorted(state, true);

	if (!addFileAtIndex(file, 0, shouldCompile))
		return false;

	if (wasSortedGroupsNotFirst || wasSortedGroupsFirst)
		sortAlphabetically(wasSortedGroupsFirst, false);*/

	return true;
}

void Project::Item::addFileUnchecked(const File& file, int insertIndex, const bool shouldCompile)
{
	Item item(project, ValueTree(Ids::FILE), belongsToModule);
	item.initialiseMissingProperties();
	item.getNameValue() = file.getFileName();
	item.getShouldCompileValue() = shouldCompile && file.hasFileExtension(fileTypesToCompileByDefault);
	item.getShouldAddToBinaryResourcesValue() = project.shouldBeAddedToBinaryResourcesByDefault(file);

	if (canContain(item))
	{
		item.setFile(file);
		addChild(item, insertIndex);
	}
}
//
//bool Project::Item::addRelativeFile(const RelativePath& file, int insertIndex, bool shouldCompile)
//{
//	Item item(project, ValueTree(Ids::FILE), belongsToModule);
//	item.initialiseMissingProperties();
//	item.getNameValue() = file.getFileName();
//	item.getShouldCompileValue() = shouldCompile;
//	item.getShouldAddToBinaryResourcesValue() = project.shouldBeAddedToBinaryResourcesByDefault(file);
//
//	if (canContain(item))
//	{
//		item.setFile(file);
//		addChild(item, insertIndex);
//		return true;
//	}
//
//	return false;
//}
//




Icon Project::Item::getIcon(bool isOpen) const
{
	auto& icons = getIcons();

	if (isFile())
	{
		if (isImageFile())
			return Icon(icons.imageDoc, Colours::transparentBlack);

		return { icons.file, Colours::transparentBlack };
	}

	if (isMainGroup())
		return { icons.juceLogo, Colours::orange };

	return { isOpen ? icons.openFolder : icons.closedFolder, Colours::transparentBlack };
}

bool Project::Item::isIconCrossedOut() const
{
	return isFile()
		&& !(shouldBeCompiled()
			|| shouldBeAddedToBinaryResources()
			|| getFile().hasFileExtension(headerFileExtensions));
}

bool Project::Item::needsSaving() const noexcept
{
	/*auto& odm = ProjucerApplication::getApp().openDocumentManager;

	if (odm.anyFilesNeedSaving())
	{
		for (int i = 0; i < odm.getNumOpenDocuments(); ++i)
		{
			auto* doc = odm.getOpenDocument(i);
			if (doc->needsSaving() && doc->getFile() == getFile())
				return true;
		}
	}*/

	return false;
}





String replacePreprocessorDefs(const StringPairArray& definitions, String sourceString)
{
	for (int i = 0; i < definitions.size(); ++i)
	{
		const String key(definitions.getAllKeys()[i]);
		const String value(definitions.getAllValues()[i]);

		sourceString = sourceString.replace("${" + key + "}", value);
	}

	return sourceString;
}
File Project::resolveFilename(String filename) const
{
	if (filename.isEmpty())
		return {};

	filename = replacePreprocessorDefs(getPreprocessorDefs(), filename);

#if ! JUCE_WINDOWS
	if (filename.startsWith("~"))
		return File::getSpecialLocation(File::userHomeDirectory).getChildFile(filename.trimCharactersAtStart("~/"));
#endif

	if (FileHelpers::isAbsolutePath(filename))
		return File::createFileWithoutCheckingPath(FileHelpers::currentOSStylePath(filename)); // (avoid assertions for windows-style paths)

	return getFile().getSiblingFile(FileHelpers::currentOSStylePath(filename));
}

String Project::getRelativePathForFile(const File& file) const
{
	auto filename = file.getFullPathName();

	auto relativePathBase = getFile().getParentDirectory();

	auto p1 = relativePathBase.getFullPathName();
	auto p2 = file.getFullPathName();

	while (p1.startsWithChar(File::getSeparatorChar()))
		p1 = p1.substring(1);

	while (p2.startsWithChar(File::getSeparatorChar()))
		p2 = p2.substring(1);

	if (p1.upToFirstOccurrenceOf(File::getSeparatorString(), true, false)
		.equalsIgnoreCase(p2.upToFirstOccurrenceOf(File::getSeparatorString(), true, false)))
	{
		filename = FileHelpers::getRelativePathFrom(file, relativePathBase);
	}

	return filename;
}
