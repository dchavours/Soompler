/*
  ==============================================================================

    ViewList.h
    Created: 13 Apr 2021 10:03:11pm
    Author:  deckard

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ViewList    : public Component
{
public:
    ViewList();
    ~ViewList();

    void paint (Graphics&) override;
    void resized() override;

private:

    TextButton listBtnOne;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ViewList)
};
