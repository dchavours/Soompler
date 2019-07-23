#include "MainPanel.h"
#include "Settings.h"
#include "Strings.h"

//==============================================================================
MainPanel::MainPanel (SoomplerAudioProcessor& processor) : stateManager(processor.getStateManager()),
    processor(processor), editor(this)
{
    setSize(Settings::WINDOW_WIDTH, Settings::WINDOW_HEIGHT);
    setName("mainPanel");

    backgroundImage = ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);

    volumeKnob.reset (new Slider ("volume knob"));
    addAndMakeVisible(volumeKnob.get());
    editor.addToGuiEditor(volumeKnob.get());
    volumeKnob->setTooltip (TRANS("volume"));
    //volumeKnob->setRange (0.0, 1.0, 0.01);
    //volumeKnob->setValue(0.5);
    volumeKnob->setSliderStyle (Slider::Rotary);
    volumeKnob->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    //volumeKnob->addListener (this);
    volumeKnob->setBounds (60, 230, 50, 50);
    volumeAttachment.reset(new SliderAttachment(stateManager, "volume", *volumeKnob));

    volumeKnobLabel.reset (new Label ("volume knob label",
                                      TRANS("Volume\n")));
    addAndMakeVisible(volumeKnobLabel.get());
    editor.addToGuiEditor(volumeKnobLabel.get());
    volumeKnobLabel->setFont (Font (15.00f, Font::plain));
    volumeKnobLabel->setJustificationType (Justification::centredLeft);
    volumeKnobLabel->setEditable (false, false, false);
    volumeKnobLabel->setColour (TextEditor::textColourId, Colours::black);
    volumeKnobLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    volumeKnobLabel->setBounds (60, 215, 64, 24);

    openFileButton.reset (new ImageButton ("open file button"));
    addAndMakeVisible (openFileButton.get());
    editor.addToGuiEditor (openFileButton.get());
    openFileButton->setTooltip (TRANS("Open Sample\n"));
    openFileButton->setButtonText (TRANS("new button"));
    openFileButton->addListener (this);
    openFileButton->setImages (false, true, true,
                               ImageCache::getFromMemory (BinaryData::openFile_png, BinaryData::openFile_pngSize),
                               1.000f, Colour (0x00000000),
                               Image(), 1.000f, Colour (0x00000000),
                               Image(), 1.000f, Colour (0x00000000));
    openFileButton->setBounds (8, 8, 39, 32);

    aboutButton.reset (new ImageButton ("about button"));
    addAndMakeVisible (aboutButton.get());
    editor.addToGuiEditor (aboutButton.get());
    aboutButton->setTooltip (TRANS("About Program\n"));
    aboutButton->setButtonText (TRANS("About program"));
    aboutButton->addListener (this);
    aboutButton->setImages (false, true, true,
                               ImageCache::getFromMemory (BinaryData::about_png, BinaryData::about_pngSize),
                               1.000f, Colour (0x00000000),
                               Image(), 1.000f, Colour (0x00000000),
                               Image(), 1.000f, Colour (0x00000000));
    aboutButton->setBounds (52, 8, 39, 32);
    
    loopButton.reset (new ToggledImageButton ("loop button"));
    addAndMakeVisible (loopButton.get());
    editor.addToGuiEditor (loopButton.get());
    loopButton->setTooltip (TRANS("Loop sample playback"));
    loopButton->setButtonText (TRANS("Loop"));
    loopButton->addListener (this);
    loopButton->setImages (false, true, true,
                            ImageCache::getFromMemory (BinaryData::loop_png, BinaryData::loop_pngSize),
                            1.000f, Colour (0x00000000),
                            Image(), 1.000f, Colour (0x00000000),
                            Image(), 1.000f, Colour (0x00000000));
    loopButton->setBounds (450, 60, 30, 30);
    loopButton->setEnabled(false);
    

    sampleViewer.reset (new SampleViewer(processor.getThumbnail(), processor, processor));
    addAndMakeVisible (sampleViewer.get());
    editor.addToGuiEditor (sampleViewer.get());
    sampleViewer->setName ("SampleViewer");
    sampleViewer->setBounds(Settings::SAMPLE_VIEWER_BOUNDS);
    // not visible until sample is loaded
    sampleViewer->setVisible(false);
    processor.addSampleInfoListener(sampleViewer);

    loadSampleTip.reset (new Label ("no sample loaded label",
                                    TRANS(Strings::NO_SAMPLE_LOADED_TEXT)));
    addAndMakeVisible (loadSampleTip.get());
    editor.addToGuiEditor (loadSampleTip.get());
    loadSampleTip->setFont (Font (27.90f, Font::plain));
    loadSampleTip->setJustificationType (Justification::centred);
    loadSampleTip->setEditable (false, false, false);
    loadSampleTip->setColour (Label::textColourId, Colour (0xff0a0000));

    loadSampleTip->setBounds (20, 136, 460, 56);

    pianoRoll.reset (new PianoRoll(processor, processor));
    pianoRoll->setBounds (Settings::PIANO_ROLL_BOUNDS);
    pianoRoll->setName ("piano roll component");
    processor.addSampleInfoListener(pianoRoll);

    addAndMakeVisible (pianoRoll.get());
    editor.addToGuiEditor (pianoRoll.get());

    // ADSR controls
    attackKnob.reset (new Slider ("attack knob"));
    addAndMakeVisible(attackKnob.get());
    editor.addToGuiEditor(attackKnob.get());
    attackKnob->setTooltip (TRANS("Attack"));
