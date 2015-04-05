/*
 * ZamGate
 * Copyright (C) 2014  Damien Zammit <damien@zamaudio.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * For a full copy of the GNU General Public License see the doc/GPL.txt file.
 */

#include "ZamGatePlugin.hpp"
#include "ZamGateUI.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamGateUI::ZamGateUI()
    : UI()
{
    setSize(ZamGateArtwork::zamgateWidth, ZamGateArtwork::zamgateHeight);

    // background
    fImgBackground = Image(ZamGateArtwork::zamgateData, ZamGateArtwork::zamgateWidth, ZamGateArtwork::zamgateHeight, GL_BGR);

    // led images
    fLedRedImg = Image(ZamGateArtwork::ledredData, ZamGateArtwork::ledredWidth, ZamGateArtwork::ledredHeight);
    fLedYellowImg = Image(ZamGateArtwork::ledyellowData, ZamGateArtwork::ledyellowWidth, ZamGateArtwork::ledyellowHeight);

    // led values
    fLedRedValue = 0.0f;
    fLedYellowValue = 0.0f;

    // knob
    Image knobImage(ZamGateArtwork::knobData, ZamGateArtwork::knobWidth, ZamGateArtwork::knobHeight);

    // knob
    fKnobAttack = new ImageKnob(this, knobImage);
    fKnobAttack->setAbsolutePos(24, 45);
    fKnobAttack->setId(ZamGatePlugin::paramAttack);
    fKnobAttack->setRange(0.1f, 200.0f);
    fKnobAttack->setUsingLogScale(true);
    fKnobAttack->setDefault(10.0f);
    fKnobAttack->setRotationAngle(240);
    fKnobAttack->setCallback(this);

    fKnobRelease = new ImageKnob(this, knobImage);
    fKnobRelease->setAbsolutePos(108, 45);
    fKnobRelease->setId(ZamGatePlugin::paramRelease);
    fKnobRelease->setRange(50.0f, 500.0f);
    fKnobRelease->setStep(1.0f);
    fKnobRelease->setDefault(80.0f);
    fKnobRelease->setRotationAngle(240);
    fKnobRelease->setCallback(this);

    fKnobThresh = new ImageKnob(this, knobImage);
    fKnobThresh->setAbsolutePos(191.7, 45);
    fKnobThresh->setId(ZamGatePlugin::paramThresh);
    fKnobThresh->setRange(-60.0f, 0.0f);
    fKnobThresh->setStep(1.0f);
    fKnobThresh->setDefault(0.0f);
    fKnobThresh->setRotationAngle(240);
    fKnobThresh->setCallback(this);

    fKnobMakeup = new ImageKnob(this, knobImage);
    fKnobMakeup->setAbsolutePos(273, 45);
    fKnobMakeup->setId(ZamGatePlugin::paramMakeup);
    fKnobMakeup->setRange(-30.0f, 30.0f);
    fKnobMakeup->setStep(1.0f);
    fKnobMakeup->setDefault(0.0f);
    fKnobMakeup->setRotationAngle(240);
    fKnobMakeup->setCallback(this);

    // set default values
    d_programChanged(0);
}

// -----------------------------------------------------------------------
// DSP Callbacks

void ZamGateUI::d_parameterChanged(uint32_t index, float value)
{
    switch (index)
    {
    case ZamGatePlugin::paramAttack:
        fKnobAttack->setValue(value);
        break;
    case ZamGatePlugin::paramRelease:
        fKnobRelease->setValue(value);
        break;
    case ZamGatePlugin::paramThresh:
        fKnobThresh->setValue(value);
        break;
    case ZamGatePlugin::paramMakeup:
        fKnobMakeup->setValue(value);
        break;
    case ZamGatePlugin::paramGainR:
        if (fLedRedValue != value)
        {
            fLedRedValue = value;
            repaint();
        }
        break;
    case ZamGatePlugin::paramOutputLevel:
        if (fLedYellowValue != value)
        {
            fLedYellowValue = value;
            repaint();
        }
        break;
    }
}

void ZamGateUI::d_programChanged(uint32_t index)
{
    if (index != 0)
        return;

    // Default values
    fKnobAttack->setValue(10.0f);
    fKnobRelease->setValue(80.0f);
    fKnobThresh->setValue(0.0f);
    fKnobMakeup->setValue(0.0f);
}

