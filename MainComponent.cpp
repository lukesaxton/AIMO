//  AI:MO is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License, version 3,
//  as published by the Free Software Foundation.
//
//  AI:MO is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "MainComponent.h"

//==============================================================================
MainContentComponent::MainContentComponent() : controllerOne("controllerOne")
{
    Logger::setCurrentLogger(&logger);
    Logger::writeToLog("Hello");
    
    setSize (800, 400);

    // specify the number of input and output channels that we want to open
    ///setAudioChannels (2, 2);
    
    //addAndMakeVisible(inputManager);
    //addAndMakeVisible(controllerOne);
    addAndMakeVisible(kTMController);
    addAndMakeVisible(logger);
    
    
}

MainContentComponent::~MainContentComponent()
{
    shutdownAudio();
}

//==============================================================================
void MainContentComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainContentComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
}

void MainContentComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainContentComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    
    
    g.fillAll (Colours::limegreen);


    // You can add your drawing code here!
}

void MainContentComponent::resized()
{
    //inputManager.setBounds(0, 0, 60, getHeight());
    kTMController.setBounds(0, 0, (getWidth()/2), getHeight());
    logger.setBounds(getLocalBounds().withLeft(kTMController.getRight()));
    //kTMController.setBounds(controllerOne.getBounds().translated((getWidth()*0.5)-30, 0));
}

#endif  // MAINCOMPONENT_H_INCLUDED