//    attackKnob->setRange (0.0, Settings::MAX_ATTACK_TIME, 0.01);
//    attackKnob->setValue(Settings::DEFAULT_ATTACK_TIME);
    attackKnob->setSliderStyle (Slider::Rotary);
    attackKnob->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    //attackKnob->addListener (this);
    attackKnob->setBounds (150, 230, 50, 50);
    attackAttachment.reset(new SliderAttachment(stateManager, "attack", *attackKnob));

    attackKnobLabel.reset (new Label ("attack knob label",
                                      TRANS("Attack\n")));
    addAndMakeVisible(attackKnobLabel.get());
    editor.addToGuiEditor(attackKnobLabel.get());
    attackKnobLabel->setFont (Font (15.00f, Font::plain));
    attackKnobLabel->setJustificationType (Justification::centredLeft);
    attackKnobLabel->setEditable (false, false, false);
    attackKnobLabel->setColour (TextEditor::textColourId, Colours::black);
    attackKnobLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    attackKnobLabel->setBounds (150, 215, 64, 24);

    decayKnob.reset (new Slider ("decay knob"));
    addAndMakeVisible(decayKnob.get());
    editor.addToGuiEditor(decayKnob.get());
    decayKnob->setTooltip (TRANS("Decay"));
//    decayKnob->setRange (0.0, Settings::MAX_DECAY_TIME, 0.01);
//    decayKnob->setValue(Settings::DEFAULT_DECAY_TIME);
    decayKnob->setSliderStyle (Slider::Rotary);
    decayKnob->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    //decayKnob->addListener (this);
    decayKnob->setBounds (200, 230, 50, 50);
    decayAttachment.reset(new SliderAttachment(stateManager, "decay", *decayKnob));

    decayKnobLabel.reset (new Label ("decay knob label",
                                      TRANS("Decay\n")));
    addAndMakeVisible(decayKnobLabel.get());
    editor.addToGuiEditor(decayKnobLabel.get());
    decayKnobLabel->setFont (Font (15.00f, Font::plain));
    decayKnobLabel->setJustificationType (Justification::centredLeft);
    decayKnobLabel->setEditable (false, false, false);
    decayKnobLabel->setColour (TextEditor::textColourId, Colours::black);
    decayKnobLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    decayKnobLabel->setBounds (200, 215, 64, 24);

    sustainKnob.reset (new Slider ("sustain knob"));
    addAndMakeVisible(sustainKnob.get());
    editor.addToGuiEditor(sustainKnob.get());
    sustainKnob->setTooltip (TRANS("Sustain"));
