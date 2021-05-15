/*
    This file is part of DeCoder.
    Copyright (C) 2021 - Giorgos Koulouris

    DeCoder is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DeCoder is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <string>



//==============================================================================
DickoderAudioProcessorEditor::DickoderAudioProcessorEditor (DickoderAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{

    
    
    
    //========================= FOR BACKGROUND IMAGE ==================================
    auto bgImage = juce::ImageCache::getFromMemory(BinaryData::BG_png, BinaryData::BG_pngSize);
    if (! bgImage.isNull())
    {
        bgImageComponent.setImage(bgImage, juce::RectanglePlacement::stretchToFit);
    }
    Component::addAndMakeVisible(bgImageComponent);

    
    
    
    //========================== SLIDER LOOK AND FEEL =================================
    getLookAndFeel().setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::teal);
    getLookAndFeel().setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::darkred);
    getLookAndFeel().setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::cadetblue);


    
    
    //========================== SLIDER AND BUTTON INIT =======================================
    
    //MID GAIN SLIDER
    midGainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    midGainSlider.setSliderSnapsToMousePosition(false);
    midGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 35, 15);
    midGainSlider.setScrollWheelEnabled(true);
    addAndMakeVisible(midGainSlider);
    midGainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MID GAIN", midGainSlider);
    midGainSlider.addListener(this);

    //SIDE GAIN SLIDER
    sideGainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    sideGainSlider.setSliderSnapsToMousePosition(false);
    sideGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 35, 15);
    sideGainSlider.setScrollWheelEnabled(true);
    addAndMakeVisible(sideGainSlider);
    sideGainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "SIDE GAIN", sideGainSlider);
    sideGainSlider.addListener(this);
    

    //ON/OFF IMAGES FOR THE BUTTONS
    auto buttonOffImage = juce::ImageCache::getFromMemory(BinaryData::ButtonOff_png, BinaryData::ButtonOff_pngSize);
    auto buttonOnImage = juce::ImageCache::getFromMemory(BinaryData::ButtonOn_png, BinaryData::ButtonOn_pngSize);
    
    //SOLO MID BUTTON INIT
    addAndMakeVisible(midSoloButton);
    midSoloButton.addListener(this);
    midSoloButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "SOLO MID", midSoloButton);
    midSoloButton.setClickingTogglesState(true);
    midSoloButton.setImages(false, false, true, buttonOffImage, 1.f, juce::Colours::transparentWhite, buttonOffImage, 1.f, juce::Colours::transparentWhite, buttonOnImage, 1.f, juce::Colours::transparentWhite);
    
    //SOLO SIDE BUTTON INIT
    sideSoloButton.addListener(this);
    addAndMakeVisible(sideSoloButton);
    sideSoloButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "SOLO SIDE", sideSoloButton);
    sideSoloButton.setClickingTogglesState(true);
    sideSoloButton.setImages(false, false, true, buttonOffImage, 1.f, juce::Colours::transparentWhite, buttonOffImage, 1.f, juce::Colours::transparentWhite, buttonOnImage, 1.f, juce::Colours::transparentWhite);
    
    //SIDE SPREAD BUTTON INIT
    sideSpreadButton.addListener(this);
    addAndMakeVisible(sideSpreadButton);
    sideSpreadButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "COLLAPSE SIDE", sideSpreadButton);
    sideSpreadButton.setClickingTogglesState(true);
    sideSpreadButton.setImages(false, false, true, buttonOffImage, 1.f, juce::Colours::transparentWhite, buttonOffImage, 1.f, juce::Colours::transparentWhite, buttonOnImage, 1.f, juce::Colours::transparentWhite);
    
    //SOLO LEFT BUTTON INIT
    leftSoloButton.addListener(this);
    addAndMakeVisible(leftSoloButton);
    leftSoloButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "SOLO LEFT", leftSoloButton);
    leftSoloButton.setClickingTogglesState(true);
    leftSoloButton.setImages(false, false, true, buttonOffImage, 1.f, juce::Colours::transparentWhite, buttonOffImage, 1.f, juce::Colours::transparentWhite, buttonOnImage, 1.f, juce::Colours::transparentWhite);
    
    //SOLO RIGHT BUTTON INIT
    rightSoloButton.addListener(this);
    addAndMakeVisible(rightSoloButton);
    rightSoloButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "SOLO RIGHT", rightSoloButton);
    rightSoloButton.setClickingTogglesState(true);
    rightSoloButton.setImages(false, false, true, buttonOffImage, 1.f, juce::Colours::transparentWhite, buttonOffImage, 1.f, juce::Colours::transparentWhite, buttonOnImage, 1.f, juce::Colours::transparentWhite);
    
    //MONO CHECK BUTTON INIT
    monoCheckButton.addListener(this);
    addAndMakeVisible(monoCheckButton);
    monoCheckButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "MONO CHECK", monoCheckButton);
    monoCheckButton.setClickingTogglesState(true);
    monoCheckButton.setImages(false, false, true, buttonOffImage, 1.f, juce::Colours::transparentWhite, buttonOffImage, 1.f, juce::Colours::transparentWhite, buttonOnImage, 1.f, juce::Colours::transparentWhite);
    
    
    //OUTPUT L TRIM SLIDER INIT
    outLeftTrimSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    outLeftTrimSlider.setSliderSnapsToMousePosition(false);
    outLeftTrimSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 35, 15);
    outLeftTrimSlider.setScrollWheelEnabled(true);
    addAndMakeVisible(outLeftTrimSlider);
    outLeftTrimSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "LOUT TRIM", outLeftTrimSlider);
    outLeftTrimSlider.addListener(this);
    
    //OUTPUT R TRIM SLIDER INIT
    outRightTrimSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    outRightTrimSlider.setSliderSnapsToMousePosition(false);
    outRightTrimSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 35, 15);
    outRightTrimSlider.setScrollWheelEnabled(true);
    addAndMakeVisible(outRightTrimSlider);
    outRightTrimSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "ROUT TRIM", outRightTrimSlider);
    outRightTrimSlider.addListener(this);
    
    //LINK BUTTON INIT
    auto linkButtonOnImage = juce::ImageCache::getFromMemory(BinaryData::Link_Button_On_png, BinaryData::Link_Button_On_pngSize);
    auto linkButtonOffImage = juce::ImageCache::getFromMemory(BinaryData::Link_Button_Off_png, BinaryData::Link_Button_Off_pngSize);
    linkButton.setImages(false, true, true, linkButtonOffImage, 1.f, juce::Colours::transparentWhite, linkButtonOffImage, 0.85f, juce::Colours::transparentWhite, linkButtonOnImage, 1.f, juce::Colours::transparentWhite);
    linkButton.addListener(this);
    linkButton.setClickingTogglesState(true);
    addAndMakeVisible(linkButton);
    linkButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "LINK", linkButton);
    
    
    //USED FOR THE FUNCTIONALITY OF THE LINK BUTTON
    oldLeftOutValue = outLeftTrimSlider.getValue();
    
    
    
    
    //========================== PEAK METER INIT ==================================
    auto peakMeterImage = juce::ImageCache::getFromMemory(BinaryData::PeakMeter_png, BinaryData::PeakMeter_pngSize);
    if (! peakMeterImage.isNull())
    {
        leftPeakMeter.setImage(peakMeterImage);
        rightPeakMeter.setImage(peakMeterImage);
    }
    addAndMakeVisible(leftPeakMeter);
    addAndMakeVisible(rightPeakMeter);
    
    
    
    //========================== RMS METER INIT ==================================
    auto rmsMeterImage = juce::ImageCache::getFromMemory(BinaryData::RMSMeter_png, BinaryData::RMSMeter_pngSize);
    if (! rmsMeterImage.isNull())
    {
        leftRmsMeter.setImage(rmsMeterImage);
        rightRmsMeter.setImage(rmsMeterImage);
    }
    addAndMakeVisible(leftRmsMeter);
    addAndMakeVisible(rightRmsMeter);
    
    //========================== LABELS INIT ==================================
    labelFont.setHeight(10);
    borderSize = std::make_unique<juce::BorderSize<int>>(0,0,0,0);
    
    leftPeakLabelPtr = std::make_unique<juce::Label>("peakLabel1", "-inf");
    leftPeakLabelPtr->setJustificationType(juce::Justification::centred);
    leftPeakLabelPtr->setFont(labelFont);
    leftPeakLabelPtr->setBorderSize(*borderSize);
    leftPeakLabelPtr->setColour(juce::Label::textColourId, juce::Colours::teal);
    addAndMakeVisible(*leftPeakLabelPtr);
    
    rightPeakLabelPtr = std::make_unique<juce::Label>("peakLabel2", "-inf");
    rightPeakLabelPtr->setJustificationType(juce::Justification::centred);
    rightPeakLabelPtr->setFont(labelFont);
    rightPeakLabelPtr->setColour(juce::Label::textColourId, juce::Colours::teal);
    rightPeakLabelPtr->setBorderSize(*borderSize);
    addAndMakeVisible(*rightPeakLabelPtr);
    
    auto resetButtonImage = juce::ImageCache::getFromMemory(BinaryData::TransparentButton_png, BinaryData::TransparentButton_pngSize);
    if (! resetButtonImage.isNull() )
    {
        meterResetButton.setImages(false, false, false, resetButtonImage, 1.0f, juce::Colours::transparentWhite, resetButtonImage, 1.0f, juce::Colours::teal, resetButtonImage, 1.0f, juce::Colours::transparentWhite);
    }
    meterResetButton.addListener(this);
    addAndMakeVisible(meterResetButton);

    
    
    
    //========================== GENERAL ==================================
    setSize (600, 200);
    setResizable(false, false);
    startTimer(10);

}

DickoderAudioProcessorEditor::~DickoderAudioProcessorEditor()
{
    stopTimer();
}

//==============================================================================
void DickoderAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}




//=========================== ARRANGING COMPONENTS ====================================
void DickoderAudioProcessorEditor::resized()
{
    bgImageComponent.setBoundsRelative(0.f, 0.f, 1.f, 1.f);
    
    midGainSlider.setBounds(46, 35 , 20, 130);
    sideGainSlider.setBounds(61, 35, 20, 130);
    
    leftSoloButton.setBounds(106, 70, 16, 16);
    rightSoloButton.setBounds(220, 70, 16, 16);
    monoCheckButton.setBounds(334, 70, 16, 16);
    
    midSoloButton.setBounds(106, 114, 16, 16);
    sideSoloButton.setBounds(220, 114, 16, 16);
    sideSpreadButton.setBounds(334, 114, 16, 16);
    
    outLeftTrimSlider.setBounds(463, 35, 20, 130);
    outRightTrimSlider.setBounds(478, 35, 20, 130);
    
    linkButton.setBounds(436, 71, 22 , 58);

    leftPeakLabelPtr->setBounds(509, 175, 28, 13);
    rightPeakLabelPtr->setBounds(541, 175, 28, 13);
    meterResetButton.setBounds(507, 174, 70, 17);


}




//=========================== IMPLEMENTATION OF BUTTON FUNCTIONALITY ====================================
void DickoderAudioProcessorEditor::buttonClicked (juce::Button* button)
{
    
    //=========================== DESELECTING REST OF THE MODES WHEN CLICKING ON A BUTTON ====================================
    if (button == &midSoloButton)
    {
        if (midSoloButton.getToggleState())
        {
            sideSoloButton.setToggleState(false, juce::NotificationType::sendNotification);
            leftSoloButton.setToggleState(false, juce::NotificationType::sendNotification);
            rightSoloButton.setToggleState(false, juce::NotificationType::sendNotification);
            
            midGainSlider.setColour(juce::Slider::trackColourId, juce::Colours::darkgrey);
            sideGainSlider.setColour(juce::Slider::trackColourId, juce::Colours::darkgrey);
        }
        else
        {
            midGainSlider.setColour(juce::Slider::trackColourId, juce::Colours::teal);
            sideGainSlider.setColour(juce::Slider::trackColourId, juce::Colours::teal);
        }
    }

    if (button == &sideSoloButton)
    {
        if (sideSoloButton.getToggleState())
        {
            midSoloButton.setToggleState(false, juce::NotificationType::sendNotification);
            leftSoloButton.setToggleState(false, juce::NotificationType::sendNotification);
            rightSoloButton.setToggleState(false, juce::NotificationType::sendNotification);
            
            midGainSlider.setColour(juce::Slider::trackColourId, juce::Colours::darkgrey);
            sideGainSlider.setColour(juce::Slider::trackColourId, juce::Colours::darkgrey);
        }
        else
        {
            midGainSlider.setColour(juce::Slider::trackColourId, juce::Colours::teal);
            sideGainSlider.setColour(juce::Slider::trackColourId, juce::Colours::teal);
        }
    }

    if (button == &leftSoloButton)
    {
        if (leftSoloButton.getToggleState())
        {
            sideSoloButton.setToggleState(false, juce::NotificationType::sendNotification);
            midSoloButton.setToggleState(false, juce::NotificationType::sendNotification);
            rightSoloButton.setToggleState(false, juce::NotificationType::sendNotification);
            
            midGainSlider.setColour(juce::Slider::trackColourId, juce::Colours::darkgrey);
            sideGainSlider.setColour(juce::Slider::trackColourId, juce::Colours::darkgrey);
        }
        else
        {
            midGainSlider.setColour(juce::Slider::trackColourId, juce::Colours::teal);
            sideGainSlider.setColour(juce::Slider::trackColourId, juce::Colours::teal);
        }
    }
    
    if (button == &rightSoloButton)
    {
        if (rightSoloButton.getToggleState())
        {
            sideSoloButton.setToggleState(false, juce::NotificationType::sendNotification);
            midSoloButton.setToggleState(false, juce::NotificationType::sendNotification);
            leftSoloButton.setToggleState(false, juce::NotificationType::sendNotification);
            
            midGainSlider.setColour(juce::Slider::trackColourId, juce::Colours::darkgrey);
            sideGainSlider.setColour(juce::Slider::trackColourId, juce::Colours::darkgrey);
        }
        else
        {
            midGainSlider.setColour(juce::Slider::trackColourId, juce::Colours::teal);
            sideGainSlider.setColour(juce::Slider::trackColourId, juce::Colours::teal);
        }
    }
    
    if (button == &meterResetButton)
    {
        leftPeakLabelPtr->setText("-Inf", juce::NotificationType::dontSendNotification);
        rightPeakLabelPtr->setText("-Inf", juce::NotificationType::dontSendNotification);
        maxLeftPeakValue = -150;
        maxRightPeakValue = -150;
    }
    
}


//FUNCTION TO CONVERT FLOAT TO 2 DECIMALS
std::string DickoderAudioProcessorEditor::toTwoDecString(float x)
{
    std::string newStr;
    
    if (x > -100)
    {
        x = std::round(100 * x);
        int z = (int)x;
        std::string str = std::to_string(z);
        int size = str.length();
        newStr.assign(str);
        

        newStr[size-2] = '.';
        newStr[size-1] = str[size-2];
        newStr += str[size-1];
    }
    else
    {
        newStr = "-Inf";
    }
   
    return newStr;
}

void DickoderAudioProcessorEditor::timerCallback()
{
    
    //===================== IMPLEMENTING OUTPUT PEAK INDICATOR FUNCTIONALITY ======================
    peakedOutputChannels = audioProcessor.hasPeaksVoid();
    
    if (peakedOutputChannels == 1)
    {
        outLeftTrimSlider.setColour(juce::Slider::thumbColourId, juce::Colours::red);
        outLeftTrimSlider.setColour(juce::Slider::trackColourId, juce::Colours::darkred);
    }
    
    if (peakedOutputChannels == 2)
    {
        outRightTrimSlider.setColour(juce::Slider::thumbColourId, juce::Colours::red);
        outRightTrimSlider.setColour(juce::Slider::trackColourId, juce::Colours::darkred);
    }
    
    if (peakedOutputChannels == 3)
    {
        outLeftTrimSlider.setColour(juce::Slider::thumbColourId, juce::Colours::red);
        outLeftTrimSlider.setColour(juce::Slider::trackColourId, juce::Colours::darkred);
        outRightTrimSlider.setColour(juce::Slider::thumbColourId, juce::Colours::red);
        outRightTrimSlider.setColour(juce::Slider::trackColourId, juce::Colours::darkred);
    }

    
    //===================== IMPLEMENTING PEAK READING FUNCTIONALITY ======================
    
    double meterReductionRate = 0.992;
    
    newLeftPeakValue = audioProcessor.returnPeakValues(0);
    newRightPeakValue = audioProcessor.returnPeakValues(1);
    
    //REFRESHING AND/OR SMOOTHING VALUES
    if (newLeftPeakValue > oldLeftPeakValue)
    {
        oldLeftPeakValue = newLeftPeakValue;
    }
    else
    {
        oldLeftPeakValue *= meterReductionRate;
    }
    
    if (newRightPeakValue > oldRightPeakValue)
    {
        oldRightPeakValue = newRightPeakValue;
    }
    else
    {
        oldRightPeakValue *= meterReductionRate;
    }

    
    //PAINTING THE METERS
    float meterBot = 172.f;
    float meterAvailableHeight = 145.f;
    
    float leftPeakTop;
    float rightPeakTop;

    if (oldLeftPeakValue >= 1.f)
    {
        leftPeakTop = meterBot - meterAvailableHeight;
    }
    else if (oldLeftPeakValue <= juce::Decibels::decibelsToGain(-60.f))
    {
        leftPeakTop = meterBot;
    }
    else
    {
        float peakLeftInDb = juce::Decibels::gainToDecibels(oldLeftPeakValue);
        leftPeakTop = meterBot - (1 - abs(peakLeftInDb) / 60.f) * meterAvailableHeight;
    }
    
    if (oldRightPeakValue >= 1.f)
    {
        rightPeakTop = meterBot - meterAvailableHeight;
    }
    else if (oldRightPeakValue <= juce::Decibels::decibelsToGain(-60.f))
    {
        rightPeakTop = meterBot;
    }
    else
    {
        float peakRightInDb = juce::Decibels::gainToDecibels(oldRightPeakValue);
        rightPeakTop = meterBot - (1 - abs(peakRightInDb) / 60.f) * meterAvailableHeight;
    }
    
    float leftPeakHeight = meterBot - leftPeakTop;
    float rightPeakHeight = meterBot - rightPeakTop;
    leftPeakMeter.setBounds(516, leftPeakTop, 14, leftPeakHeight);
    rightPeakMeter.setBounds(548, rightPeakTop, 14, rightPeakHeight);
    
    
    
    //===================== IMPLEMENTING RMS READING FUNCTIONALITY ======================
    newLeftRmsValue = audioProcessor.returnRmsValues(0);
    newRightRmsValue = audioProcessor.returnRmsValues(1);

    //REFRESHING AND/OR SMOOTHING VALUES
    if (newLeftRmsValue >= oldLeftRmsValue)
    {
        oldLeftRmsValue = newLeftRmsValue;
    }
    else
    {
        oldLeftRmsValue *= meterReductionRate;
    }
    
    if (newRightRmsValue >= oldRightRmsValue)
    {
        oldRightRmsValue = newRightRmsValue;
    }
    else
    {
        oldRightRmsValue *= meterReductionRate;
    }
    
   
    //PAINTING THE METERS
    float leftRmsTop;
    float rightRmsTop;
    
    if (oldLeftRmsValue >= 1.0f)
    {
        leftRmsTop = meterBot - meterAvailableHeight;
    }
    else if (oldLeftRmsValue <= juce::Decibels::decibelsToGain(-60.0f))
    {
        leftRmsTop = meterBot;
    }
    else
    {
        float rmsLeftInDb = juce::Decibels::gainToDecibels(oldLeftRmsValue);
        leftRmsTop = meterBot - (1 - abs(rmsLeftInDb) / 60.f) * meterAvailableHeight;
    }
    
    
    if (oldRightRmsValue >= 1.0f)
    {
        rightRmsTop = meterBot - meterAvailableHeight;
    }
    else if (oldRightRmsValue <= juce::Decibels::decibelsToGain(-60.0f))
    {
        rightRmsTop = meterBot;
    }
    else
    {
        float rmsRightInDb = juce::Decibels::gainToDecibels(oldRightRmsValue);
        rightRmsTop = meterBot - (1 - abs(rmsRightInDb) / 60.f) * meterAvailableHeight;
    }
    
    float leftRmsHeight = meterBot - leftRmsTop;
    float rightRmsHeight = meterBot - rightRmsTop;
    
    leftRmsMeter.setBounds(516, leftRmsTop, 14, leftRmsHeight);
    rightRmsMeter.setBounds(548, rightRmsTop, 14, rightRmsHeight);
    
    
    //========= UPDATING LABEL TEXTS ==================
    if (oldLeftPeakValue >= maxLeftPeakValue)
    {
        maxLeftPeakValue = oldLeftPeakValue;
        float x = maxLeftPeakValue;
        x = juce::Decibels::gainToDecibels(x);
        leftPeakLabelPtr->setText(toTwoDecString(x), juce::NotificationType::dontSendNotification);
    }
    if (oldRightPeakValue >= maxRightPeakValue)
    {
        maxRightPeakValue = oldRightPeakValue;
        float x = maxRightPeakValue;
        x = juce::Decibels::gainToDecibels(x);
        rightPeakLabelPtr->setText(toTwoDecString(x), juce::NotificationType::dontSendNotification);
    }
    

}




//========================= RESETING OUTPUT PEAKING INDICATORS =========================
void DickoderAudioProcessorEditor::sliderDragStarted(juce::Slider*  slider)
{
    if (slider == &outLeftTrimSlider)
    {
        outLeftTrimSlider.setColour(juce::Slider::thumbColourId, juce::Colours::cadetblue);
        outLeftTrimSlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::teal);
        outRightTrimSlider.setColour(juce::Slider::thumbColourId, juce::Colours::cadetblue);
        outRightTrimSlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::teal);
    }
    
    if (slider == &outRightTrimSlider)
    {
        outLeftTrimSlider.setColour(juce::Slider::thumbColourId, juce::Colours::cadetblue);
        outLeftTrimSlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::teal);
        outRightTrimSlider.setColour(juce::Slider::thumbColourId, juce::Colours::cadetblue);
        outRightTrimSlider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::teal);
    }
}


//========================== LINKING THE OUTPUT SLIDERS (ONE WAY) ===========================
void DickoderAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &outLeftTrimSlider)
    {
        if (linkButton.getToggleState())
        {
            double valueDiff = outLeftTrimSlider.getValue() - oldLeftOutValue;
            if (outRightTrimSlider.getValue() + valueDiff > 2)
            {
                outRightTrimSlider.setValue(2);
            }
            else if (outRightTrimSlider.getValue() + valueDiff < 0)
            {
                outRightTrimSlider.setValue(0);
            }
            else
            {
                outRightTrimSlider.setValue(outRightTrimSlider.getValue() + valueDiff);
            }
        }
    oldLeftOutValue = outLeftTrimSlider.getValue();
    }
  
}


    
    
    
