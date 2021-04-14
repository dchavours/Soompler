/*
  ==============================================================================

    ViewList.cpp
    Created: 13 Apr 2021 10:03:11pm
    Author:  deckard

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ViewList.h"
#include "LinearPanel.h"

//==============================================================================
ViewList::ViewList()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

ViewList::~ViewList()
{
}


void ViewList::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}



void LinearPanel::setPosition(int x, int y) {
    setBounds(x, y, getWidth(), getHeight());
}
 