//    sustainKnob->setRange (0.0, 1.0, 0.01);
//    sustainKnob->setValue(Settings::DEFAULT_SUSTAIN_LEVEL);
    sustainKnob->setSliderStyle (Slider::Rotary);
    sustainKnob->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
   // sustainKnob->addListener (this);
    sustainKnob->setBounds (250, 230, 50, 50);
    sustainAttachment.reset(new SliderAttachment(stateManager, "sustain", *sustainKnob));

    sustainKnobLabel.reset (new Label ("sustain knob label",
                                      TRANS("Sustain\n")));
    addAndMakeVisible(sustainKnobLabel.get());
    editor.addToGuiEditor(sustainKnobLabel.get());
    sustainKnobLabel->setFont (Font (15.00f, Font::plain));
    sustainKnobLabel->setJustificationType (Justification::centredLeft);
    sustainKnobLabel->setEditable (false, false, false);
    sustainKnobLabel->setColour (TextEditor::textColourId, Colours::black);
    sustainKnobLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    sustainKnobLabel->setBounds (250, 215, 64, 24);

    releaseKnob.reset (new Slider ("release knob"));
    addAndMakeVisible(releaseKnob.get());
    editor.addToGuiEditor(releaseKnob.get());
    releaseKnob->setTooltip (TRANS("Release"));
//    releaseKnob->setRange (0.0, Settings::MAX_RELEASE_TIME, 0.01);
//    releaseKnob->setValue(Settings::DEFAULT_RELEASE_TIME);
    releaseKnob->setSliderStyle (Slider::Rotary);
    releaseKnob->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    //releaseKnob->addListener (this);
    releaseKnob->setBounds (300, 230, 50, 50);
    releaseAttachment.reset(new SliderAttachment(stateManager, "release", *releaseKnob));

    releaseKnobLabel.reset (new Label ("release knob label",
                                      TRANS("Release\n")));
    addAndMakeVisible(releaseKnobLabel.get());
    editor.addToGuiEditor(releaseKnobLabel.get());
    releaseKnobLabel->setFont (Font (15.00f, Font::plain));
    releaseKnobLabel->setJustificationType (Justification::centredLeft);
    releaseKnobLabel->setEditable (false, false, false);
    releaseKnobLabel->setColour (TextEditor::textColourId, Colours::black);
    releaseKnobLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    releaseKnobLabel->setBounds (300, 215, 64, 24);

    attackKnob->setEnabled(false);
    decayKnob->setEnabled(false);
    sustainKnob->setEnabled(false);
    releaseKnob->setEnabled(false);

    // Reverse sample
    reverseButton.reset(new ToggleButton(TRANS("Reverse\n")));
    addAndMakeVisible(reverseButton.get());
    editor.addToGuiEditor(reverseButton.get());
    reverseButton->setBounds(380, 230, 100, 50);
    reverseButton->addListener(this);
    reverseButton->setEnabled(false);

    // add GUI editor last
    // it ensures that gui overlay will work properly
    editor.initOverlay();
}

MainPanel::~MainPanel()
{
    volumeAttachment = nullptr;
    attackAttachment = nullptr;
    decayAttachment = nullptr;
    sustainAttachment = nullptr;
    releaseAttachment = nullptr;

    attackKnob = nullptr;
    decayKnob = nullptr;
    sustainKnob = nullptr;
    releaseKnob = nullptr;
    volumeKnob = nullptr;
    volumeKnobLabel = nullptr;
    openFileButton = nullptr;
    aboutButton = nullptr;
    sampleViewer = nullptr;
    loadSampleTip = nullptr;
    pianoRoll = nullptr;
    reverseButton = nullptr;
    loopButton = nullptr;
}

