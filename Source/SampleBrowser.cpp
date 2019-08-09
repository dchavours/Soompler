/*
  ==============================================================================

    SampleBrowser.cpp
    Created: 9 Aug 2019 5:05:47pm
    Author:  artyom

  ==============================================================================
*/

#include "SampleBrowser.h"
#include "Settings.h"
#include "SamplePreviewComponent.h"

SampleBrowser::SampleBrowser(FileListener& listener) : Component("SampleBrowser"), fileListener(listener)
{
    background = ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);
    previewComponent.reset(new SamplePreviewComponent());
    browser.reset(new FileBrowserComponent(FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles,
                                           Settings::INITIAL_DIRECTORY, Settings::BROWSER_FILE_FILTER.get(), nullptr));
    browser->addListener(this);

    addAndMakeVisible(browser.get());
    addAndMakeVisible(previewComponent.get());
}

void SampleBrowser::resized()
{
    FlexBox fb;

    fb.flexDirection = FlexBox::Direction::row;

    fb.items.add(FlexItem(*(browser.get())).withFlex(3));
    fb.items.add(FlexItem(*(previewComponent.get())).withFlex(1));

    fb.performLayout(getLocalBounds().toFloat());
}

void SampleBrowser::selectionChanged()
{

}

void SampleBrowser::fileClicked(const File &file, const MouseEvent &e)
{

}

void SampleBrowser::fileDoubleClicked(const File &file)
{
    fileListener.fileRecieved(file);
}

void SampleBrowser::browserRootChanged(const File &newRoot)
{

}

void SampleBrowser::paint(Graphics &g)
{
   //g.drawImage(background, Rectangle<float>(this->getX(), this->getY(), this->getWidth(), this->getHeight()));
    Component::paint(g);
}