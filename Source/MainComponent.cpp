/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "TreeViewTest.h"
//==============================================================================
MainComponent::MainComponent()
{
	tree.setMultiSelectEnabled(true);
	tree.setRootItemVisible(true);
	tree.setDefaultOpenness(true);
	tree.setIndentSize(14);
	tree.getViewport()->setScrollBarThickness(6);
	//tree.setColour(TreeView::backgroundColourId, juce::Colours::black);

	auto juceProjectFileName = juce::File::getCurrentWorkingDirectory().getFullPathName() + "/test.jucer";

	auto p = new Project(File(juceProjectFileName));
 
	tree.setRootItem(new GroupItem(p->getMainGroup()));

	addAndMakeVisible(tree);





	
    setSize (600, 400);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setFont (Font (16.0f));
    //g.setColour (Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
	tree.setBounds(getBounds());
}