//==============================================================================
void MainPanel::paint (Graphics& g)
{
    g.fillAll (Colour (0xff323e44));
    g.drawImage(backgroundImage, Rectangle<float>(0, 0, this->getWidth(), this->getHeight()));

    editor.paintBackOverlay(g);

    // draw menu panel
    g.setGradientFill(Settings::MAIN_PANEL_GRADIENT);
    g.fillRect(0, 0, this->getWidth(), Settings::MAIN_PANEL_HEIGHT);

}

void MainPanel::resized()
{
}

void MainPanel::sliderValueChanged (Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == volumeKnob.get())
    {
        processor.setVolume(volumeKnob.get()->getValue());
    }
    else if (sliderThatWasMoved == attackKnob.get())
    {
        adsrParams.attack = attackKnob->getValue();
    }
    else if (sliderThatWasMoved == decayKnob.get())
    {
        adsrParams.decay = decayKnob->getValue();
    }
    else if (sliderThatWasMoved == sustainKnob.get())
    {
        adsrParams.sustain = sustainKnob->getValue();
    }
    else if (sliderThatWasMoved == releaseKnob.get())
    {
        adsrParams.release = releaseKnob->getValue();
    }

    processor.setAdsrParams(adsrParams);
}

void MainPanel::buttonClicked (Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == openFileButton.get())
    {
        openFileButtonClicked();
    }
    else if (buttonThatWasClicked == aboutButton.get())
    {
        aboutButtonClicked();
    }
    else if (buttonThatWasClicked == loopButton.get())
    {
        loopButtonClicked();
    }
    else if (buttonThatWasClicked == reverseButton.get()) {
        reverseButtonClicked();
    }
//    else if (buttonThatWasClicked == playButton.get())
//    {
//        playSampleButtonClicked();
//    }
//    else if (buttonThatWasClicked == stopButton.get())
//    {
//        stopSampleButtonClicked();
//    }
}

float MainPanel::getVolume() const
{
    return volumeKnob.get()->getValue();
}

void MainPanel::openFileButtonClicked()
{
    FileChooser chooser(Strings::OPEN_FILE_DIALOG_TEXT,
                        File::getSpecialLocation(File::userHomeDirectory),
                        "*.mp3;*.wav;*.ogg", false);

    if (chooser.browseForFileToOpen())
    {
        File sampleFile (chooser.getResult());

        processor.loadSample(sampleFile);

        //playButton->setEnabled(true);

        repaint();
    }
}

void MainPanel::aboutButtonClicked()
{
    // show about dialog
}

void MainPanel::playSampleButtonClicked()
{
    processor.playSample();
    //playButton->setVisible(false);
    //stopButton->setVisible(true);
}

void MainPanel::stopSampleButtonClicked()
{
    processor.stopSamplePlayback();
}


void MainPanel::transportStateChanged(TransportState state)
{
    switch (state) {
    case Ready:
        loadSampleTip->setVisible(false);
        sampleViewer->setVisible(true);
        //playButton->setVisible(true);

        attackKnob->setEnabled(true);
        decayKnob->setEnabled(true);
        sustainKnob->setEnabled(true);
        releaseKnob->setEnabled(true);
        
        loopButton->setEnabled(true);
        reverseButton->setEnabled(true);
        break;
    case Starting:
        break;
    case Stopped:
        //stopButton->setVisible(false);
        //playButton->setVisible(true);
        break;
    default:
        break;
    }
}

void MainPanel::loopButtonClicked() {
    processor.setLoopEnabled(loopButton->isToggled());
}

void MainPanel::reverseButtonClicked() {
    processor.reverseSample();
}

bool MainPanel::isInterestedInFileDrag(const juce::StringArray &files) {
    for (auto fileName : files) {
        for (auto format : Settings::SUPPORTED_FILE_FORMATS) {
            if (fileName.endsWith(format)) {
                return true;
            }
        }
    }
    
    return false;
}


void MainPanel::filesDropped(const juce::StringArray &files, int x, int y) {
    // by now only first dropped file will be loaded
    auto fileName = files[0];
    
    processor.loadSample(File(fileName));
}