// -----------------------------------------------------------------------
// Widget Callbacks

void ZamGateUI::imageKnobDragStarted(ImageKnob* knob)
{
    d_editParameter(knob->getId(), true);
}

void ZamGateUI::imageKnobDragFinished(ImageKnob* knob)
{
    d_editParameter(knob->getId(), false);
}

void ZamGateUI::imageKnobValueChanged(ImageKnob* knob, float value)
{
    d_setParameterValue(knob->getId(), value);
}

void ZamGateUI::onDisplay()
{
    fImgBackground.draw();

    // draw leds
    static const float sLedSpacing  = 15.5f;
    static const int   sLedInitialX = 344.7;

    static const int sYellowLedStaticY = 16;
    static const int sRedLedStaticY    = 45;

    int numRedLeds;
    int numYellowLeds;

	if (fLedRedValue >= 40.f)
		numRedLeds = 12;
	else if (fLedRedValue >= 30.f)
		numRedLeds = 11;
	else if (fLedRedValue >= 20.f)
		numRedLeds = 10;
	else if (fLedRedValue >= 15.f)
		numRedLeds = 9;
	else if (fLedRedValue >= 10.f)
		numRedLeds = 8;
	else if (fLedRedValue >= 8.f)
		numRedLeds = 7;
	else if (fLedRedValue >= 6.f)
		numRedLeds = 6;
	else if (fLedRedValue >= 5.f)
		numRedLeds = 5;
	else if (fLedRedValue >= 4.f)
		numRedLeds = 4;
	else if (fLedRedValue >= 3.f)
		numRedLeds = 3;
	else if (fLedRedValue >= 2.f)
		numRedLeds = 2;
	else if (fLedRedValue >= 1.f)
		numRedLeds = 1;
	else numRedLeds = 0;

    for (int i=numRedLeds; i>0; --i)
        fLedRedImg.drawAt(sLedInitialX + (12 - i)*sLedSpacing, sRedLedStaticY);

	if (fLedYellowValue >= 20.f)
		numYellowLeds = 19;
	else if (fLedYellowValue >= 10.f)
		numYellowLeds = 18;
	else if (fLedYellowValue >= 8.f)
		numYellowLeds = 17;
	else if (fLedYellowValue >= 4.f)
		numYellowLeds = 16;
	else if (fLedYellowValue >= 2.f)
		numYellowLeds = 15;
	else if (fLedYellowValue >= 1.f)
		numYellowLeds = 14;
	else if (fLedYellowValue >= 0.f)
		numYellowLeds = 13;
	else if (fLedYellowValue >= -1.f)
		numYellowLeds = 12;
	else if (fLedYellowValue >= -2.f)
		numYellowLeds = 11;
	else if (fLedYellowValue >= -3.f)
		numYellowLeds = 10;
	else if (fLedYellowValue >= -4.f)
		numYellowLeds = 9;
	else if (fLedYellowValue >= -5.f)
		numYellowLeds = 8;
	else if (fLedYellowValue >= -6.f)
		numYellowLeds = 7;
	else if (fLedYellowValue >= -8.f)
		numYellowLeds = 6;
	else if (fLedYellowValue >= -10.f)
		numYellowLeds = 5;
	else if (fLedYellowValue >= -15.f)
		numYellowLeds = 4;
	else if (fLedYellowValue >= -20.f)
		numYellowLeds = 3;
	else if (fLedYellowValue >= -30.f)
		numYellowLeds = 2;
	else if (fLedYellowValue >= -40.f)
		numYellowLeds = 1;
	else numYellowLeds = 0;

	if (numYellowLeds > 12) {
		for (int i=12; i<numYellowLeds; ++i)
			fLedRedImg.drawAt(sLedInitialX + i*sLedSpacing, sYellowLedStaticY);
		for (int i=0; i<12; ++i)
			fLedYellowImg.drawAt(sLedInitialX + i*sLedSpacing, sYellowLedStaticY);
	} else {
		for (int i=0; i<numYellowLeds; ++i)
			fLedYellowImg.drawAt(sLedInitialX + i*sLedSpacing, sYellowLedStaticY);
	}
}

// -----------------------------------------------------------------------

UI* createUI()
{
    return new ZamGateUI();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
