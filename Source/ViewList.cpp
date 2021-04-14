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




/*
This is actually a copy of : 

void LinearPanel::setPosition(int x, int y) {
    setBounds(x, y, getWidth(), getHeight());
}

How would I just reference this in an effort to make cleaner code?


*/
void ViewList::setPosition(int x, int y) {
    setBounds(x, y, getWidth(), getHeight());
}
 